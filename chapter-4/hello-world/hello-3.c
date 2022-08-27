/*
 * hello-3.c - Illustrating the __init, __exit, and __initdata macros.
 */
#include <linux/init.h> // for the macros
#include <linux/kernel.h> // for pr_info()
#include <linux/module.h> // for kernel modules

static int hello_3_data __initdata = 3;

static int __init hello3_init(void) {
    pr_info("Hello, World %d\n", hello_3_data);
    return 0;
}

static void __exit hello_3_exit(void) {
    pr_info("Goodbye, World 3\n");
}

module_init(hello_3_init);
module_exit(hello_3_exit);
MODULE_LICENSE("GPL");
