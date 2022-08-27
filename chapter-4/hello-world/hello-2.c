/*
 * hello-2.c - Demonstrating the module_init() and module_exit() macros.
 * This is preferred over using init_module() and cleanup_module().
 */
#include <linux/init.h> // for the init/exit macros
#include <linux/kernel.h> // for pr_info()
#include <linux/module.h> // required for kernel modules

MODULE_LICENSE("GPL");

static int __init hello_2_init(void) {
    pr_info("Hello, Second World!\n");
    return 0;
}

static void __exit hello_2_exit(void) {
    pr_info("Goodbye, Second World.\n");
}

module_init(hello_2_init);
module_exit(hello_2_exit);
