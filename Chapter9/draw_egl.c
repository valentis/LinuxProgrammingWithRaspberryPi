#include <stdio.h>
#include <unistd.h>
#include <bcm_host.h> 			/* BCM GPU 사용을 위한 헤더 파일 */
#include <EGL/egl.h> 			/* EGL을 위한 헤더 파일 */
#include <EGL/eglext.h>
#include <GLES/gl.h> 			/* OpenGL ES를 위한 헤더 파일 */

int main(int argc, const char **argv)
{
    /* 화면의 크기를 저장하기 위한 변수 */
    uint32_t screenWidth, screenHeight;
    
    /* EGL을 위한 변수 선언 */
    EGLDisplay gDisplay;
    EGLSurface gSurface;
    EGLContext gContext;
    EGLConfig gConfig;
    EGLBoolean result;
    EGLint numConfig;
    int32_t success = 0;
    
    /* 라즈베리 파이에서 GPU 출력을 위한 자료형 */
    static EGL_DISPMANX_WINDOW_T nativeWindow;
    DISPMANX_ELEMENT_HANDLE_T dispmanElement;
    DISPMANX_DISPLAY_HANDLE_T dispmanDisplay;
    DISPMANX_UPDATE_HANDLE_T dispmanUpdate;
    VC_RECT_T dst_rect, src_rect;
    
    /* eglChooseConfig() 함수에서 사용할 설정값 */
    static const EGLint attribute_list[] = {
                        EGL_RED_SIZE, 8,
                        EGL_GREEN_SIZE, 8,
                        EGL_BLUE_SIZE, 8,
                        EGL_ALPHA_SIZE, 8,
                        EGL_DEPTH_SIZE, 16, 	/* 색상 깊이(Color Depth) 버퍼를 위해 사용 */
                        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
                        EGL_NONE
    };
    
    /* eglCreateContext() 함수에서 사용할 설정값 */
    static const EGLint context_attributes[] = {
                        EGL_CONTEXT_CLIENT_VERSION, 1, 	/* OpenGL ES 1.x: 1, OpenGL ES 2: 2 */
                        EGL_NONE
    };
    
    bcm_host_init(); 			/* BCM GPU를 위한 초기화 수행 */
    
    /* EGL 디스플레이(display) 연결을 위한 EGLDisplay 객체 획득 */
    gDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    
    /* EGLDisplay 객체의 초기화 */
    result = eglInitialize(gDisplay, NULL, NULL);
    
    /* EGL 프레임 버퍼 설정을 위해서 필요한 EGLConfig 객체 획득 */
    result = eglChooseConfig(gDisplay, attribute_list, &gConfig, 1, &numConfig);
    
    /* EGL에 OpenGL ES를 사용하기 위한 설정 */
    result = eglBindAPI(EGL_OPENGL_ES_API);
    
    /* EGL 렌더링을 위한 콘텍스트(EGLContext) 생성 */
    gContext = eglCreateContext(gDisplay, gConfig, EGL_NO_CONTEXT, context_attributes);
    
    /* 현재 화면의 크기를 가져온다. */
    success = graphics_get_display_size(0 /* LCD */, &screenWidth, &screenHeight);
    
    /* 라즈베리 파이에서 출력을 위한 설정 */
    dst_rect.x = 0;
    dst_rect.y = 0;
    dst_rect.width = screenWidth;
    dst_rect.height = screenHeight;
    src_rect.x = 0;
    src_rect.y = 0;
    src_rect.width = screenWidth << 16;
    src_rect.height = screenHeight << 16;
    
    /* 라즈베리 파이의 출력을 위한 디스플레이(DISPMANX_DISPLAY_HANDLE_T) 획득 */
    dispmanDisplay = vc_dispmanx_display_open(0 /* LCD */);
    
    /* 화면 갱신을 위한 DISPMANX_UPDATE_HANDLE_T 자료형을 가져온다. */
    dispmanUpdate = vc_dispmanx_update_start(0);
    
    /* 라즈베리 파이에서 디스플레이 요소(DISPMANX_ELEMENT_HANDLE_T)를 추가한다. */
    dispmanElement = vc_dispmanx_element_add(dispmanUpdate, dispmanDisplay,
                0/*layer*/, &dst_rect, 0/*src*/,
                &src_rect, DISPMANX_PROTECTION_NONE,
                0 /*alpha*/, 0/*clamp*/, (DISPMANX_TRANSFORM_T)0/*transform*/);

    nativeWindow.element = dispmanElement;
    nativeWindow.width = screenWidth;
    nativeWindow.height = screenHeight;
    vc_dispmanx_update_submit_sync(dispmanUpdate); 	/* 화면 갱신을 위한 싱크 수행 */
    
    /* EGL 렌더링을 위한 윈도우 화면(EGLSurface) 객체를 생성한다. */
    gSurface = eglCreateWindowSurface(gDisplay, gConfig, &nativeWindow, NULL);
    
    /* EGL 콘텍스트와 EGL 화면을 서로 연결한다. */
    result = eglMakeCurrent(gDisplay, gSurface, gSurface, gContext);
    
    /* OpenGL에서 glClear() 함수로 버퍼를 지울 때의 색상을 설정 */
    glClearColor(0.15f, 0.25f, 0.35f, 1.0f);
    
    /* 색상 버퍼 비트(GL_COLOR_BUFFER_BIT) 지우기 */
    glClear(GL_COLOR_BUFFER_BIT);
    
    /* 현재 윈도우에 OpenGL의 표시를 위한 영역 설정 */
    glViewport(0, 0, screenWidth, screenHeight);

    eglSwapBuffers(gDisplay, gSurface); 		/* OpenGL ES의 내용을 EGL로 출력 */
    
    getchar();
    
    bcm_host_deinit(); /* BCM GPU를 위한 정리 작업 수행 */
    
    return 0;
}
