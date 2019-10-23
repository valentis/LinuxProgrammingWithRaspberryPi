#ifndef __BMP_FILE_H__
#define __BMP_FILE_H__

typedef struct __attribute__((__packed__)) {
    unsigned short bfType; 		/* BM 표시: “BM” (2글자) 문자 */
    unsigned int bfSize; 		/* 파일의 크기: 4바이트 정수 */
    unsigned short bfReserved1; 	/* 추후의 확장을 위해 필드(reserved) */
    unsigned short bfReserved2; 	/* 추후의 확장을 위해 필드(reserved) */
    unsigned int bfOffBits; 		/* 실제 이미지까지의 오프셋: 바이트 */
} BITMAPFILEHEADER; 			/* BMP 파일 데이터를 위한 구조체 */

typedef struct {
    unsigned int biSize; 		/* 현 구조체의 크기: 4바이트 */
    unsigned int biWidth; 		/* 이미지의 폭(픽셀 단위): 4바이트 */
    unsigned int biHeight; 		/* 이미지의 높이(픽셀 단위): 4바이트 */
    unsigned short biPlanes; 		/* 비트 플레인 수(항상1): 2바이트 */
    unsigned short biBitCount; 		/* 픽셀당 비트 수: 2바이트 */
    unsigned int biCompression; 	/* 압축 유형: 4바이트 */
    unsigned int SizeImage; 		/* 이미지의 크기(압축 전 바이트 단위): 4바이트 */
    unsigned int biXPelsPerMeter; 	/* 가로 해상도: 4바이트 */
    unsigned int biYPelsPerMeter; 	/* 세로 해상도: 4바이트 */
    unsigned int biClrUsed; 		/* 실제 사용되는 색상 수: 4바이트 */
    unsigned int biClrImportant; 	/* 중요한 색상 인덱스(0인 경우 전체): 4바이트 */
} BITMAPINFOHEADER; 			/* BMP 이미지 데이터를 위한 구조체 */

typedef struct {
    unsigned char rgbBlue; 		/* 파란색을 위한 요소 */
    unsigned char rgbGreen; 		/* 초록색을 위한 요소 */
    unsigned char rgbRed; 		/* 빨간색을 위한 요소 */
    unsigned char rgbReserved; 		/* 투명도(알파값) 등을 위한 예약 공간 */
} RGBQUAD; 				/* 색상 팔레트를 위한 구조체 */

#endif /* __BMP_FILE_H__ */
