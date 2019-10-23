#include <linux/kernel.h>

asmlinkage long sys_helloworld(void)
{
    printk(KERN_INFO "Hello World! from Linux Kernel\n");

    return 0;
}
