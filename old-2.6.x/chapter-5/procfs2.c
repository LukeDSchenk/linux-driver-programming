// procfs2.c - create a "file" in /proc

#include <linux/module.h>  // we are writing a module
#include <linux/kernel.h>  // we are doing kernel programming
#include <linux/proc_fs.h> // we are using the proc file system
#include <asm/uaccess.h>   // for copy_from_user

#define PROCFS_MAX_SIZE 1024
#define PROCFS_NAME     "buffer1k"

// this struct holds info about the /proc file
static struct proc_dir_entry *our_proc_file;

// the buffer used to store characters for this module
static char procfs_buffer[PROCFS_MAX_SIZE];

// the size of the buffer
static unsigned long procfs_buffer_size = 0;

/// This function is called when our /proc file is read
size_t procfile_read(char *buffer, char **buffer_location, off_t offset, int buffer_length, int *eof, void *data) {
    
}
