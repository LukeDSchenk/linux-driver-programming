#include <linux/module.h> // needed by all modules
#include <linux/kernel.h> // needed for KERN_INFO (kernel logging)

int init_module(void) {
	printk(KERN_INFO "Hello, World (1)!\n");
	return 0; // non-zero return means the module can't be loaded in this case
}

void cleanup_module(void) {
	printk(KERN_INFO "Goodbye, World (1)!\n");
}
