/*
 * hello-1.c - The simplest kernel module.
 */
#include <linux/kernel.h> // needed for pr_info()
#include <linux/module.h> // needed by all modules

MODULE_LICENSE("GPL");

int init_module(void) {
    pr_info("Hello, World!\n");
    return 0; // non-zero return means init_module failed; module can't be loaded
              // negative returns will cause the module to not load at all
              // positive returns will attempt to load but fail and mem dump in kern.log
}

void cleanup_module(void) {
    pr_info("Goodbye, World.\n");
}
