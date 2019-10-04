#include <stdio.h>
#include <pthread.h>

int g_var = 1;
pthread_mutex_t mid;

void *inc_function(void *);
void *dec_function(void *);

int main(int argc, char** argv)
{
    pthread_t ptInc, ptDec;

    pthread_mutex_init(&mid, NULL); 	/* 뮤텍스 초기화 */

    pthread_create(&ptInc, NULL, inc_function, NULL);

    pthread_create(&ptDec, NULL, dec_function, NULL);
    pthread_join(ptInc, NULL);
    pthread_join(ptDec, NULL);

    pthread_mutex_destroy(&mid); 	/* 뮤텍스 삭제 */

    return 0;
}

void *inc_function(void *arg)
{
    pthread_mutex_lock(&mid); 		/* 임계 구역 설정 */
    printf("Inc : %d < Before\n", g_var);
    g_var++;
    printf("Inc : %d > After\n", g_var);
    pthread_mutex_unlock(&mid); 	/* 임계 구역 해제 */

    return NULL;
}

void *dec_function(void *arg)
{
    pthread_mutex_lock(&mid); 		/* 임계 구역 설정 */
    printf("Dec : %d < Before\n", g_var);
    g_var--;
    printf("Dec : %d > After\n", g_var);
    pthread_mutex_unlock(&mid); 	/* 임계 구역 해제 */

    return NULL;
}
