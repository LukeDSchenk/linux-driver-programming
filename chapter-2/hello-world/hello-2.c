#include <linux/module.h>  // needed by all modules
#include <linux/kernel.h>  // needed for kernel log priorities
#include <linux/init.h>    // needed for function macros

// remember, static functions are restricted to the file in which they are shared
static int __init hello_2_init(void) {
	printk(KERN_INFO "Hello, World (2)!\n");
	return 0;
}

static void __exit hello_2_exit(void) {
	printk(KERN_INFO "Goodbye, World (2)!\n");
}

module_init(hello_2_init);
module_exit(hello_2_exit);
