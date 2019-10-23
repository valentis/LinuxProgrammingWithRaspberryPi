#include <linux/unistd.h>
#include <linux/kernel.h>
#include <asm/uaccess.h>
#include <asm/errno.h>

asmlinkage int sys_add(int a, int b, int *res) 
{
    int err = 0, ret = 0;

    err = access_ok(res, sizeof(int)); /* 사용자 영역에 쓸 수 있는지 검사 */
    if(err == -EFAULT) return err;

    ret = a + b;
    put_user(ret, res); /* 사용자 영역으로 값을 보낸다. */

    printk(KERN_INFO "%d + %d = %d from Linux Kernel\n", a, b, *res);
 
    return err;
}
