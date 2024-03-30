#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>

dev_t virtual_device_number;
struct cdev *virtual_device;

int virtual_device_open(struct inode *inode, struct file *filp)
{
	return 0;
}

int virtual_device_release(struct inode *inode, struct file *filp)
{
	return 0;
}

ssize_t virtual_device_write(struct file *filp, const char *buf, size_t count, loff_t *f_pos)
{
	printk( KERN_ALERT "virtual_device write function called\n" );
	return count;
}

ssize_t virtual_device_read(struct file *filp, char *buf, size_t count, loff_t *f_pos)
{
	printk( KERN_ALERT "virtual_device read function called\n" );
	return count;
}

static struct file_operations vd_fops = {
	.read = virtual_device_read,
	.write = virtual_device_write,
	.open = virtual_device_open,
	.release = virtual_device_release,
};

int __init dev_init(void)
{
	virtual_device_number = MKDEV(252, 10);
	register_chrdev_region(virtual_device_number, 1, "virtual_device");

	virtual_device = cdev_alloc();
	cdev_init(virtual_device, &vd_fops);
	cdev_add(virtual_device, virtual_device_number, 1);

	printk("[Module Message] Major: %d\n", MAJOR(virtual_device_number));
	printk("[Module Message] Minor: %d\n", MINOR(virtual_device_number));

	return 0;
}

void __exit dev_exit(void)
{
	cdev_del(virtual_device);
	unregister_chrdev_region(virtual_device_number, 1);
}

module_init(dev_init);
module_exit(dev_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("KIPYEONG");
MODULE_DESCRIPTION("cdev example");
