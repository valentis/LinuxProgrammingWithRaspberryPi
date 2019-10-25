#include <stdio.h>
#include <arm_neon.h>                    /* ARM NEON 프로그래밍을 위한 헤더 파일 */

void printData(uint8x16_t data) 
{
    int i;
    static uint8_t p[16];

    vst1q_u8(p, data);                      /* 벡터를 배열에 대입한다. */

    for (i = 0; i < 16; i++) printf("%02d ", p[i]);
    printf ("\n");
}


int main(int argc, char **argv) 
{
    /* 초기화를 위한 데이터셋 */
    const uint8_t source[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
    uint8x16_t data, ret;                   /* 계산된 결과를 위한 벡터(데이터) 변수 */ 
    uint8x16_t fill3 = vmovq_n_u8(3);      /* 모두 5로 채워진 16비트 벡터를 생성한다. */
  
    data = vld1q_u8(source);              /* 데이터셋을 벡터에 대입한다. */

    printf("data    : ");
    printData(data);                       /* 벡터에 저장된 값을 출력한다. */
   
    ret = vaddq_u8(data, fill3);            /* 두 벡터의 요소들을 각각 더한다. */
    printf("Add    : ");   printData(ret);       /* 덧셈의 결과 값을 출력한다. */
    
    ret = vsubq_u8(data, fill3);            /* 두 벡터의 요소들을 각각 뺀다. */
    printf("Subtract : ");   printData(ret);       /* 뺄셈의 결과 값을 출력한다. */

    ret = vmulq_u8(data, fill3);            /* 두 벡터의 요소들을 각각 곱한다. */
    printf("Multiply : ");   printData(ret);       /* 곱셈의 결과 값을 출력한다. */

    return 0;
}



