// procfs1.c - create a "file" in /proc

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>  // necessary in order to use the proc fs
#include <linux/uaccess.h>
#include <asm/uaccess.h>

#define procfs_name "helloworld"

// This structure holds information about the /proc file.
struct proc_dir_entry *our_proc_file;
int len, temp;
char *msg;
char *data;


ssize_t procfile_read(struct file *filep, char *buf, size_t count, loff_t *offp) {
	printk(KERN_INFO "procfile_read (/proc/%s) called\n", procfs_name);
	
        data = PDE_DATA(file_inode(filep));
	if (!data) {
		printk(KERN_INFO "Null data");
		return 0;
	}

	if (count > temp)
		count = temp;
	temp = temp - count;
	
	copy_to_user(buf, data, count);
	if (count == 0) {
		temp = len;
		return count;
	}
}

struct file_operations proc_fops = {
        .owner = THIS_MODULE,
        .read = procfile_read
};

int init_module() {
	// create_proc_entry() is a deprecated function
	// instead, we will want to use proc_create()
	// which takes the same params plus a fops struct
	//our_proc_file = create_proc_entry(procfs_name, 0644, NULL);
	our_proc_file = proc_create(procfs_name, 0644, NULL, &proc_fops); // proc_create_data is used to pass kwargs

	if (our_proc_file == NULL) {
		remove_proc_entry(procfs_name, NULL); // second param is NULL because there is no parent
		printk(KERN_ALERT "Error: Could not initialize /proc/%s\n", procfs_name);
		return -ENOMEM;
	}

	printk(KERN_INFO "/proc/%s created\n", procfs_name);
	return 0;        // everything is okay
}

void cleanup_module() {
	remove_proc_entry(procfs_name, NULL); // second parameter is NULL because there is no parent
	printk(KERN_INFO "/proc/%s removed\n", procfs_name);
}
