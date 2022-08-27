/*
 * hello-4.c - Demonstrates module documentation macros.
 */
 #include <linux/init.h> // for the macros
 #include <linux/kernel.h> // for pr_info()
 #include <linux/module.h> // for kernel modules

 MODULE_LICENSE("GPL");
 MODULE_AUTHOR("LKMPG");
 MODULE_DESCRIPTION("A sample driver");

 static int hello_4_data __initdata = 4;

 static int __init hello4_init(void) {
     pr_info("Hello, World %d\n", hello_4_data);
     return 0;
 }

 static void __exit hello_4_exit(void) {
     pr_info("Goodbye, World 4\n");
 }

 module_init(hello_4_init);
 module_exit(hello_4_exit);
