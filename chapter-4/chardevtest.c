/*
 * chardev.c: Creates a read-only char device that says how many times you've read from the file
*/

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <asm/uaccess.h>  // for put_user
//#include <linux/uaccess.h> // this is the file used by Engineer Man for copy_to_user()

MODULE_AUTHOR("Luke Schenk");
MODULE_LICENSE("GPL");

// Prototypes - this would normally go in a header file
int init_module(void);
void cleanup_module(void);
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);

#define DEVICE_NAME "chardev" // Dev name as it appears in /proc/devices
#define BUF_LEN 80            // Max length of the message from the device

// Global variables are declared as static, so they are global only within this file

static int Major;             // major number assigned to out device driver
static int Device_Open = 0;   // is device open? Used to prevent multiple access to device

static char msg[BUF_LEN];     // the message the device will give when asked 
static char *msg_ptr;

static struct file_operations fops = {
	.read = device_read,
	.write = device_write,
	.open = device_open,
	.release = device_release
};

/// This function is called when the module is loaded.
int init_module(void) {
	Major = register_chrdev(0, DEVICE_NAME, &fops);
	if (Major < 0) {
		printk(KERN_ALERT "Registering char device failed with %d\n", Major);
		return Major;
	}

	printk(KERN_INFO "I was assigned major number %d. To talk to\n", Major);
	printk(KERN_INFO "the driver, create a dev file with\n");
	printk(KERN_INFO "'mknod /dev/%s c %d 0'.\n", DEVICE_NAME, Major);
	printk(KERN_INFO "Try various minor numbers. Try to cat and echo to \n");
	printk(KERN_INFO "the device file.\n");
	printk(KERN_INFO "Remove the device file and module when done.\n");

	return 0;
}

/// This function is called when the module is unloaded.
void cleanup_module(void) {
	// The code presented in the example will throw an error on modern kernels
	// I am using 5.4.0-74-generic
	// The version of unregister_chrdev() used by my kernel version is a void function,
	// and therefore should not be error checked
	unregister_chrdev(Major, DEVICE_NAME);
	printk(KERN_INFO "Module unloaded!\n");
}


///////////////////////////////////////////////
///////////////// Methods /////////////////////
///////////////////////////////////////////////


/// Called when a process tries to open the device file;
/// like `cat /dev/mycharfile`.
static int device_open(struct inode *inode, struct file *file) {
	static int counter = 0;

	if (Device_Open)
		return -EBUSY; // EBUSY is an error code referring to a busy device/resource.
	                       // Here it is used as a negative to indicate failure.

	Device_Open++;
	sprintf(msg, "I already told you %d times Hello world!\n", counter++);
	msg_ptr = msg;
	try_module_get(THIS_MODULE);

	return 0;
}

/// Called when a process closes the device file.
static int device_release(struct inode *inode, struct file *file) {
	Device_Open--;  // We are now ready for our next caller

	// Decrement the usage count, or else once you opened the file
	// you will never get rid of this module.
	module_put(THIS_MODULE);

	return 0;
}

// NOTE: ssize_t simply means "signed size type", so you should use size_t for returning
//       a value, such as a number of bytes read, and ssize_t when returning a size/value
//       which could also be a negative number indicating failure, etc.

/// Called when a process, which already opened the dev file, attempts to read from it.
static ssize_t device_read(struct file *filp, // see include/linux/fs.h
                           char *buffer,      // buffer to fill with data
			   size_t length,     // length of the buffer
			   loff_t *offset) {
	
	// Number of bytes actually written to the buffer
	int bytes_read = 0;

	// If we're at the end of the message,
	// return 0 signifying the end of the file
	if (*msg_ptr == 0)
		return 0;

	// Actually put the data into the buffer
	while (length && *msg_ptr) {
		// The buffer is in the user data segment, not the kernel segment.
		// Therefore "*" assignment won't work. We have to use put_user,
		// which copies data from the kernel data segment to the user segment.
		put_user(*(msg_ptr++), buffer++); // this is essentially copying the data from 
		                                  // the message pointer to the user buffer,
						  // a single byte at a time. It stops when all
						  // bytes have been read from the device file, 
						  // or the user segment buffer is out of room

		length--;
		bytes_read++;
	}

	// Most read functions return the number of bytes put into the buffer
	return bytes_read;
}

/// Called when a process writes to the dev file: echo "hi" > /dev/hello
static ssize_t device_write(struct file *filp, const char *buff, size_t len, loff_t *off) {
	printk(KERN_ALERT "Sorry, this operation isn't supported.\n");
	return -EINVAL;
}
