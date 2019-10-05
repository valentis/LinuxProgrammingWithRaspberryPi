#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/fb.h>
#include <sys/ioctl.h>

#define FBDEVICE "/dev/fb0"

int main(int argc, char** argv)
{
    int fbfd = 0;

    /* 프레임 버퍼 정보 처리를 위한 구조체 */
    struct fb_var_screeninfo vinfo, old_vinfo;
    struct fb_fix_screeninfo finfo;
    
    /* 프레임 버퍼를 위한 디바이스 파일을 읽기와 쓰기 모드로 연다. */
    fbfd = open(FBDEVICE, O_RDWR);
    if(fbfd < 0) {
        perror("Error: cannot open framebuffer device");
        return -1;
    }
    
    /* 현재 프레임 버퍼에 대한 화면 정보를 얻어온다. */
    if(ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo) < 0) {
        perror("Error reading fixed information");
        return -1;
    }
    
    /* 현재 프레임 버퍼에 대한 가상 화면 정보를 얻어온다. */
    if(ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) < 0) {
        perror("Error reading variable information");
        return -1;
    }
    
    /* 현재 프레임 버퍼에 대한 정보를 출력한다. */
    printf("Resolution : %dx%d, %dbpp\n", vinfo.xres, vinfo.yres,
    vinfo.bits_per_pixel);
    printf("Virtual Resolution : %dx%d\n", vinfo.xres_virtual, vinfo.yres_virtual);
    printf("Length of frame buffer memory : %d\n", finfo.smem_len);
    
    /* 이전의 값을 백업해 놓는다. */
    old_vinfo = vinfo;
    
    /* 프레임 버퍼에 새로운 해상도(800 × 600)를 설정한다. */
    vinfo.xres = 800;
    vinfo.yres = 600;
    //vinfo.bits_per_pixel = 16;
    
    if(ioctl(fbfd, FBIOPUT_VSCREENINFO, &vinfo) < 0) {
        perror("fbdev ioctl(PUT)");
        return -1;
    }
    
    /* 설정한 프레임 버퍼에 대한 정보를 출력한다. */
    printf("New Resolution : %dx%d, %dbpp\n", vinfo.xres, vinfo.yres, vinfo.bits_per_pixel);
    getchar(); /* 사용자의 입력을 대시 */
    
    ioctl(fbfd, FBIOPUT_VSCREENINFO, &old_vinfo); /* 원래의 값으로 복귀 */
    
    close(fbfd); /* 사용이 끝난 프레임 버퍼의 디바이스 파일 닫기 */
    
    return 0;
}
