#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("Dual BSD/GPL");

/* 모듈의 초기화 부분 */
static int initModule(void)
{
    printk(KERN_INFO "Hello module!\n");
    return 0;
}

/* 모듈이 내려질 때의 정리 부분 */
static void cleanupModule(void)
{
    printk(KERN_INFO "Good-bye module!\n");
}

/* 모듈 함수 등록 부분 */
module_init(initModule);
module_exit(cleanupModule);
