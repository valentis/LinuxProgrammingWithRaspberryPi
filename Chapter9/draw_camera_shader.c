#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>					/* 저수준(Low level) I/O를 위해 사용 */
#include <errno.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/fb.h>					/* 프레임버퍼를 위한 헤더 파일 */
#include <linux/videodev2.h>                            /* Video4Linux2를 위한 헤더파일 */
#include <gbm.h>					/* GBM을 위한 헤더 파일 */
#include <xf86drmMode.h>				/* DRM을 위한 헤더 파일 */
#include <EGL/egl.h>					/* EGL을 위한 헤더 파일 */
#include <GLES2/gl2.h>					/* OpenGL을 위한 헤더 파일 */

#define VIDEODEV        "/dev/video0"               	/* Pi Camera를 위한 디바이스 파일 */
#define FBDEV           "/dev/fb0"                    	/* 프레이버퍼를 위한 디바이스 파일 */
#define WIDTH           800                             /* 캡쳐받을 영상의 크기 */
#define HEIGHT          600                

/* Video4Linux에서 사용할 영상 저장을 위한 버퍼 */
struct buffer {
    void* start;
    size_t length;
};

static int fd = -1;                                     /* Pi Camera의 장치의 파일 디스크립터 */
static int fbfd = -1;                                   /* 프레임버퍼의 파일 디스크립터 */
//static unsigned char *pixels = NULL;                    /* 이미지 데이터를 위한 변수 */
struct buffer *buffers = NULL;                          /* Pi Camera의MMAP를 위한 변수 */
static struct fb_var_screeninfo vinfo;                  /* 프레임버퍼의 정보 저장을 위한 구조체 */
static int device;					/* DRM 장치를 위한 변수 */
static EGLDisplay gDisplay;				/* EGL을 위한 변수들 */
static EGLSurface gSurface;
static EGLContext gContext;
static drmModeRes *resources;				/* DRM을 위한 변수들 */
static drmModeModeInfo modeInfo;
static drmModeCrtc *crtc;
static struct gbm_device *gbmDev;			/* GBM을 위한 변수들 */
static struct gbm_surface *gbmSurface;
static struct gbm_bo *prevBo = NULL;
static EGLint prevFB;
static uint32_t connector_id;
static drmModeConnector *connector;
static GLuint programObject;				/* 텍스처매핑을 위한 변수 */

static void processImage(const void *p);
static int readFrame( );
static void initRead(unsigned int buffer_size);

static int initEGL(void)
{
    /* EGL을 위한 변수 선언 */
    EGLConfig gConfig, *gConfigs;
    EGLint cfgCnt, cfgIdx, count = 0;

    /* eglChooseConfig( ) 함수에서 사용할 설정값 */
    static const EGLint attribute_list[] = {
                EGL_RED_SIZE, 8,
                EGL_GREEN_SIZE, 8,
                EGL_BLUE_SIZE, 8,
                EGL_ALPHA_SIZE, 0,
                EGL_NONE
    };

    /* eglCreateContext( ) 함수에서 사용할 설정값 */
    static const EGLint context_attributes[] = {
                EGL_CONTEXT_CLIENT_VERSION, 2, /* OpenGL ES 1.x: 1, OpenGL ES 2: 2 */
                EGL_NONE
    };

    /* DRM(Direct Rendering Manager) 초기화 */
    resources = drmModeGetResources(device);
    connector = drmModeGetConnector(device, resources->connectors[0]);
    modeInfo = connector->modes[0];

    drmModeEncoder *encoder = drmModeGetEncoder(device, connector->encoder_id);
    crtc = drmModeGetCrtc(device, encoder->crtc_id);
    connector_id = connector->connector_id;

    /* GBM(Generic Buffer Manager) 초기화 */
    gbmDev = gbm_create_device(device);
    gbmSurface = gbm_surface_create(gbmDev, modeInfo.hdisplay, modeInfo.vdisplay,
                                      GBM_FORMAT_XRGB8888,
                                      GBM_BO_USE_SCANOUT | GBM_BO_USE_RENDERING);

    /* EGL 초기화 */
    gDisplay = eglGetDisplay(gbmDev);
    eglInitialize(gDisplay, NULL, NULL);
    eglGetConfigs(gDisplay, NULL, 0, &count);
    //eglBindAPI(EGL_OPENGL_API);
    eglBindAPI(EGL_OPENGL_ES_API);			    /* EGL과 OpenGL ES를 연결 */
    gConfigs = malloc(count * sizeof *gConfigs);
    eglChooseConfig(gDisplay, attribute_list, gConfigs, count, &cfgCnt);

    /* 현재 사용 가능한 채널 획득 */
    for(cfgIdx = 0; cfgIdx < cfgCnt; cfgIdx++) {
        EGLint id;
        if(!eglGetConfigAttrib(gDisplay, gConfigs[cfgIdx], EGL_NATIVE_VISUAL_ID, &id)) continue;
        if(id == GBM_FORMAT_XRGB8888) break;
    }
    
    /* EGL 렌더링 콘텍스트(context) 생성 */
    gContext = eglCreateContext(gDisplay, gConfigs[cfgIdx], 
                                EGL_NO_CONTEXT, context_attributes);

    /* 화면 출력을 위한 EGL 윈도우 표면 생성 */
    gSurface = eglCreateWindowSurface(gDisplay, gConfigs[cfgIdx], gbmSurface, NULL);
    free(gConfigs);
    /* 콘덱스트를 표면(surface)과 연결 */
    eglMakeCurrent(gDisplay, gSurface, gSurface, gContext);

    return EXIT_SUCCESS;
}

/* 셰이더 객체를 생성하여 셰이더 소스를 로드한 후, 셰이더를 컴파일 */
GLuint LoadShader(GLenum type, const char *shaderSrc)
{
    GLuint shaderId; /* 셰이더의 ID */
    GLint compiled;

    /* 셰이더 객체의 생성 */
    shaderId = glCreateShader(type);
    if(shaderId == 0) return 0;

    glShaderSource(shaderId, 1, &shaderSrc, NULL); 	/* 셰이더 소스 로드 */
    glCompileShader(shaderId); 				/* 셰이더 컴파일 */

    /* 컴파일 상태 검사 */
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compiled);
    if(!compiled) {
        GLint infoLen = 0;
        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLen);
        if(infoLen > 1) {
            char* infoLog = malloc(sizeof(char) * infoLen);
            glGetShaderInfoLog(shaderId, infoLen, NULL, infoLog);
            free(infoLog);
        }

        glDeleteShader(shaderId);		 	/* 에러 시 셰이더 삭제 */

        return 0;
    }

    return shaderId;
}

/* Y, U, V 요소 설정 */
GLuint SetLayer(GLuint id, char* shaderStr)
{
    int factor = 2;
    if(id == 0) factor = 1;

    /* 텍스처 설정 */
    glActiveTexture(GL_TEXTURE0 + id);
    glGenTextures(1, &id); 					/* 텍스처 객체 생성 */
    glBindTexture(GL_TEXTURE_2D, id);		 		/* 텍스처 객체와 연결 */
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, WIDTH/factor, HEIGHT/factor, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, NULL);

    /* 필터링 모드 설정 */ 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// GL_NEAREST
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	// GL_NEAREST
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    return id;
}

/* 텍스처 이미지 생성 */
GLuint CreateTexture2D( )
{
    /* 텍스처 객체를 위한 변수 */ 
    GLuint texY = 0;
    GLuint texU = 1;
    GLuint texV = 2;

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    /* 텍스처 생성 및 설정 */
    SetLayer(texY, "y_tex");
    SetLayer(texU, "u_tex");
    SetLayer(texV, "v_tex");

    return texY; 
}

static int initOpenGL(void)
{
    /* EGL과 OpenGL 함수 수행 */
    /* 화면의 크기를 저장하기 위한 변수 */
    uint32_t screenWidth = modeInfo.hdisplay, screenHeight = modeInfo.vdisplay;

    /* 버텍스 셰이더(Shader)를 위한 소스 코드 */
    char vShaderStr[] =
        "precision mediump float;                       \n"
	"attribute vec3 a_position;               	\n"
    	"attribute vec2 a_texCoord;			\n"
    	"varying vec2 v_texCoord;			\n"
	"						\n"
        "void main( ) {					\n"
        "   gl_Position = vec4(a_position, 1);		\n"
	"   v_texCoord = a_texCoord;			\n"
        "}						\n";

    /* 프래그먼트 셰이더(Shader)를 위한 소스 코드 */
    char fShaderStr[] =
        "precision mediump float;                       \n"
        "varying vec2 v_texCoord;                       \n"
        "uniform sampler2D y_tex, u_tex, v_tex;         \n"
        "                                               \n"
        "void main( ) {                                 \n"
        "  float nx = v_texCoord.x;                     \n"
        "  float ny = v_texCoord.y;			\n"
        "  float y = texture2D(y_tex, vec2(nx, ny))[0]; \n"
        "  float u = texture2D(u_tex, vec2(nx, ny))[0]; \n"
        "  float v = texture2D(v_tex, vec2(nx, ny))[0]; \n"
        "  y = 1.1643 * (y - 0.0625);\n"
        "  u = u - 0.5; \n"
        "  v = v - 0.5; \n"
        "  float r = y + 1.5958 * v;\n"
        "  float g = y - 0.39173 * u - 0.81290 * v;\n"
        "  float b = y + 2.018 * u;\n"
        "  gl_FragColor = vec4(r, g, b, 1.0);\n"
        //"  gl_FragColor = vec4(y, y, y, 1.0);\n"
        //"  gl_FragColor = vec4 (1.0, 0.5, 0.5, 1.0);\n"
        "}                                              \n";

    /* 버텍스 셰이더와 프래그먼트 셰이더 불러오기 */
    GLuint vertexShader = LoadShader(GL_VERTEX_SHADER, vShaderStr);
    GLuint fragmentShader = LoadShader(GL_FRAGMENT_SHADER, fShaderStr);

    /* 프로그램 객체 생성 */
    programObject = glCreateProgram( );
    if(programObject == 0) {
       return 0;
    }
   
    /* 버텍스 셰이더와 프래그먼트 셰이더를 프로그램 객체와 연결 */
    glAttachShader(programObject, vertexShader);
    glAttachShader(programObject, fragmentShader);

    /* 버택스 셰이더에서 사용하고 있는 vPosition 변수에 인덱스(0)를 대입 */
    glBindAttribLocation(programObject, 0, "a_position");

    GLint linked;
    glLinkProgram(programObject); 				/* 프로그램을 링크 */
    glGetProgramiv(programObject, GL_LINK_STATUS, &linked); 	/* 링크의 상태를 검사 */
    if(!linked) {
         GLint infoLen = 0;
         glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &infoLen);
         if(infoLen > 1) {
             char *infoLog = malloc(sizeof(char) * infoLen);
             glGetProgramInfoLog(programObject, infoLen, NULL, infoLog);
             fprintf(stdout, "%s\n", infoLog);
             free(infoLog);
         }
         glDeleteProgram(programObject);

         return EXIT_FAILURE;
    }

    CreateTexture2D( ); 						/* 텍스처 생성 */

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); 			/* 배경색을 설정 */

    /* 사용한 세이터 삭제 */
    glDeleteShader(fragmentShader);
    glDeleteShader(vertexShader);

    /* 뷰포트를 사용하기 위해 현재 윈도우의 크기를 질의 */
    EGLint surface_width = WIDTH;
    EGLint surface_height = HEIGHT;

    eglQuerySurface(gDisplay, gSurface, EGL_WIDTH, &surface_width);
    eglQuerySurface(gDisplay, gSurface, EGL_HEIGHT, &surface_height);

    /* 현재 윈도우에서 OpenGL을 표시하기 위한 영역의 설정 */
    glViewport(0, 0, surface_width, surface_height);

    return EXIT_SUCCESS;
}

/* 카메라 영상을 텍스처 매핑 */
void render( )
{
    GLint verAttrib = 0, texAttrib = 1;
    GLfloat vVertices[] = {
	-1.0f,  1.0f, 0.0f,  					/* 위치 0	: 왼쪽 위 */
 	 0.0f,  0.0f,        					/* 텍스처 0 */
	 1.0f,  1.0f, 0.0f,  					/* 위치 1	: 오른쪽 위 */
 	 1.0f,  0.0f,        					/* 텍스처 1 */
  	 1.0f, -1.0f, 0.0f,  					/* 위치 2	: 오른쪽 아래 */
	 1.0f,  1.0f,        					/* 텍스처 2 */
	-1.0f, -1.0f, 0.0f,  					/* 위치 3	: 왼쪽 아래 */
	 0.0f,  1.0f         					/* 텍스처 3 */
    };

    GLshort indices[] = {0, 1, 2, 2, 3, 0};

    glUseProgram(programObject); 				/* 프로그램에 연결한 셰이더를 사용 */

    glClear(GL_COLOR_BUFFER_BIT);				/* 화면 지우기 */

    /* 버텍스의 위치 불러오기 */
    glVertexAttribPointer(verAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), vVertices);

    /* 텍스처의 좌표 불러오기 */
    glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), &vVertices[3]);

    /* 텍스처 매핑 수행 */
    const unsigned char* pixels = buffers[0].start;
    glActiveTexture(GL_TEXTURE0 + 0);
    glUniform1i(glGetUniformLocation(programObject, "y_tex"), 0);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, WIDTH, HEIGHT, GL_LUMINANCE, GL_UNSIGNED_BYTE, pixels);

    glActiveTexture(GL_TEXTURE0 + 1);
    glUniform1i(glGetUniformLocation(programObject, "u_tex"), 1); 
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, WIDTH/2, HEIGHT/2, GL_LUMINANCE, GL_UNSIGNED_BYTE, pixels+WIDTH*HEIGHT);

    glActiveTexture(GL_TEXTURE0 + 2);
    glUniform1i(glGetUniformLocation(programObject, "v_tex"), 2);    
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, WIDTH/2, HEIGHT/2, GL_LUMINANCE, GL_UNSIGNED_BYTE, pixels+(WIDTH*HEIGHT*5/4));

    /* 버텍스와 텍스처 가능 설정 */
    glEnableVertexAttribArray(verAttrib);
    glEnableVertexAttribArray(texAttrib);

    /* 삼각형들을 이용해서 객체 그리기 : 2개의 삼각형 = 1개의 사각형 */
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices);
}

static void initDevice( )
{   
    struct v4l2_capability cap;                    		/* 비디오 장치에 대한 기능을 조사한다. */
    struct v4l2_format fmt;
    unsigned int min;

    if(ioctl(fd, VIDIOC_QUERYCAP, &cap) < 0) {       		/* 장치가 V4L2를 지원하는지 조사 */
        if(errno == EINVAL) {
            perror("/dev/video0 is no V4L2 device");
            exit(EXIT_FAILURE);
        } else {
            perror("VIDIOC_QUERYCAP");
            exit(EXIT_FAILURE);
        }
    }

    /* 장치가 영상 캡쳐 기능이 있는지 조사 */
    if(!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
        perror("/dev/video0 is no video capture device");
        exit(EXIT_FAILURE);
    }

    memset(&(fmt), 0, sizeof(fmt));
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width = WIDTH;
    fmt.fmt.pix.height = HEIGHT;
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_YUV420;  //V4L2_PIX_FMT_YUYV;
    fmt.fmt.pix.field = V4L2_FIELD_NONE;

    if(ioctl(fd, VIDIOC_S_FMT, &fmt) == -1) {
        perror("VIDIOC_S_FMT");
        exit(EXIT_FAILURE);
    }

    /* 영상의 최소 크기를 구함 */
    min = fmt.fmt.pix.width * vinfo.bits_per_pixel/8;
    if(fmt.fmt.pix.bytesperline < min)
        fmt.fmt.pix.bytesperline = min;
    min = fmt.fmt.pix.bytesperline * fmt.fmt.pix.height;
    if(fmt.fmt.pix.sizeimage < min)
        fmt.fmt.pix.sizeimage = min;

    initRead(fmt.fmt.pix.sizeimage); 				/* 영상 읽기를 위한 초기화 */
}

static void initRead(unsigned int buffer_size) 
{
    /* 메모리를 할당한다 */
    buffers = (struct buffer*)calloc(1, sizeof(*buffers));
    if(!buffers) {
        perror("Out of memory");
        exit(EXIT_FAILURE);
    }

    /* 버퍼를 초기화 한다. */
    buffers[0].length = buffer_size;
    buffers[0].start = malloc(buffer_size);
    if(!buffers[0].start) {
        perror("Out of memory");
        exit(EXIT_FAILURE);
    }
#if 0
    /* 메모리를 할당한다 */
    pixels = (unsigned char*)malloc(WIDTH * HEIGHT * vinfo.bits_per_pixel/8 * sizeof(unsigned char));
    if(!pixels) {
        perror("Out of memory");
        exit(EXIT_FAILURE);
    }
#endif
} 

#define NO_OF_LOOP    100

static void mainloop( ) 
{
    unsigned int count = NO_OF_LOOP;
    while(count-- > 0) {                                               	/* 100회 반복 */
        for(;;) {
            fd_set fds;
            struct timeval tv;
            int r;

            /* fd_set 구조체를 초기화하고 비디오 장치를 파일 디스크립터를 설정한다. */
            FD_ZERO(&fds);
            FD_SET(fd, &fds);

            /* 타임아웃(Timeout)을 2초로 설정한다. */
            tv.tv_sec = 2;
            tv.tv_usec = 0;

            r = select(fd + 1, &fds, NULL, NULL, &tv);      		/* 비디오 데이터가 올때까지 대기 */
            switch(r) {
                case -1:                                         	/* select( ) 함수 에러시의 처리 */
                      if(errno != EINTR) {
                          perror("select( )");
                          exit(EXIT_FAILURE);
                      }
                      break;
                case 0:                                              	/* 타임아웃시의 처리 */
                      perror("select timeout");
                      exit(EXIT_FAILURE);
                      break;
            };

            if(readFrame( )) break;                                 	/* 현재의 프레임을 읽어서 표시 */
        }
    }
}

static int readFrame( ) 
{
  if(read(fd, buffers[0].start, buffers[0].length) < 0) {
     perror("read( )");
     exit(EXIT_FAILURE);
  }

  processImage(buffers[0].start);

  return 1;
}

/* unsigned char의 범위를 넘어가지 않도록 경계 검사를 수행다. */
unsigned char clip(int value, int min, int max)
{
    return(value > max ? max : value < min ? min : value);
}

/* YUYV를 BGRA로 변환한다. */
static void processImage(const void *p)
{
#if 0
    int width = WIDTH, height = HEIGHT;
    unsigned char* in = (unsigned char*)p;
    int y0, u, y1, v;

    for(int i = 0, x = 0; i < width*height*2; i+=4, x+=6) {
        const unsigned char *pyuv = in+i;

        /* YUYV 성분을 분리 */
        y0 = pyuv[0];
        u  = pyuv[1] - 128;
        y1 = pyuv[2];
        v  = pyuv[3] - 128;

        pixels[x+0] = clip((298 * y0 + 409 * v + 128) >> 8, 0, 255);		/* Red */
        pixels[x+1] = clip((298 * y0 - 100 * u - 208 * v + 128) >> 8, 0, 255);  /* Green */
        pixels[x+2] = clip((298 * y0 + 516 * u + 128) >> 8, 0, 255);		/* Blue */

        pixels[x+3] = clip((298 * y1 + 409 * v + 128) >> 8, 0, 255);		/* Red */
        pixels[x+4] = clip((298 * y1 - 100 * u - 208 * v + 128) >> 8, 0, 255);  /* Green */
        pixels[x+5] = clip((298 * y1 + 516 * u + 128) >> 8, 0, 255);		/* Blue */
    };
#endif
    render( );

    int rc = eglSwapBuffers(gDisplay, gSurface);
    if(rc != EGL_TRUE) {
        fprintf(stderr, "eglSwapBuffers failed\n");
        exit(EXIT_FAILURE);
    }

    /* GBM 잠금 */
    EGLint currFB;
    struct gbm_bo *currBo = gbm_surface_lock_front_buffer(gbmSurface);
    uint32_t handle = gbm_bo_get_handle(currBo).u32;
    uint32_t pitch = gbm_bo_get_stride(currBo);
    drmModeAddFB(device, modeInfo.hdisplay, modeInfo.vdisplay, 24, 32, pitch, 
                 handle, &currFB);
    drmModeSetCrtc(device, crtc->crtc_id, currFB, 0, 0, &connector_id, 1, &modeInfo);

    /* 사용한 버퍼 해제 */
    if(prevBo) {
        drmModeRmFB(device, prevFB);
        gbm_surface_release_buffer(gbmSurface, prevBo);
    }

    prevBo = currBo;
    prevFB = currFB;
}

static void uninitDevice( ) 
{
    free(buffers[0].start);
    free(buffers);
//    free(pixels);
}

int main(int argc, char ** argv)
{
    long screensize = 0;
    int rc;

    device = open("/dev/dri/card1", O_RDWR);             		/* DRI 장치 열기 */

    /* 장치 열기 */
    fd = open(VIDEODEV, O_RDWR| O_NONBLOCK, 0); 			/* Pi Camera 열기 */
    if(fd == -1) {
        perror("open( ) : video devive");
        return EXIT_FAILURE;
    }

    fbfd = open(FBDEV, O_RDWR); 					/* 프레임버퍼 열기 */
    if(fbfd == -1) {
        perror("open( ) : framebuffer device");
        return EXIT_FAILURE;
    }

    if(ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) == -1) { 		/* 프레임버퍼의 정보 가져오기 */
         perror("Error reading variable information.");
         return EXIT_FAILURE;
    }

    initDevice( );                                                 	/* 장치 초기화 */ 

    rc = initEGL( );
    if(rc != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }

    rc = initOpenGL( );
    if(rc != EXIT_SUCCESS) {
    	return EXIT_FAILURE;
    }

    mainloop( );                                                         /* 캡쳐 실행 */

    uninitDevice( );                                                   	/* 장치 해제 */

    /* 사용한 EGL 정리 작업 */
    eglMakeCurrent(gDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    eglDestroySurface(gDisplay, gSurface);
    eglDestroyContext(gDisplay, gContext);
    eglTerminate(gDisplay);
    eglReleaseThread( );

    drmModeSetCrtc(device, crtc->crtc_id, crtc->buffer_id, crtc->x, crtc->y, 
                   &connector_id, 1, &crtc->mode);
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

    /* 장치 닫기 */
    close(fbfd);
    close(fd);
    close(device);

    return EXIT_SUCCESS;                                    		/* 애플리케이션 종료 */
}

