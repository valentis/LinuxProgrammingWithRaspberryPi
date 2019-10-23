#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/unistd.h>

int add(int a, int b, int* res)
{
    return syscall(__NR_add, a, b, res);
}

int main(int argc, char** argv) 
{
    int ret = 0;

    add(3, 5, &ret);
    printf("ret : %d\n", ret);

    return 0;
}
