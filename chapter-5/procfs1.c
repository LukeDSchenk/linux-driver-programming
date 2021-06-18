// Module to create a proc entry. The user can read and write to the proc entry.
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include <asm/types.h>

#define DATA_SIZE 1024 // The size of the data this proc entry can take
#define MY_PROC_ENTRY "helloworld"
#define PROC_FULL_PATH "/proc/helloworld"

// okay, so I think the reasoning behind the common global variable usage has to
// do with the fact that we are actually reserving kernel memory for this stuff
struct proc_dir_entry *proc;
int len;
char *msg = NULL;

/*
 * Function to write to the proc. Here we free get the new value from buffer,
 * count the buffer and then overwrite the data in our file.
 *
 * Note that - you can have any other implementation as well for this, all you
 * have to ensure is that you comply with the expectations of the write() system
 * calls like filling in the buffer, and returning the numbers of characters written.
*/

/// For writing to our proc file. Prints the data written in kern.log.
/// count is the count of the user space buffer we are writing to the proc file!!!
static ssize_t my_proc_write(struct file *filp, const char __user *buffer, size_t count, loff_t *pos) {
    // it looks like this here is used to get the data written to the file from user land
    char *data = PDE_DATA(file_inode(filp)); // PDE_DATA is in proc_fs.h, file_inode is in fs.h? why isn't it included?
    int i; // iterator, we will use it twice here

    // if the user buffer is longer than the data size of this file, throw an error
    if (count > DATA_SIZE) {
        return -EFAULT; // errno 14: EFAULT bad address: basically sigsegv in kernel land
    }

    printk(KERN_INFO "Printing the data being passed. count is %lu", (size_t) count);
    for (i=0; i < count; i++) {
        printk(KERN_INFO "Index: %d . Character %c ASCII: %d", i, buffer[i], buffer[i]);
    }

    printk(KERN_INFO "Writing to proc");
    if (copy_from_user(data, buffer, count)) { // this will return 0 if nothing is wrong
        return -EFAULT;                         // quick side note here, I really really prefer it when
    }                                           // functions return negative values on error, and I hate
                                                // how in C there seems to be no standard way for that stuff, idk
                                                // or is it that in library functions it is when negative values are returned? idfk
    data[count-1] = '\0';

    printk(KERN_INFO "msg has been set as %s", msg);
    printk(KERN_INFO "Message is: ");
    for (i=0; i < count; i++) {
        printk(KERN_INFO "\n Index: %d . Character: %c", i, msg[i]);
    }

    *pos = (int) count;
    len = count - 1;

    return count; // return number of bytes written, following write() standards
}

/// Function used to read the proc entry, here we copy data from our proc entry
/// to the buffer passed.
ssize_t my_proc_read(struct file *filp, char *buf, size_t count, loff_t *offp) {
    int err;
    char *data = PDE_DATA(file_inode(filp)); // ahh, so I think this basically just returns whatever
                                             // is in the file (perhaps placed by a call to our write function),
                                             // so I think we could replace this with whatever the hell we wanted?
    if ((int) (*offp) > len) {
        return 0;
    }

    printk(KERN_INFO "Reading the proc entry, len of the file is %d", len);
    if(!(data)) {
        printk(KERN_INFO "NULL DATA");
        return 0;
    }

    if (count == 0) {
        printk(KERN_INFO "Read of size zero, doing nothing");
        return count;
    } else {
        printk(KERN_INFO "Read of size %d", (int) count);
    }

    count = len + 1; // +1 to read the \0 (null terminator) ??
    err = copy_to_user(buf, data, count); // replace data here with whatever you want, and get rid of the write functionality
    printk(KERN_INFO "Read data: %s", buf);
    *offp = count;

    if (err) {
        printk(KERN_INFO "Error in copying data");
    } else {
        printk(KERN_INFO "Successfully copied data");
    }

    return count;
}

/// The file_operations structure to associate our proc_entry with read/write ops.
struct file_operations proc_fops = {
    .read = my_proc_read,
    .write = my_proc_write,
};

/*
 * This function will create the proc entry. This function will allocate some data
 * where the data will be written incase of a write to the proc entry. The same
 * memory will be used to serve the reads. Initially, the function fills the data
 * with DATA which contains "Hello, World!".

 * The important function to see here is the proc_create_data, this function will
 * take the proc entry name and create it with the given permissions (0666).
 * We also need to pass the fops struct which has the function pointers to the functions
 * to be called when read() or write() is called on the file. The last argument
 * has the pointer to the data associated with the file.
*/
int create_new_proc_entry(void) {
    int i;
    char *DATA = "Hello, World!";
    len = strlen(DATA);
    msg = kmalloc((size_t) DATA_SIZE, GFP_KERNEL); // +1 for \0, kmalloc is just kernel malloc

    if (msg != NULL) {
        printk(KERN_INFO "Allocated memory for msg");
    } else {
        return -1;
    }

    strncpy(msg, DATA, len+1); // copy full length of DATA and \0 into msg
    for (i=0; i < len+1; i++) {
        printk(KERN_INFO "%c", msg[i]);
        if (msg[i] == '\0') {
            printk(KERN_INFO "YES");
        }
    }

    proc = proc_create_data(MY_PROC_ENTRY, 0666, NULL, &proc_fops, msg); // ohhhh so basically proc_create() is used when you have no data to initialize in the proc file ooooooooooohhhhhhhhh
    if (proc) { // returns a pointer to a proc entry, so this works
        return 0;
    } else {
        return -1;
    }
}

/// The init function of the module. Does nothing other than calling create_new_proc_entry().
int proc_init(void) {
    if (create_proc_entry()) {
        return -1;
    }
    return 0;
}

/// Function to remove the proc entry. Call this when the module unloads.
void proc_cleanup(void) {
    remove_proc_entry(MY_PROC_ENTRY, NULL);
}

MODULE_LICENSE("GPL");
module_init(proc_init);
module_exit(proc_cleanup);
