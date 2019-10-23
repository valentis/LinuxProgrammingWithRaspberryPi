#include <unistd.h>
#include <linux/unistd.h>

int helloworld()
{
    //return syscall(__NR_SYSCALL_BASE+294);
    return syscall(__NR_helloworld);
}

int main(int argc, char** argv)
{
    int ret = helloworld();

    return ret;
}
