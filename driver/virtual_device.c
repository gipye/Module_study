#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>		// using file system
#include <asm/uaccess.h>	// for copy_to_user()
#include <linux/slab.h>		// for kmalloc()

static char *buffer = NULL;
static int major = 0;

int virtual_device_open(struct inode *inode, struct file *flip)
{
	printk( KERN_ALERT "virtual device open function called\n" );
	return 0;
}

int virtual_device_release(struct inode *inode, struct file *flip)
{
	printk( KERN_ALERT "virtual device release function called\n" );
	return 0;
}

ssize_t virtual_device_read(struct file *flip, char *buf, size_t count, loff_t *f_pos)
{
    printk(KERN_ALERT "virtual device read function called\n");
    if (copy_to_user(buf, buffer, 1024))
        return -EFAULT;
    return count;
}

ssize_t virtual_device_write(struct file *flip, const char *buf, size_t count, loff_t *f_pos)
{
	printk( KERN_ALERT "virtual device write function called\n" );
	strcpy(buffer, buf);
	return count;
}

static struct file_operations vd_fops = {
	.open = virtual_device_open,
	.release = virtual_device_release,
	.read = virtual_device_read,
	.write = virtual_device_write,
};

int __init virtual_device_init(void)
{
	int result = register_chrdev(0, "virtual_device", &vd_fops);
	if (result < 0) {
		printk( KERN_ALERT "driver init failed: %d\n", result );
		return result;
	}

	major = result;
	printk( KERN_ALERT "driver init successful: %d\n", major );

	buffer = (char *)kmalloc(1024, GFP_KERNEL);
	if (buffer)
		memset(buffer, 0, 1024);

	return 0;
}

void __exit virtual_device_exit(void)
{
	unregister_chrdev(major, "virtual_device"); 
	printk( KERN_ALERT "driver cleanup successful\n" );

	if (buffer)
		kfree(buffer);
}

module_init(virtual_device_init);
module_exit(virtual_device_exit);

MODULE_LICENSE("GPL");
