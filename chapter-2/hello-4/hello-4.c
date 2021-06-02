#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

#define DRIVER_AUTHOR "Luke Donald Schenk <nineofpine@protonmail.com>"
#define DRIVER_DESC   "A sample driver from The Linux Driver Programming manual."

static int __init init_hello_4(void) {
	printk(KERN_INFO "Hello, World (4)!\n");
	return 0;
}

static void __exit cleanup_hello_4(void) {
	printk(KERN_INFO "Goodbye, World (4)!\n");
}

module_init(init_hello_4);
module_exit(cleanup_hello_4);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);

/*
 * This module uses /dev/testdevice. The MODULE_SUPPORTED_DEVICE macro might be
 * used in the future to help automatic configuration of modules, but is currently
 * unused other than for documentation purposes.
*/
MODULE_SUPPORTED_DEVICE("testdevice");
