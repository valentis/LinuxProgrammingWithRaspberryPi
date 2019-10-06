#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <sys/ioctl.h>

static const char* I2C_DEV = "/dev/i2c-1"; 	/* I2C를 위한 장치 파일 */
static const int I2C_SLAVE = 0x0703; 		/* ioctl() 함수에서 I2C_SLAVE 설정을 위한 값 */

static const int LPS25H_ID = 0x5C; 		/* SenseHAT의 I2C-1의 값 */
static const int HTS221_ID = 0x5F;

static const int CTRL_REG1 = 0x20; 		/* STMicroelectronics의 스펙 문서의 값 */
static const int CTRL_REG2 = 0x21;

static const int PRESS_OUT_XL = 0x28; 		/* STMicroelectronics의 LPS25H 스펙 문서의 값 */
static const int PRESS_OUT_L = 0x29;
static const int PRESS_OUT_H = 0x2A;
static const int PTEMP_OUT_L = 0x2B;
static const int PTEMP_OUT_H = 0x2C;

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

int kbhit(void);

/* 기압과 온도를 위한 함수 */
void getPressure(int fd, double *temperature, double *pressure);

/* 온도와 습도를 위한 함수 */
void getTemperature(int fd, double *temperature, double *humidity);

int main(int argc, char **argv)
{
    int i = 0;
    int pressure_fd, temperature_fd;
    double t_c = 0.0; 			/* 온도와 압력, 습도를 출력하기 위한 변수 */
    double pressure = 0.0;
    double temperature, humidity;

    /* I2C 장치 파일을 오픈 */
    if((pressure_fd = open(I2C_DEV, O_RDWR)) < 0) {
        perror("Unable to open i2c device");
        return 1;
    }

    /* I2C 장치를 슬레이브(slave) 모드로 LPS25H를 설정 */
    if(ioctl(pressure_fd, I2C_SLAVE, LPS25H_ID) < 0) {
        perror("Unable to configure i2c slave device");
        close(pressure_fd);
        return 1;
    }

    /* I2C 장치 파일을 오픈 */
    if((temperature_fd = open(I2C_DEV, O_RDWR)) < 0) {
        perror("Unable to open i2c device");
        return 1;
    }

    /* I2C 장치를 슬레이브(slave) 모드로 HTS221를 설정 */
    if(ioctl(temperature_fd, I2C_SLAVE, HTS221_ID) < 0) {
        perror("Unable to configure i2c slave device");
        close(temperature_fd);
        return 1;
    }

    printf("p : Pressure, t : Temperature, q : Quit\n");
    for(i = 0; ;i++) {
        if(kbhit()) { /* 키보드가 눌렸는지 확인한다. */
            switch(getchar()) { /* 문자를 읽는다. */
                case 'p': /* 읽은 문자가 p이면 기압을 출력한다. */
                     /* LPS25H 장치 초기화 */
                     wiringPiI2CWriteReg8(pressure_fd, CTRL_REG1, 0x00);
                     wiringPiI2CWriteReg8(pressure_fd, CTRL_REG1, 0x84);
                     wiringPiI2CWriteReg8(pressure_fd, CTRL_REG2, 0x01);

                    /* 기압과 온도 값 얻기 */
                    getPressure(pressure_fd, &t_c, &pressure);
                    /* 계산된 값을 출력 */
                    printf("Temperature(from LPS25H) = %.2f°C\n", t_c);
                    printf("Pressure = %.0f hPa\n", pressure);
                    break;
                case 't': /* 읽은 문자가 t이면 온도를 출력한다. */
                    /* HTS221 장치 초기화 */
                    wiringPiI2CWriteReg8(temperature_fd, CTRL_REG1, 0x00);
                    wiringPiI2CWriteReg8(temperature_fd, CTRL_REG1, 0x84);
                    wiringPiI2CWriteReg8(temperature_fd, CTRL_REG2, 0x01);

                    /* 온도와 습도 값 얻기 */
                    getTemperature(temperature_fd, &temperature, &humidity);
                    /* 계산된 값을 출력 */
                    printf("Temperature(from HTS221) = %.2f°C\n", temperature);
                    printf("Humidity = %.0f%% rH\n", humidity);
                    break;
                case 'q': /* 읽은 문자가 q이면 종료한다. */
                    goto END;
                    break;
            };
        }

        printf("%20d\t\t\r", i); /* 현재 카운트한 숫자를 출력한다. */
        delay(100); /* 100밀리초 동안 쉰다. */
    }

END:
    printf("Good Bye!\n");

    /* 사용이 끝난 장치를 정리 */
    wiringPiI2CWriteReg8(pressure_fd, CTRL_REG1, 0x00);
    close(pressure_fd);
    wiringPiI2CWriteReg8(temperature_fd, CTRL_REG1, 0x00);
    close(temperature_fd);

    return 0;
}

/* 키보드 입력을 처리하기 위한 함수 */
int kbhit(void)
{
    struct termios oldt, newt; 		/* 터미널에 대한 구조체 */
    int ch, oldf;

    tcgetattr(0, &oldt); 		/* 현재 터미널에 설정된 정보를 가져온다. */
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO); 	/* 정규 모드 입력과 에코를 해제한다. */
    tcsetattr(0, TCSANOW, &newt); 	/* 새 값으로 터미널을 설정한다. */
    oldf = fcntl(0, F_GETFL, 0);
    fcntl(0, F_SETFL, oldf | O_NONBLOCK); 	/* 입력을 논블로킹 모드로 설정한다. */

    ch = getchar();

    tcsetattr(0, TCSANOW, &oldt); 	/* 기존의 값으로 터미널의 속성을 바로 적용한다. */
    fcntl(0, F_SETFL, oldf);
    if(ch != EOF) {
        ungetc(ch, stdin); 		/* 앞에서 읽은 위치로 이전으로 포인터를 돌려준다. */
        return 1;
    }

    return 0;
}

/* 기압과 온도 계산을 위한 함수 */
void getPressure(int fd, double *temperature, double *pressure)
{
    int result;
    unsigned char temp_out_l = 0, temp_out_h = 0; 	/* 온도를 계산하기 위한 변수 */
    unsigned char press_out_xl = 0; 	/* 기압을 계산하기 위한 변수 */
    unsigned char press_out_l = 0;
    unsigned char press_out_h = 0;
    short temp_out = 0; 		/* 온도와 압력을 저장하기 위한 변수 */
    int press_out = 0;

    /* 측정이 완료될 때까지 대기 */
    do {
        delay(25); 			/* 25밀리초 대기 */
        result = wiringPiI2CReadReg8(fd, CTRL_REG2);
    } while(result != 0);

    /* 측정된 온도 값 읽기(2바이트 읽기) */
    temp_out_l = wiringPiI2CReadReg8(fd, PTEMP_OUT_L);
    temp_out_h = wiringPiI2CReadReg8(fd, PTEMP_OUT_H);

    /* 측정된 기압 값 읽기(3바이트 읽기) */
    press_out_xl = wiringPiI2CReadReg8(fd, PRESS_OUT_XL);
    press_out_l = wiringPiI2CReadReg8(fd, PRESS_OUT_L);
    press_out_h = wiringPiI2CReadReg8(fd, PRESS_OUT_H);

    /* 각각 측정한 값들을 합성해서 온도(16비트)와 기압(24비트) 값 생성(비트/시프트 이용) */
    temp_out = temp_out_h << 8 | temp_out_l;
    press_out = press_out_h << 16 | press_out_l << 8 | press_out_xl;

    /* 출력값 계산 */
    *temperature = 42.5 + (temp_out / 480.0);
    *pressure = press_out / 4096.0;
}

/* 온도와 습도를 가져오기 위한 함수 */
void getTemperature(int fd, double *temperature, double *humidity)
{
    int result;

    /* 측정이 완료될 때까지 대기 */
    do {
        delay(25); 			/* 25밀리초 대기 */
        result = wiringPiI2CReadReg8(fd, CTRL_REG2);
    } while(result != 0);

    /* 온도(LSB(ADC))를 위한 보정값(x-데이터를 위한 2지점) 읽기 */
    unsigned char t0_out_l = wiringPiI2CReadReg8(fd, T0_OUT_L);
    unsigned char t0_out_h = wiringPiI2CReadReg8(fd, T0_OUT_H);
    unsigned char t1_out_l = wiringPiI2CReadReg8(fd, T1_OUT_L);
    unsigned char t1_out_h = wiringPiI2CReadReg8(fd, T1_OUT_H);

    /* 온도(°C)를 위한 보정값(y-데이터를 위한 2지점) 읽기 */
    unsigned char t0_degC_x8 = wiringPiI2CReadReg8(fd, T0_degC_x8);
    unsigned char t1_degC_x8 = wiringPiI2CReadReg8(fd, T1_degC_x8);
    unsigned char t1_t0_msb = wiringPiI2CReadReg8(fd, T1_T0_MSB);

    /* 습도(LSB(ADC))를 위한 보정값(x-데이터를 위한 2지점) 읽기 */
    unsigned char h0_out_l = wiringPiI2CReadReg8(fd, H0_T0_OUT_L);
    unsigned char h0_out_h = wiringPiI2CReadReg8(fd, H0_T0_OUT_H);
    unsigned char h1_out_l = wiringPiI2CReadReg8(fd, H1_T0_OUT_L);
    unsigned char h1_out_h = wiringPiI2CReadReg8(fd, H1_T0_OUT_H);

    /*습도(% rH)를 위한 보정값(y-데이터를 위한 2지점) 읽기 */
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

    /* 온도 보정값(y-값) 계산 */
    double d_t0_degC = s_t0_degC_x8 / 8.0;
    double d_t1_degC = s_t1_degC_x8 / 8.0;

    /* 습도 보정값(y-값) 계산 */
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
