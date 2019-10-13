#include <stdio.h>
#include <unistd.h>
#include <bcm_host.h> 		/* BCM GPU 사용을 위한 헤더 파일 */

int main(int argc, char **argv)
{
    bcm_host_init(); 		/* BCM GPU를 위한 초기화 수행 */

    getchar();

    bcm_host_deinit(); 		/* BCM GPU를 위한 정리 작업 수행 */

    return 0;
}
