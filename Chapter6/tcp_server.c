#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define TCP_PORT 5100 				/* 서버의 포트 번호 */

int main(int argc, char** argv)
{
    int ssock; 					/* 소켓 디스크립트 정의 */
    socklen_t clen;
    int n;
    struct sockaddr_in servaddr, cliaddr; 	/* 주소 구조체 정의 */
    char mesg[BUFSIZ];

    /* 서버 소켓 생성 */
    if((ssock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket()");
        return -1;
    }

    /* 주소 구조체에 주소 지정 */
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(TCP_PORT); 	/* 사용할 포트 지정 */

    /* bind 함수를 사용하여 서버 소켓의 주소 설정 */
    if(bind(ssock, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("bind()");
        return -1;
    }

    /* 동시에 접속하는 클라이언트의 처리를 위한 대기 큐를 설정 */
    if(listen(ssock, 8) < 0) {
        perror("listen()");
        return -1;
    }

    clen = sizeof(cliaddr);
    do {
        /* 클라이언트가 접속하면 접속을 허용하고 클라이언트 소켓 생성 */
        int n, csock = accept(ssock, (struct sockaddr *)&cliaddr, &clen);

        /* 네트워크 주소를 문자열로 변경 */
        inet_ntop(AF_INET, &cliaddr.sin_addr, mesg, BUFSIZ);
        printf("Client is connected : %s\n", mesg);
        if((n = read(csock, mesg, BUFSIZ)) <= 0)
            perror("read()");
        printf("Received data : %s", mesg);

        /* 클라이언트로 buf에 있는 문자열 전송 */
        if(write(csock, mesg, n) <= 0)
            perror("write()");
        close(csock); 			/* 클라이언트 소켓을 닫음 */
    } while(strncmp(mesg, "q", 1));

    close(ssock); 			/* 서버 소켓을 닫음 */

    return 0;
}
