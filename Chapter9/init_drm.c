#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <gbm.h>
#include <xf86drmMode.h>

int main(int argc, char **argv)
{
    int device = open("/dev/dri/card1", O_RDWR);             /* DRI 장치 열기 */

    /* DRM(Direct Rendering Manager) 초기화 */
    drmModeRes *resources = drmModeGetResources(device);
    drmModeConnector *connector = drmModeGetConnector(device, 
    resources->connectors[0]);
    drmModeModeInfo modeInfo = connector->modes[0];

    /* GDM(Generic Buffer Manager) 초기화 */
    struct gbm_device *gbmDev = gbm_create_device(device);
    struct gbm_surface *gbmSurface = gbm_surface_create(gbmDev, 
                                      modeInfo.hdisplay, modeInfo.vdisplay, 
                                      GBM_FORMAT_XRGB8888, 
                                      GBM_BO_USE_SCANOUT | GBM_BO_USE_RENDERING);

    getchar( );          /* EGL과 OpenGL 함수 수행 */
    
    /* GBM 장치 정리 */
    gbm_surface_destroy(gbmSurface);
    gbm_device_destroy(gbmDev);
    
    /* DRM 장치 정리 */
    drmModeFreeConnector(connector);
    drmModeFreeResources(resources);
    
    close(device);
    
    return 0;
}

