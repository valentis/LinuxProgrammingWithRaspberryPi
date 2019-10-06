#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <termios.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <linux/input.h>

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

/* 스레드에서 사용하는 뮤텍스 */
static pthread_mutex_t pressure_lock;
static pthread_mutex_t temperature_lock;

static int is_run = 1; 				/* 스레드 종료를 위한 플래그 */
int pressure_fd, temperature_fd;

int kbhit(void);

/* 스레드 처리를 위한 함수 */
void* webserverFunction(void* arg);
static void *clnt_connection(void *arg);
int sendData(FILE* fp, char *ct, char *filename);
void sendOk(FILE* fp);
void sendError(FILE* fp);

void getPressure(int fd, double *temperature, double *pressure); /* 기압과 온도를 위한 함수 */
void getTemperature(int fd, double *temperature, double *humidity); /* 온도와 습도를 위한 함수 */

void *pressureFunction(void *arg)
{
    double t_c = 0.0; /* 온도와 압력을 출력하기 위한 변수 */
    double pressure = 0.0;
    while(is_run) {
        if(pthread_mutex_trylock(&pressure_lock) != EBUSY) { 	/* 임계 구역 설정 */
            /* LPS25H 장치 초기화 */
            wiringPiI2CWriteReg8(pressure_fd, CTRL_REG1, 0x00);
            wiringPiI2CWriteReg8(pressure_fd, CTRL_REG1, 0x84);
            wiringPiI2CWriteReg8(pressure_fd, CTRL_REG2, 0x01);

            getPressure(pressure_fd, &t_c, &pressure);
            printf("Temperature(from LPS25H) = %.2f°C\n", t_c);
            printf("Pressure = %.0f hPa\n", pressure);

            pthread_mutex_unlock(&pressure_lock); 		/* 임계 구역 해제 */
        }
        delay(1000);
    }
    return NULL;
}

void *temperatureFunction(void *arg)
{
    double temperature, humidity;
    while(is_run) {
        if(pthread_mutex_trylock(&temperature_lock) != EBUSY) { /* 임계 구역 설정 */
	    /* HTS221 장치 초기화 */
	    wiringPiI2CWriteReg8(temperature_fd, CTRL_REG1, 0x00);
	    wiringPiI2CWriteReg8(temperature_fd, CTRL_REG1, 0x84);
	    wiringPiI2CWriteReg8(temperature_fd, CTRL_REG2, 0x01);

            getTemperature(temperature_fd, &temperature, &humidity);
            printf("Temperature(from HTS221) = %.2f°C\n", temperature);
            printf("Humidity = %.0f%% rH\n", humidity);
            pthread_mutex_unlock(&temperature_lock); 		/* 임계 구역 해제 */
        }
        delay(1000);
    }
    return NULL;
}

void* joystickFunction(void* arg)
{
    int fd;
    struct input_event ie;
    char* joy_dev = "/dev/input/event10";
    if((fd = open(joy_dev, O_RDONLY)) == -1) {
        perror("opening device");
        return NULL;
    }
    while(is_run) {
        read(fd, &ie, sizeof(struct input_event));
        printf("time %ld.%06ld\ttype %d\tcode %-3d\tvalue %d\n",
                ie.time.tv_sec, ie.time.tv_usec, ie.type, ie.code,
                ie.value);
        if(ie.type) {
            switch(ie.code) {
                case KEY_UP: printf("Up\n"); break;		/* 'Up' 방향키: 조이스틱 'Up' */
                case KEY_DOWN: printf("Down\n"); break;		/* 'Down' 방향키: 조이스틱 'Down' */
                case KEY_LEFT: printf("Left\n"); break;		/* 'Left' 방향키: 조이스틱 'Left' */
                case KEY_RIGHT: printf("Right\n"); break;	/* 'Right' 방향키: 조이스틱 'Right' */
                case KEY_ENTER: 				/* ‘Enter’ 키: 조이스틱 ‘Press’ */
		    printf("Enter\n"); is_run = 0; break; 	
                default: printf("Default\n"); break; 		/* 위의 키가 아닌 경우 */
            }
        }
        delay(100);
    }

    return NULL;
}

int main(int argc, char **argv)
{
    int i = 0;
    pthread_t ptPressure, ptTemperature, ptJoystick, ptWebserver;
    pthread_mutex_init(&pressure_lock, NULL); 		/* 뮤텍스 초기화 */
    pthread_mutex_init(&temperature_lock, NULL);

    /* 프로그램을 시작할 때 서버를 위한 포트 번호를 입력받는다. */
    if(argc!=2) {
        printf("usage: %s <port>\n", argv[0]);
        return -1;
    }

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

//    pthread_create(&ptPressure, NULL, pressureFunction, NULL);
//    pthread_create(&ptTemperature, NULL, temperatureFunction, NULL);
    pthread_create(&ptJoystick, NULL, joystickFunction, NULL);
    pthread_create(&ptWebserver, NULL, webserverFunction, (void*)(atoi(argv[1])));

    printf("q : Quit\n");
    for(i = 0; is_run;i++) {
        if(kbhit()) { 			/* 키보드가 눌렸는지 확인한다. */
            switch(getchar()) { 	/* 문자를 읽는다. */
	        case 'q': 		/* 읽은 문자가 q이면 종료한다. */
		    pthread_kill(ptWebserver, SIGINT);
		    pthread_cancel(ptWebserver);
		    is_run = 0;
		    goto END;
            };
        }

    //    printf("%20d\t\t\r", i); 	/* 현재 카운트한 숫자를 출력한다. */
        delay(100); 			/* 100밀리초 동안 쉰다. */
    }

END:
    printf("Good Bye!\n");

    /* 사용이 끝난 장치를 정리 */
    wiringPiI2CWriteReg8(pressure_fd, CTRL_REG1, 0x00);
    close(pressure_fd);
    wiringPiI2CWriteReg8(temperature_fd, CTRL_REG1, 0x00);
    close(temperature_fd);

    pthread_mutex_destroy(&pressure_lock); 	/* 뮤텍스 삭제 */
    pthread_mutex_destroy(&temperature_lock);

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

void* webserverFunction(void* arg)
{
    int ssock;
    pthread_t thread;
    struct sockaddr_in servaddr, cliaddr;
    unsigned int len;
    int port = (int)(arg);

    /* 서버를 위한 소켓을 생성한다. */
    ssock = socket(AF_INET, SOCK_STREAM, 0);
    if(ssock == -1) {
        perror("socket()");
        exit(1);
    }

    /* 입력받는 포트 번호를 이용해서 서비스를 운영체제에 등록한다. */
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);
    if(bind(ssock, (struct sockaddr *)&servaddr, sizeof(servaddr))==-1) {
        perror("bind()");
        exit(1);
    }
    
    /* 최대 10대의 클라이언트의 동시 접속을 처리할 수 있도록 큐를 생성한다. */
    if(listen(ssock, 10) == -1) {
        perror("listen()");
        exit(1);
    }

    while(is_run) {
        char mesg[BUFSIZ];
        int csock;

        /* 클라이언트의 요청을 기다린다. */
        len = sizeof(cliaddr);
        csock = accept(ssock, (struct sockaddr*)&cliaddr, &len);

        /* 네트워크 주소를 문자열로 변경 */
        inet_ntop(AF_INET, &cliaddr.sin_addr, mesg, BUFSIZ);
        printf("Client IP : %s:%d\n", mesg, ntohs(cliaddr.sin_port));

        /* 클라이언트의 요청이 들어오면 스레드를 생성하고 클라이언트의 요청을 처리한다. */
        pthread_create(&thread, NULL, clnt_connection, &csock);
    //    pthread_join(thread, NULL);
    }

    return 0;
}
    
void *clnt_connection(void *arg)
{
    /* 스레드를 통해서 넘어온 arg를 int 형의 파일 디스크립터로 변환한다. */
    int csock = *((int*)arg);
    FILE *clnt_read, *clnt_write;
    char reg_line[BUFSIZ], reg_buf[BUFSIZ];
    char method[BUFSIZ], type[BUFSIZ];
    char filename[BUFSIZ], *ret;

    /* 파일 디스크립터를 FILE 스트림으로 변환한다. */
    clnt_read = fdopen(csock, "r");
    clnt_write = fdopen(dup(csock), "w");

    /* 한 줄의 문자열을 읽어서 reg_line 변수에 저장한다. */
    fgets(reg_line, BUFSIZ, clnt_read);
    
    /* reg_line 변수에 문자열을 화면에 출력한다. */
    fputs(reg_line, stdout);

    /* ' ' 문자로 reg_line을 구분해서 요청 라인의 내용(메소드)를 분리한다. */
    ret = strtok(reg_line, "/ ");
    strcpy(method, (ret != NULL)?ret:"");
    if(strcmp(method, "POST") == 0) { 		/* POST 메소드일 경우를 처리한다. */
        sendOk(clnt_write); 			/* 단순히 OK 메시지를 클라이언트로 보낸다. */
        goto END;
    } else if(strcmp(method, "GET") != 0) {	/* GET 메소드가 아닐 경우를 처리한다. */
        sendError(clnt_write); 			/* 에러 메시지를 클라이언트로 보낸다. */
        goto END;
    }

    ret = strtok(NULL, " "); 			/* 요청 라인에서 경로(path)를 가져온다. */
    strcpy(filename, (ret != NULL)?ret:"");
    if(filename[0] == '/') { 			/* 경로가 '/'로 시작될 경우 /를 제거한다. */
        for(int i = 0, j = 0; i < BUFSIZ; i++, j++) {
            if(filename[0] == '/') j++;
            filename[i] = filename[j];
            if(filename[j] == '\0') break;
        }
    }

    /* URL에 경로(path)이 없을 때 기본 처리 */
    if(!strlen(filename)) strcpy(filename, "index.html");

    /* 라즈베리 파이를 제어하기 위한 HTML 코드를 분석해서 처리한다. */
    if(strstr(filename, "?") != NULL) {
        char optLine[BUFSIZ];
        char optStr[32][BUFSIZ];
        char opt[BUFSIZ], var[BUFSIZ], *tok;
        int count = 0;

        ret = strtok(filename, "?");
        if(ret == NULL) goto END;
        strcpy(filename, ret);
        ret = strtok(NULL, "?");
        if(ret == NULL) goto END;
        strcpy(optLine, ret);

        /* 옵션을 분석한다. */
        tok = strtok(optLine, "&");
        while(tok != NULL) {
            strcpy(optStr[count++], tok);
            tok = strtok(NULL, "&");
        }
 
        /* 분석한 옵션을 처리한다. */
        for(int i = 0; i < count; i++) {
            strcpy(opt, strtok(optStr[i], "="));
            strcpy(var, strtok(NULL, "="));
            printf("%s = %s\n", opt, var);
            if(!strcmp(opt, "led") && !strcmp(var, "On")) { 		/* 8×8 LED 매트릭스를 켬 */
            } else if(!strcmp(opt, "led") && !strcmp(var, "Off")) { 	/* 8×8 LED 매트릭스를 끔 */
            }
        }
    }

    /* 메시지 헤더를 읽어서 화면에 출력하고 나머지는 무시한다. */
    do {
        fgets(reg_line, BUFSIZ, clnt_read);
        fputs(reg_line, stdout);
        strcpy(reg_buf, reg_line);
        char* buf = strchr(reg_buf, ':');
    } while(strncmp(reg_line, "\r\n", 2)); 	/* 요청 헤더는 ‘\r\n’으로 끝난다. */

    /* 파일의 이름을 이용해서 클라이언트로 파일의 내용을 보낸다. */
    sendData(clnt_write, type, filename);

END:
    fclose(clnt_read); 				/* 파일의 스트림을 닫는다. */
    fclose(clnt_write);
    pthread_exit(0); 				/* 스레드를 종료시킨다. */

    return (void*)NULL;
}
    
int sendData(FILE* fp, char *ct, char *filename)
{
    /* 클라이언트로 보낼 성공에 대한 응답 메시지 */
    char protocol[ ] = "HTTP/1.1 200 OK\r\n";
    char server[ ] = "Server:Netscape-Enterprise/6.0\r\n";
    char cnt_type[ ] = "Content-Type:text/html\r\n";
    char end[ ] = "\r\n"; 			/* 응답 헤더의 끝은 항상 \r\n */
    char html[BUFSIZ];
    double temperature, humidity;
    double t_c = 0.0; 				/* 온도와 압력을 출력하기 위한 변수 */
    double pressure = 0.0;

    /* LPS25H 장치 초기화 */
    wiringPiI2CWriteReg8(pressure_fd, CTRL_REG1, 0x00);
    wiringPiI2CWriteReg8(pressure_fd, CTRL_REG1, 0x84);
    wiringPiI2CWriteReg8(pressure_fd, CTRL_REG2, 0x01);
    getPressure(pressure_fd, &t_c, &pressure); 	/* 기압/온도를 위한 함수 */

    /* HTS221 장치 초기화 */
    wiringPiI2CWriteReg8(temperature_fd, CTRL_REG1, 0x00);
    wiringPiI2CWriteReg8(temperature_fd, CTRL_REG1, 0x84);
    wiringPiI2CWriteReg8(temperature_fd, CTRL_REG2, 0x01);
    getTemperature(temperature_fd, &temperature, &humidity); 	/* 온도/습도를 위한 함수 */

    sprintf(html, "<html><head><meta http-equiv=\"Content-Type\" " \
                  "content=\"text/html; charset=UTF-8\" />" \
                  "<title>Raspberry Pi Controller</title></head><body><table>" \
                  "<tr><td>Temperature</td><td colspan=2>" \
                  "<input readonly name=\"temperature\"value=%.3f></td></tr>" \
                  "<tr><td>Humidity</td><td colspan=2>" \
                  "<input readonly name=\"humidity\"value=%.3f></td></tr>" \
                  "<tr><td>Pressure</td><td colspan=2>" \
                  "<input readonly name=\"pressure\"value=%.3f></td></tr></table>" \
                  "<form action=\"index.html\" method=\"GET\" "\
                  "onSubmit=\"document.reload()\"><table>" \
                  "<tr><td>8x8 LED Matrix</td><td>" \
                  "<input type=radio name=\"led\" value=\"On\" checked=checked>On</td>" \
                  "<td><input type=radio name=\"led\" value=\"Off\">Off</td>" \
                  "</tr><tr><td>Submit</td>" \
                  "<td colspan=2><input type=submit value=\"Submit\"></td></tr>" \
                  "</table></form></body></html>",
                  temperature, humidity, pressure);

    fputs(protocol, fp);
    fputs(server, fp);
    fputs(cnt_type, fp);
    fputs(end, fp);
    fputs(html, fp);
    fflush(fp);

    return 0;
}
    
void sendOk(FILE* fp)
{
    /* 클라이언트에 보낼 성공에 대한 HTTP 응답 메시지 */
    char protocol[ ] = "HTTP/1.1 200 OK\r\n";
    char server[ ] = "Server: Netscape-Enterprise/6.0\r\n\r\n";

    fputs(protocol, fp);
    fputs(server, fp);
    fflush(fp);
}
    
void sendError(FILE* fp)
{
    /* 클라이언트로 보낼 실패에 대한 HTTP 응답 메시지 */
    char protocol[ ] = "HTTP/1.1 400 Bad Request\r\n";
    char server[ ] = "Server: Netscape-Enterprise/6.0\r\n";
    char cnt_len[ ] = "Content-Length:1024\r\n";
    char cnt_type[ ] = "Content-Type:text/html\r\n\r\n";

    /* 화면에 표시될 HTML의 내용 */
    char content1[ ] = "<html><head><title>BAD Connection</title></head>";
    char content2[ ] = "<body><font size=+5>Bad Request</font></body></html>";
    printf("send_error\n");

    fputs(protocol, fp);
    fputs(server, fp);
    fputs(cnt_len, fp);
    fputs(cnt_type, fp);
    fputs(content1, fp);
    fputs(content2, fp);
    fflush(fp);
}
