#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <GLES/gl.h> 		/* OpenGL ES를 사용하기 위해서 */
#include <GLES2/gl2.h>
#include <EGL/egl.h> 		/* EGL을 사용하기 위해서 */

#include "bcm_host.h" 		/* 브로드컴(Broadcom)의 VideoCore 사용을 위해서 */

/* 셰이더 객체를 생성하여 셰이더 소스를 로드한 후, 셰이더를 컴파일 */
GLuint LoadShader(GLenum type, const char *shaderSrc)
{
    GLuint shaderId; 		/* 셰이더의 ID */
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
            char* infoLog = malloc(sizeof(char) * infoLen);
            glGetShaderInfoLog(shaderId, infoLen, NULL, infoLog);
            free(infoLog);
        }

        /* 에러 시 셰이더 삭제 */
        glDeleteShader(shaderId);

        return 0;
    }

    return shaderId;
}
    
int main(int argc, const char **argv)
{
    /* 해상도를 위한 초깃값을 1920 × 1080(FULL HD)로 지정 */
    uint32_t GScreenWidth = 1920;
    uint32_t GScreenHeight = 1080;
    int32_t success = 0;

    /* EGL 사용을 위한 변수들 */
    EGLDisplay GDisplay;
    EGLSurface GSurface;
    EGLContext GContext;
    EGLConfig config;
    EGLBoolean result;
    EGLint num_config;
    static EGL_DISPMANX_WINDOW_T nativewindow;

    DISPMANX_ELEMENT_HANDLE_T dispman_element;
    DISPMANX_DISPLAY_HANDLE_T dispman_display;
    DISPMANX_UPDATE_HANDLE_T dispman_update;
    VC_RECT_T dst_rect;
    VC_RECT_T src_rect;

    static const EGLint attribute_list[] = {
            EGL_RED_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_BLUE_SIZE, 8,
            EGL_ALPHA_SIZE, 8,
            EGL_DEPTH_SIZE, 16, 	/* 깊이 버퍼링(Depth Buffering)을 하기 위해서 필요 */
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_NONE
    };

    static const EGLint context_attributes[] = {
            EGL_CONTEXT_CLIENT_VERSION, 2, 	/* OpenGL ES 2.0을 위한 버전 설정 */
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
    static GLubyte front[] = {2, 1, 3, 0}; 	/* 앞면(front face) */
    static GLubyte back[] = {5, 6, 4, 7}; 	/* 뒷면(back face) */
    static GLubyte top[] = {6, 2, 7, 3}; 	/* 윗면(top face) */
    static GLubyte bottom[] = {1, 5, 0 ,4}; 	/* 바닥면(bottom face) */
    static GLubyte left[] = {3, 0, 7, 4}; 	/* 왼쪽면(left face) */
    static GLubyte right[] = {6, 5, 2, 1}; 	/* 오른쪽면(right face) */
    
    /* 버텍스 셰이더(Shader)를 위한 소스 코드 */
    GLbyte vShaderStr[] =
        "attribute vec4 vPosition; \n"
        "void main() \n"
        "{ \n"
        " gl_Position = vPosition; \n"
        "} \n";

    /* 프래그먼트 셰이더(Shader)를 위한 소스 코드 */
    GLbyte fShaderStr[] =
        "precision mediump float; \n"
        "void main() \n"
        "{ \n"
        " gl_FragColor = vec4(0.5, 0.5, 0.0, 1.0); \n"
        "} \n";
    
    GLuint vertexShader;
    GLuint fragmentShader;
    GLuint programID;
    GLint linked;
    bcm_host_init(); 			/* 브로드컴의 VideoCore 초기화 */

    /* EGL 디스플레이(display) 연결(connection)을 위한 객체 획득 */
    GDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);

    /* EGL 디스플레이(display) 연결(connection)을 위한 객체 초기화 */
    result = eglInitialize(GDisplay, NULL, NULL);

    /* EGL 프레임 버퍼 설정을 위한 객체 획득 */
    result = eglChooseConfig(GDisplay, attribute_list, &config, 1, &num_config);

    /* EGL과 OpenGL ES를 연결 */
    result = eglBindAPI(EGL_OPENGL_ES_API);

    /* EGL 렌더링 콘텍스트(context) 생성 */
    GContext = eglCreateContext(GDisplay, config, EGL_NO_CONTEXT,
    context_attributes);

    /* EGL 윈도우 표면(window surface) 생성 */
    success = graphics_get_display_size(0/* LCD */, &GScreenWidth, &GScreenHeight);

    /* 화면의 중심에 객체가 표시되도록 설정 */
    dst_rect.x = (GScreenWidth-GScreenHeight) / 2; 	/* dst_rect.x = 0; */
    dst_rect.y = 0;
    dst_rect.width = GScreenHeight; 			/* dst_rect.width = GScreenWidth; */
    dst_rect.height = GScreenHeight;
    src_rect.x = 0;
    src_rect.y = 0;
    src_rect.width = GScreenWidth << 16;
    src_rect.height = GScreenHeight << 16;
    dispman_display = vc_dispmanx_display_open(0 /* LCD */);
    dispman_update = vc_dispmanx_update_start(0);
    dispman_element = vc_dispmanx_element_add(dispman_update, dispman_display,
    					      0/*layer*/, &dst_rect, 0/*src*/,
   				   	      &src_rect, DISPMANX_PROTECTION_NONE,
    					      0 /*alpha*/, 0/*clamp*/,
    					     (DISPMANX_TRANSFORM_T)0/*transform*/);
    nativewindow.element = dispman_element;
    nativewindow.width = GScreenWidth;
    nativewindow.height = GScreenHeight;
    vc_dispmanx_update_submit_sync(dispman_update);

    /* 화면 출력을 위한 EGL 윈도우 표면 생성 */
    GSurface = eglCreateWindowSurface(GDisplay, config, &nativewindow, NULL);

    /* 콘덱스트를 표면(surface)과 연결 */
    result = eglMakeCurrent(GDisplay, GSurface, GSurface, GContext);

    /* 배경색을 설정 */
    glClearColor(0.15f, 0.25f, 0.35f, 1.0f);
    glViewport(0, 0, GScreenWidth, GScreenHeight); 	/* 뷰포트 설정 */

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
    
    glMatrixMode(GL_PROJECTION); 		/* OpenGL ES의 모드를 투영으로 설정 */
    glLoadIdentity();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 	/* 화면 지우기 */
    glEnable(GL_DEPTH_TEST); 			/* 3차원을 위한 GL_DEPTH_TEST 프래그 설정 */

    /* OpenGL ES 2.0을 위한 좌표계 설정: 원근 투영 */
    GLfloat nearp = 1.0f, farp = 500.0f, hht, hwd;
    hht = nearp * (GLfloat)tan(45.0 / 2.0 / 180.0 * M_PI);
    hwd = hht * (GLfloat)GScreenWidth / (GLfloat)GScreenHeight;
    glFrustumf(-hwd, hwd, -hht, hht, nearp, farp);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glVertexAttribPointer(
        0, 		/* 인덱스(Index) */
        3, 		/* 요소의 수(x, y, z) */
        GL_FLOAT, 	/* 데이터의 형 */
        GL_FALSE, 	/* 정규화(normalized) */
        0, 		/* 간격(stride) */
        g_vertex_data 	/* 정점의 좌표를 가지고 있는 배열 */
    );

    /* 앞의 셰이더의 인덱스 0에 정육면체의 좌푯값을 설정 */
    glEnableVertexAttribArray(0);

    /* 6개의 면(face)을 다른 색으로 표시 */
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, front);
    
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, back);
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, top);
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, bottom);
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, left);
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, right);

    /* EGL로 화면 표시 */
    eglSwapBuffers(GDisplay, GSurface);

    getchar();

    return 0;
}
