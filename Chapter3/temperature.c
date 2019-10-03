#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <sys/ioctl.h>

static const char* I2C_DEV = "/dev/i2c-1"; 	/* I2C를 위한 장치 파일 */
static const int I2C_SLAVE = 0x0703; 		/* ioctl( ) 함수에서 I2C_SLAVE 설정을 위한 값 */

static const int HTS221_ID = 0x5F; 		/* SenseHAT의 I2C-1의 값 */

static const int CTRL_REG1 = 0x20; 		/* STMicroelectronics의 HTS221 스펙 문서의 값 */
static const int CTRL_REG2 = 0x21;

static const int H0_T0_OUT_L = 0x36; 		/* STMicroelectronics의 HTS221 스펙 문서의 값 */
static const int H0_T0_OUT_H = 0x37;

static const int H1_T0_OUT_L = 0x3A;
static const int H1_T0_OUT_H = 0x3B;
static const int H0_rH_x2 = 0x30;
static const int H1_rH_x2 = 0x31;

static const int H_T_OUT_L = 0x28;
static const int H_T_OUT_H = 0x29;

static const int T0_OUT_L = 0x3C;
static const int T0_OUT_H = 0x3D;
static const int T1_OUT_L = 0x3E;
static const int T1_OUT_H = 0x3F;

static const int T0_degC_x8 = 0x32;
static const int T1_degC_x8 = 0x33;

static const int T1_T0_MSB = 0x35;

static const int TEMP_OUT_L = 0x2A;
static const int TEMP_OUT_H = 0x2B;

/* 온도와 습도를 위한 함수 */
void getTemperature(int fd, double *temperature, double *humidity);

int main( )
{
    int i2c_fd;
    double temperature, humidity;

    /* I2C 장치 파일을 오픈 */
    if((i2c_fd = open(I2C_DEV, O_RDWR)) < 0) {
        perror("Unable to open i2c device");
        return 1;
    }

    /* I2C 장치를 슬레이브(slave) 모드로 HTS221를 설정 */
    if(ioctl(i2c_fd, I2C_SLAVE, HTS221_ID) < 0) {
        perror("Unable to configure i2c slave device");
        close(i2c_fd);
        return 1;
    }

    for(int i = 0; i < 10; i++) {
        /* HTS221 장치 초기화 */
        wiringPiI2CWriteReg8(i2c_fd, CTRL_REG1, 0x00);
        wiringPiI2CWriteReg8(i2c_fd, CTRL_REG1, 0x84);
        wiringPiI2CWriteReg8(i2c_fd, CTRL_REG2, 0x01);

        /* 온도와 습도값 획득 */
        getTemperature(i2c_fd, &temperature, &humidity);

        /* 계산된 값을 출력 */
        printf("Temperature(from HTS221) = %.2f°C\n", temperature);
        printf("Humidity = %.0f%% rH\n", humidity);

	delay(1000); 			/* 다음 측정하기 전에 1초(1000밀리초)간 대기 */
    }

    /* 사용이 끝난 장치를 정리 */
    wiringPiI2CWriteReg8(i2c_fd, CTRL_REG1, 0x00);

    close(i2c_fd);

    return 0;
}

/* 온도와 습도를 가져오기 위한 함수 */
void getTemperature(int fd, double *temperature, double *humidity)
{
    int result;

    /* 측정이 완료될 때까지 대기 */
    do {
        delay(25); /* 25밀리초 대기 */
        result = wiringPiI2CReadReg8(fd, CTRL_REG2);
    } while(result != 0);

    /* 온도(LSB (ADC))를 위한 보정 값(x-데이터를 위한 2지점) 읽기 */
    unsigned char t0_out_l = wiringPiI2CReadReg8(fd, T0_OUT_L);
    unsigned char t0_out_h = wiringPiI2CReadReg8(fd, T0_OUT_H);
    unsigned char t1_out_l = wiringPiI2CReadReg8(fd, T1_OUT_L);
    unsigned char t1_out_h = wiringPiI2CReadReg8(fd, T1_OUT_H);

    /* 온도(°C)를 위한 보정 값(y-데이터를 위한 2지점) 읽기 */
    unsigned char t0_degC_x8 = wiringPiI2CReadReg8(fd, T0_degC_x8);
    unsigned char t1_degC_x8 = wiringPiI2CReadReg8(fd, T1_degC_x8);
    unsigned char t1_t0_msb = wiringPiI2CReadReg8(fd, T1_T0_MSB);

    /* 습도(LSB(ADC))를 위한 보정 값(x-데이터를 위한 2지점) 읽기 */
    unsigned char h0_out_l = wiringPiI2CReadReg8(fd, H0_T0_OUT_L);
    unsigned char h0_out_h = wiringPiI2CReadReg8(fd, H0_T0_OUT_H);
    unsigned char h1_out_l = wiringPiI2CReadReg8(fd, H1_T0_OUT_L);
    unsigned char h1_out_h = wiringPiI2CReadReg8(fd, H1_T0_OUT_H);

    /* 습도(% rH)를 위한 보정 값(y-데이터를 위한 2지점) 읽기 */
    unsigned char h0_rh_x2 = wiringPiI2CReadReg8(fd, H0_rH_x2);
    unsigned char h1_rh_x2 = wiringPiI2CReadReg8(fd, H1_rH_x2);

    /* 각각 측정한 값들을 합성해서 온도(x-값) 값 생성(비트/시프트 이용) */
    short s_t0_out = t0_out_h << 8 | t0_out_l;
    short s_t1_out = t1_out_h << 8 | t1_out_l;

    /* 각각 측정한 값들을 합성해서 습도(x-값) 값 생성(비트/시프트 이용) */
    short s_h0_t0_out = h0_out_h << 8 | h0_out_l;
    short s_h1_t0_out = h1_out_h << 8 | h1_out_l;

    /* 16비트와 10비트의 값 생성(비트 마스크/시프트 이용) */
    unsigned short s_t0_degC_x8 = (t1_t0_msb & 3) << 8 | t0_degC_x8;
    unsigned short s_t1_degC_x8 = ((t1_t0_msb & 12) >> 2) << 8 | t1_degC_x8;

    /* 온도 보정 값(y-값) 계산*/
    double d_t0_degC = s_t0_degC_x8 / 8.0;
    double d_t1_degC = s_t1_degC_x8 / 8.0;

    /* 습도 보정 값(y-값) 계산*/
    double h0_rH = h0_rh_x2 / 2.0;
    double h1_rH = h1_rh_x2 / 2.0;

    /* 온도와 습도의 계산을 위한 보정 선형 직선 그래프 'y = mx + c' 공식을 계산 */
    double t_gradient_m = (d_t1_degC - d_t0_degC) / (s_t1_out - s_t0_out);
    double t_intercept_c = d_t1_degC - (t_gradient_m * s_t1_out);
    double h_gradient_m = (h1_rH - h0_rH) / (s_h1_t0_out - s_h0_t0_out);
    double h_intercept_c = h1_rH - (h_gradient_m * s_h1_t0_out);

    /* 주변의 온도 읽기(2바이트 읽기) */
    unsigned char t_out_l = wiringPiI2CReadReg8(fd, TEMP_OUT_L);
    unsigned char t_out_h = wiringPiI2CReadReg8(fd, TEMP_OUT_H);

    /* 16비트 값 생성 */
    short s_t_out = t_out_h << 8 | t_out_l;

    /* 주변의 습도 읽기(2바이트 읽기) */
    unsigned char h_t_out_l = wiringPiI2CReadReg8(fd, H_T_OUT_L);
    unsigned char h_t_out_h = wiringPiI2CReadReg8(fd, H_T_OUT_H);

    /* 16비트 값 생성 */
    short s_h_t_out = h_t_out_h << 8 | h_t_out_l;

    /* 주변의 온도 계산 */
    *temperature = (t_gradient_m * s_t_out) + t_intercept_c;

    /* 주변의 습도 계산 */
    *humidity = (h_gradient_m * s_h_t_out) + h_intercept_c;
}

