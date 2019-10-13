#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <math.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <sys/ioctl.h>
#include <gbm.h>
#include <xf86drmMode.h>
#include <GL/gl.h>
#include <GLES2/gl2.h>		/* OpenGL ES 2.0를 사용하기 위해서 */
#include <EGL/egl.h> 		/* EGL을 사용하기 위해서 */

static const char* I2C_DEV = "/dev/i2c-1"; 	/* I2C를 위한 장치 파일 */
static const int I2C_SLAVE = 0x0703; 		/* ioctl() 함수에서 I2C_SLAVE 설정을 위한 값 */

static const int LSM9DS1_MAG_ID = 0x1C; 	/* SenseHAT의 I2C-1의 값 */
static const int LSM9DS1_ACC_ID = 0x6A;

static const int CTRL_REG1_G = 0x10; 		/* LSM9DS1 스펙에 나와 있는 값 */
static const int CTRL_REG4 = 0x1E;
static const int CTRL_REG6_XL = 0x20;
static const int CTRL_REG7_XL = 0x21;
static const int CTRL_REG8 = 0x22;
static const int CTRL_REG9 = 0x23;

static const int OUT_X_G = 0x18; 		/* 자이로 센서를 위한 값 */
static const int OUT_Y_G = 0x1A;
static const int OUT_Z_G = 0x1C;

static const int OUT_X_XL = 0x28; 		/* 가속도 센서를 위한 값 */
static const int OUT_Y_XL = 0x2A;
static const int OUT_Z_XL = 0x2C;

static const int OUT_X_L_M = 0x28; 		/* 지자기 센서를 위한 값 */
static const int OUT_X_H_M = 0x29;
static const int OUT_Y_L_M = 0x2A;
static const int OUT_Y_H_M = 0x2B;
static const int OUT_Z_L_M = 0x2C;
static const int OUT_Z_H_M = 0x2D;

static void getAccel(int fd, int *ax, int *ay, int *az);
static void getGyro(int fd, int *gx, int *gy, int *gz);
static void getMagneto(int fd, int *mx, int *my, int *mz);

/* 셰이더 객체를 생성하여 셰이더 소스를 로드한 후, 셰이더를 컴파일 */
GLuint LoadShader(GLenum type, const char *shaderSrc)
{
    GLuint shaderId; /* 셰이더의 ID */
    GLint compiled;
     
    /* 셰이더 객체의 생성 */
    shaderId = glCreateShader(type);

    if(shaderId == 0) return 0;

    /* 셰이더 소스 로드 */
    glShaderSource(shaderId, 1, &shaderSrc, NULL);

    /* 셰이더 컴파일 */
    glCompileShader(shaderId);

    /* 컴파일 상태 검사 */
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compiled);
    if(!compiled) {
        GLint infoLen = 0;
        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLen);
        if(infoLen > 1) {
            char* infoLog = (char*)malloc(sizeof(char) * infoLen);
            glGetShaderInfoLog(shaderId, infoLen, NULL, infoLog);
            free(infoLog);
        }

        /* 에러 시 셰이더 삭제 */
        glDeleteShader(shaderId);

        return 0;
    }

    return shaderId;
}

int main(int argc, char **argv)
{
    int device = open("/dev/dri/card1", O_RDWR);             /* DRI 장치 열기 */
    int acc_fd, mag_fd;

    /* 가속도/자이로 센서를 위한 초기화 */
    acc_fd = open(I2C_DEV, O_RDWR);
    if(ioctl(acc_fd, I2C_SLAVE, LSM9DS1_ACC_ID) < 0) {
        perror("Failed to acquire bus for accelerometer\n");
        return 1;
    }

    /* 지자기 센서를 위한 초기화 */
    mag_fd = open(I2C_DEV, O_RDWR);
    if(ioctl(mag_fd, I2C_SLAVE, LSM9DS1_MAG_ID) < 0) {
        perror("Failed to acquire bus for magnetometer\n");
        return 1;
    }

    /* 속도/자이로 센서를 위한 초기화 */
    wiringPiI2CWriteReg8(acc_fd, CTRL_REG6_XL, 0x60); 	/* 119hz accel */

    /* 자이로스코프를 모든 각도에서 사용할 수 있도록 초기화 */
    wiringPiI2CWriteReg8(acc_fd, CTRL_REG4, 0x38);
    wiringPiI2CWriteReg8(acc_fd, CTRL_REG1_G, 0x28); 	/* 0x28 = 14.9hz, 500dps */

    /* 지자기 센서를 위한 초기화 */
    wiringPiI2CWriteReg8(mag_fd, CTRL_REG6_XL, 0x48); 	/* 출력 데이터의 속도와 파워 모드 설정 */
    wiringPiI2CWriteReg8(mag_fd, CTRL_REG7_XL, 0x00); 	/* 기본 스케일(Scale) */
    wiringPiI2CWriteReg8(mag_fd, CTRL_REG8, 0x00);	/* 연속 변환 */
    wiringPiI2CWriteReg8(mag_fd, CTRL_REG9, 0x08); 	/* 고성능 모드 */

    /* EGL을 위한 변수 선언 */
    EGLDisplay gDisplay;
    EGLSurface gSurface;
    EGLContext gContext;
    EGLConfig gConfig, *gConfigs;
    EGLint cfgCnt, cfgIdx, count = 0, currFB, prevFB;
    struct gbm_bo *prevBo;

    EGLBoolean result;
    EGLint num_config;

    /* eglChooseConfig( ) 함수에서 사용할 설정값 */
    static const EGLint attribute_list[] = {
                EGL_RED_SIZE, 8,
                EGL_GREEN_SIZE, 8,
                EGL_BLUE_SIZE, 8,
                EGL_ALPHA_SIZE, 0,
                EGL_DEPTH_SIZE, 16, 	/* 깊이 버퍼링(Depth Buffering)을 하기 위해서 필요 */
                EGL_NONE
    };

    /* eglCreateContext( ) 함수에서 사용할 설정값 */
    static const EGLint context_attributes[] = {
                EGL_CONTEXT_CLIENT_VERSION, 2, 	/* OpenGL ES 1.x: 1, OpenGL ES 2: 2 */
                EGL_NONE
    };

    /* 정육면체를 위한 8개의 정점 정의 */
    GLfloat g_vertex_data[] = {
                -0.5f, -0.5f, -0.5f,
                 0.5f, -0.5f, -0.5f,
                 0.5f, 0.5f, -0.5f,
                -0.5f, 0.5f, -0.5f,
                -0.5f, -0.5f, 0.5f,
                 0.5f, -0.5f, 0.5f,
                 0.5f, 0.5f, 0.5f,
                -0.5f, 0.5f, 0.5f,
    };

    /* 정육면체의 6개 면(face)에 대한 인덱스 설정: 방향성 */
    static GLubyte front[] = {2, 1, 3, 0}; 		/* 앞면(front face) */
    static GLubyte back[] = {5, 6, 4, 7}; 		/* 뒷면(back face) */
    static GLubyte top[] = {6, 2, 7, 3}; 		/* 윗면(top face) */
    static GLubyte bottom[] = {1, 5, 0 ,4}; 	/* 바닥면(bottom face) */
    static GLubyte left[] = {3, 0, 7, 4}; 		/* 왼쪽면(left face) */
    static GLubyte right[] = {6, 5, 2, 1}; 		/* 오른쪽면(right face) */

    /* 버텍스 셰이더(Shader)를 위한 소스 코드 */
    GLbyte vShaderStr[] =
               "attribute vec4 vPosition; \n"
	       "uniform float rad; \n"
	       "varying vec4 vColor; \n"
               "void main() { \n"
	       "  mat4 xRotMat = mat4(1.0, 0.0,       0.0,      0.0, \n"
	       "                      0.0, cos(rad), -sin(rad), 0.0, \n"
	       "                      0.0, sin(rad),  cos(rad), 0.0, \n"
	       "                      0.0, 0.1,       0.0,      1.0); \n"
	       "  mat4 zRotMat = mat4(cos(rad), -sin(rad), 0.0, 0.0, \n"
	       "                      sin(rad),  cos(rad), 0.0, 0.0, \n"
	       "                      0.0,       0.0,      1.0, 0.0, \n"
	       "                      0.0,       0.0,      0.0, 1.0); \n"
	       "  gl_Position = xRotMat * zRotMat * vPosition; \n"
	       "  vColor = 0.5 * (vPosition + vec4(1.0, 1.0, 1.0, 0.0)); \n"
               "} \n";

    /* 프래그먼트 셰이더(Shader)를 위한 소스 코드 */
    GLbyte fShaderStr[] =
               "varying vec4 vColor; \n"
               "void main() \n"
               "{ \n"
	       "  gl_FragColor = vColor; \n"
               "} \n";

    GLuint vertexShader;
    GLuint fragmentShader;
    GLuint programID;
    GLint linked;
    
    /* DRM(Direct Rendering Manager) 초기화 */
    drmModeRes *resources = drmModeGetResources(device);
    drmModeConnector *connector = drmModeGetConnector(device,
                                          resources->connectors[0]);
    drmModeModeInfo modeInfo = connector->modes[0];

    drmModeEncoder *encoder = drmModeGetEncoder(device, connector->encoder_id);
    drmModeCrtc *crtc = drmModeGetCrtc(device, encoder->crtc_id);
    uint32_t connector_id = connector->connector_id;

    /* GBM(Generic Buffer Manager) 초기화 */
    struct gbm_device *gbmDev = gbm_create_device(device);
    struct gbm_surface *gbmSurface = gbm_surface_create(gbmDev,
                                      modeInfo.hdisplay, modeInfo.vdisplay,
                                      GBM_FORMAT_XRGB8888,
                                      GBM_BO_USE_SCANOUT | GBM_BO_USE_RENDERING);

    /* EGL 초기화 */
    gDisplay = eglGetDisplay(gbmDev);
    eglInitialize(gDisplay, NULL, NULL);
    eglGetConfigs(gDisplay, NULL, 0, &count);
    eglBindAPI(EGL_OPENGL_API);
    gConfigs = malloc(count * sizeof *gConfigs);
    eglChooseConfig(gDisplay, attribute_list, gConfigs, count, &cfgCnt);

    /* 현재 사용 가능한 채널 획득 */
    for(cfgIdx = 0; cfgIdx < cfgCnt; cfgIdx++) {
        EGLint id;
        if(!eglGetConfigAttrib(gDisplay, gConfigs[cfgIdx], EGL_NATIVE_VISUAL_ID, &id)) continue;
        if(id == GBM_FORMAT_XRGB8888) break;
    }
    
    /* EGL 서피스 생성 */
    gContext = eglCreateContext(gDisplay, gConfigs[cfgIdx], 
                                EGL_NO_CONTEXT, context_attributes);
    gSurface = eglCreateWindowSurface(gDisplay, gConfigs[cfgIdx], gbmSurface, NULL);
    free(gConfigs);
    eglMakeCurrent(gDisplay, gSurface, gSurface, gContext);

    /* EGL과 OpenGL 함수 수행 */
    /* 화면의 크기를 저장하기 위한 변수 */
    uint32_t screenWidth = modeInfo.hdisplay, screenHeight = modeInfo.vdisplay;

    /* OpenGL에서 glClear() 함수로 버퍼를 지울 때의 색상을 설정 */
    glClearColor(0.15f, 0.25f, 0.35f, 1.0f);
   
    /* 색상 버퍼 비트(GL_COLOR_BUFFER_BIT) 지우기 */
    glClear(GL_COLOR_BUFFER_BIT);
   
    /* 현재 윈도우에서 OpenGL을 표시하기 위한 영역의 설정 */
    //glViewport(0, 0, screenWidth, screenHeight);
    glViewport((screenWidth-screenHeight)/2, 0, screenHeight, screenHeight);

    /* 버텍스 셰이더와 프래그먼트 셰이더 불러오기 */
    vertexShader = LoadShader(GL_VERTEX_SHADER, vShaderStr);
    fragmentShader = LoadShader(GL_FRAGMENT_SHADER, fShaderStr);

    /* 프로그램 객체 생성 */
    programID = glCreateProgram();

    if(programID == 0) {
        printf("Error : glCreateProgram\n");
        return 0;
    }
    
    /* 버텍스 셰이더와 프래그먼트 셰이더를 프로그램 객체와 연결 */
    glAttachShader(programID, vertexShader);
    glAttachShader(programID, fragmentShader);

    /* 버텍스 셰이더에서 사용하고 있는 vPosition 변수에 인덱스(0)를 대입 */
    glBindAttribLocation(programID, 0, "vPosition");

    /* 프로그램을 링크 */
    glLinkProgram(programID);

    /* 링크의 상태를 검사 */
    glGetProgramiv(programID, GL_LINK_STATUS, &linked);

    /* 프로그램에 연결한 셰이더를 사용 */
    glUseProgram(programID);

    glMatrixMode(GL_PROJECTION); 	/* OpenGL ES의 모드를 투영으로 설정 */
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 	/* 화면 지우기 */
    glEnable(GL_DEPTH_TEST); 		/* 3차원을 위한 GL_DEPTH_TEST 프래그 설정 */

    /* OpenGL ES 2.0을 위한 좌표계 설정: 원근 투영 */
    GLfloat nearp = 1.0f, farp = 500.0f, hht, hwd;
    hht = nearp * (GLfloat)tan(45.0 / 2.0 / 180.0 * M_PI);
    hwd = hht * (GLfloat)screenWidth / (GLfloat)screenHeight;

    glFrustum(-hwd, hwd, -hht, hht, nearp, farp); 

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glVertexAttribPointer(
        0, /* 인덱스(Index) */
        3, /* 요소의 수(x, y, z) */
        GL_FLOAT, /* 데이터의 형 */
        GL_FALSE, /* 정규화(normalized) */
        0, /* 간격(stride) */
        g_vertex_data /* 정점의 좌표를 가지고 있는 배열 */
    );

    /* 앞의 셰이더의 인덱스 0에 정육면체의 좌푯값을 설정 */
    glEnableVertexAttribArray(0);

    GLfloat rad = 0;
    while(1) {
        int ax, ay, az;
        int gx, gy, gz;
        int mx, my, mz;
        
        getAccel(acc_fd, &ax, &ay, &az);
        printf("Accelerator : %d, %d, %d\n", ax, ay, ax);

        getGyro(acc_fd, &gx, &gy, &gz);
        printf("Gyro : %d(pitch), %d(roll), %d(yaw)\n", gx, gy, gx);
        
        getMagneto(mag_fd, &mx, &my, &mz);
        printf("magnetic : %d, %d, %d\n", mx, my, mx);

        rad += (ax > 0)?0.005:-0.005;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 	/* 화면 지우기 */

        /* 애니메이션을 위한 코드 */
        GLint location_rad = glGetUniformLocation(programID, "rad");
        glUniform1f(location_rad, rad);
    
        /* 6개의 면(face)을 다른 색으로 표시 */
        glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, front);
        glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, back);
        glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, top);
        glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, bottom);
        glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, left);
        glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, right);
        
        eglSwapBuffers(gDisplay, gSurface); /* OpenGL ES의 내용을 EGL로 출력 */
    
        /* GBM 잠금 */
        struct gbm_bo *currBo = gbm_surface_lock_front_buffer(gbmSurface);
        uint32_t handle = gbm_bo_get_handle(currBo).u32;
        uint32_t pitch = gbm_bo_get_stride(currBo);

        drmModeAddFB(device, modeInfo.hdisplay, modeInfo.vdisplay, 24, 32, pitch, handle, &currFB);
        drmModeSetCrtc(device, crtc->crtc_id, currFB, 0, 0, &connector_id, 1, &modeInfo);

        /* 사용한 버퍼 해제 */
        if(prevBo) {
            drmModeRmFB(device, prevFB);
            gbm_surface_release_buffer(gbmSurface, prevBo);
        }

        prevBo = currBo;
        prevFB = currFB;

        usleep(100);
    }

    drmModeSetCrtc(device, crtc->crtc_id, crtc->buffer_id, crtc->x, crtc->y, &connector_id, 1, &crtc->mode);
    drmModeFreeCrtc(crtc);

    /* 사용한 버퍼 해제 */
    if(prevBo) {
        drmModeRmFB(device, prevFB);
        gbm_surface_release_buffer(gbmSurface, prevBo);
    }
    
    /* GBM 장치 정리 */
    gbm_surface_destroy(gbmSurface);
    gbm_device_destroy(gbmDev);

    /* DRM 장치 정리 */
    drmModeFreeConnector(connector);
    drmModeFreeResources(resources);

    close(device);
    close(acc_fd);
    close(mag_fd);

    return 0;
}

void getAccel(int fd, int *ax, int *ay, int *az)
{
    /* 가속도 센서의 값 읽어오기 */ 
    *ax = wiringPiI2CReadReg16(fd, OUT_X_XL);
    *ay = wiringPiI2CReadReg16(fd, OUT_Y_XL);
    *az = wiringPiI2CReadReg16(fd, OUT_Z_XL);

    /* 경계 검사 */
    if(*ax > 32767) *ax -= 65536;
    if(*ay > 32767) *ay -= 65536;
    if(*az > 32767) *az -= 65536;
}

void getGyro(int fd, int *gx, int *gy, int *gz)
{
    /* 자이로 센서의 값 읽어오기 */
    *gx = wiringPiI2CReadReg16(fd, OUT_X_G);
    *gy = wiringPiI2CReadReg16(fd, OUT_Y_G);
    *gz = wiringPiI2CReadReg16(fd, OUT_Z_G);
}

void getMagneto(int fd, int *mx, int *my, int *mz)
{
    /* 지자기 센서의 값 읽어오기 */
    unsigned char out_x_l_m = wiringPiI2CReadReg8(fd, OUT_X_L_M);
    unsigned char out_x_h_m = wiringPiI2CReadReg8(fd, OUT_X_H_M);
    unsigned char out_y_l_m = wiringPiI2CReadReg8(fd, OUT_Y_L_M);
    unsigned char out_y_h_m = wiringPiI2CReadReg8(fd, OUT_Y_H_M);
    unsigned char out_z_l_m = wiringPiI2CReadReg8(fd, OUT_Z_L_M);
    unsigned char out_z_h_m = wiringPiI2CReadReg8(fd, OUT_Z_H_M);

    *mx = out_x_l_m + (out_x_h_m << 8);
    *my = out_y_l_m + (out_y_h_m << 8);
    *mz = out_z_l_m + (out_z_h_m << 8);

    /* 부호 있는 값으로 수정 */
    if(*mx > 32767) *mx -= 65536;
    if(*my > 32767) *my -= 65536;
    if(*mz > 32767) *mz -= 65536;
}


