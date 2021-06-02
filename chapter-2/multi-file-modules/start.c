#include <linux/kernel.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Luke Donald Schenk <nineofpine@protonmail.com>");

int init_module(void) {
	printk(KERN_INFO "Hello, World - this is the kernel speaking\n");
	return 0;
}
