#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <mqueue.h>

int main(int argc, char** argv)
{
    mqd_t mq;
    const char* name = "/posix_msq";
    char buf[BUFSIZ];
    mq = mq_open(name, O_WRONLY);

    /* "Hello, World!"라는 문자열을 보낸다. */
    strcpy(buf, "Hello, World!\n");
    mq_send(mq, buf, strlen(buf), 0); 	/* 메시지 큐로 데이터를 보낸다. */

    /* "q"로 설정하고 보낸다. */
    strcpy(buf, "q");
    mq_send(mq, buf, strlen(buf), 0);

    /* 메시지 큐를 닫는다. */
    mq_close(mq);

    return 0;
}
