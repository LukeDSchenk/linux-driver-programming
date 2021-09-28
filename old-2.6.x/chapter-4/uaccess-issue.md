# Context

It appears that my system is not able to locate the "uaccess.h" header file used for the `put_user()` function.
Upon further inspection, it appears that the proper files should be available, so I am unsure as to why it is not working. 
I am going to look into this issue a bit further, and if I can not figure out a solution (I am thinking maybe this has to do with the newer kernel version I am using compared to the text) I will try out the `copy_to_user()` function demonstrated by Engineer Man [here](https://www.youtube.com/watch?v=CWihl19mJig).

# Found more info!

According to [this post](https://stackoverflow.com/questions/46302524/copy-to-user-undefined-in-linux-kernel-version-4-12-8) on Stack Overflow, it would seem that the fatal error: *no such file or directory* implies that the asm/uaccess.h file is no longer supported by my current kernel version. I should look into how best to verify what is and isn't supported based on kernel version. 

Reading through this user's question and then scrolling down, it becomes clear that using **linux/uaccess.h** is now preferred over asm/uaccess.h. I will try compiling this way, and also compare the `put_user()` function to the `copy_to_user()` to see which is better in this case.
