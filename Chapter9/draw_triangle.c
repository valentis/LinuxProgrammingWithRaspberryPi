#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <gbm.h>
#include <xf86drmMode.h>
#include <GL/gl.h>
#include <EGL/egl.h>

int main(int argc, char **argv)
{
    int device = open("/dev/dri/card1", O_RDWR);             /* DRI 장치 열기 */

    /* EGL을 위한 변수 선언 */
    EGLDisplay gDisplay;
    EGLSurface gSurface;
    EGLContext gContext;
    EGLConfig gConfig, *gConfigs;
    EGLint cfgCnt, cfgIdx, count = 0, currFB;

    /* eglChooseConfig() 함수에서 사용할 설정값 */
    static const EGLint attribute_list[] = {
                EGL_RED_SIZE, 8,
                EGL_GREEN_SIZE, 8,
                EGL_BLUE_SIZE, 8,
                EGL_ALPHA_SIZE, 0,
                EGL_NONE
    };

    /* eglCreateContext() 함수에서 사용할 설정값 */
    static const EGLint context_attributes[] = {
                EGL_CONTEXT_CLIENT_VERSION, 1, /* OpenGL ES 1.x: 1, OpenGL ES 2: 2 */
                EGL_NONE
    };

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
    gContext = eglCreateContext(gDisplay, gConfigs[cfgIdx], EGL_NO_CONTEXT, context_attributes);
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

    /* 현재 윈도우에 OpenGL의 표시를 위한 영역 설정 */
    glViewport(0, 0, screenWidth, screenHeight);
    
    /* 출력될 도형을 위한 좌표 설정 */
    GLfloat points[] = {
        -0.5, -0.5, 0.0, 	/* 좌측 아래 */
        0.5, -0.5, 0.0, 	/* 우측 아래 */
        0.0, 0.5, 0.0, 		/* 중앙 위쪽 */
    };

    /* 출력될 도형을 위한 색상 설정 */
    glColor4f(1.0, 1.0, 1.0, 1.0);

    /* 정점 배열 사용을 설정 */
    glEnableClientState(GL_VERTEX_ARRAY);

    /* 그래픽 출력을 위한 정점 배열 사용에 대한 설정 */
    glVertexPointer(3, GL_FLOAT, 0, points);

    /* 삼각형을 그린다. 처리할 정점의 수: 3개 */
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 3);

    /* 그래픽 출력을 하고 나서 정점 배열 사용에 대한 설정 해제 */
    glDisableClientState(GL_VERTEX_ARRAY);

    eglSwapBuffers(gDisplay, gSurface); /* OpenGL ES의 내용을 EGL로 출력 */

    struct gbm_bo *currBo = gbm_surface_lock_front_buffer(gbmSurface);
    uint32_t handle = gbm_bo_get_handle(currBo).u32;
    uint32_t pitch = gbm_bo_get_stride(currBo);

    drmModeAddFB(device, modeInfo.hdisplay, modeInfo.vdisplay, 24, 32, pitch, handle, &currFB);
    drmModeSetCrtc(device, crtc->crtc_id, currFB, 0, 0, &connector_id, 1, &modeInfo);

    getchar();

    /* DRM Crtc 원래 모드로 돌리기 */
    drmModeSetCrtc(device, crtc->crtc_id, crtc->buffer_id, crtc->x, crtc->y, &connector_id, 1, &crtc->mode);
    drmModeFreeCrtc(crtc);

    /* 사용한 버퍼 해제 */
    if(currBo) {
        drmModeRmFB(device, currFB);
        gbm_surface_release_buffer(gbmSurface, currBo);
    }
   
    /* GBM 장치 정리 */
    gbm_surface_destroy(gbmSurface);
    gbm_device_destroy(gbmDev);
    
    /* DRM 장치 정리 */
    drmModeFreeConnector(connector);
    drmModeFreeResources(resources);
    
    close(device);
    
    return 0;
}

