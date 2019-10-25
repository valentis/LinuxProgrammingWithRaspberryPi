#include <stdio.h>
#include <arm_neon.h>                    /* ARM NEON 프로그래밍을 위한 헤더 파일 */

void printData(uint8x16_t data) 
{
    printf("%02d ", vgetq_lane_u8(data, 0));
    printf("%02d ", vgetq_lane_u8(data, 1));
    printf("%02d ", vgetq_lane_u8(data, 2));
    printf("%02d ", vgetq_lane_u8(data, 3));
    printf("%02d ", vgetq_lane_u8(data, 4));
    printf("%02d ", vgetq_lane_u8(data, 5));
    printf("%02d ", vgetq_lane_u8(data, 6));
    printf("%02d ", vgetq_lane_u8(data, 7));
    printf("%02d ", vgetq_lane_u8(data, 8));
    printf("%02d ", vgetq_lane_u8(data, 9));
    printf("%02d ", vgetq_lane_u8(data, 10));
    printf("%02d ", vgetq_lane_u8(data, 11));
    printf("%02d ", vgetq_lane_u8(data, 12));
    printf("%02d ", vgetq_lane_u8(data, 13));
    printf("%02d ", vgetq_lane_u8(data, 14));
    printf("%02d ", vgetq_lane_u8(data, 15));
    printf("\n");
}

int main(int argc, char **argv) 
{
    /* 초기화를 위한 데이터셋 */
    const uint8_t source[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    uint8x16_t data, ret;                  /* 계산된 결과를 위한 벡터(데이터) 변수 */
    uint8x16_t fill5 = vmovq_n_u8(5);      /* 모두 5로 채워진 16비트 벡터를 생성한다. */

    data = vld1q_u8(source);               /* 데이터셋을 벡터에 대입한다. */

    printf("data   : ");
    printData(data);                       /* 벡터에 저장된 값을 출력한다. */
   
    ret = vaddq_u8(data, fill5);           /* 두 벡터의 요소들을 각각 더한다. */

    printf("result : ");
    printData(ret);                         /* 벡터에 저장된 값을 출력한다. */
    
    return 0;
}

