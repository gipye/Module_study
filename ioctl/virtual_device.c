#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>

static int major = 0;

int virtual_device_open(struct inode *inode, struct file *filp)
{
	printk( KERN_ALERT "virtual device open function called\n" );
	return 0;
}

int virtual_device_release(struct inode *inode, struct file *filp)
{
	printk( KERN_ALERT "virtual device release function called\n" );
	return 0;
}

long virtual_device_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	printk( KERN_ALERT "cmd: %d\n", cmd );
	switch (cmd) {
	case 1:
		printk( KERN_ALERT "ioctl read...\n" );
		break;
	case 2:
		printk( KERN_ALERT "ioctl write...\n" );
		break;
	default:
		printk( KERN_ALERT "ioctl unknown command...\n" );
		break;
	}
	return 0;
}

/**
 * 커스텀 연산을 등록한 구조체 생성
 */
static struct file_operations vd_fops = {
	.owner = THIS_MODULE,
	.open = virtual_device_open,
	.release = virtual_device_release,
	.unlocked_ioctl = virtual_device_ioctl,
};

/**
 * 모듈 초기화 루틴 함수
 */
int __init virtual_device_init(void)
{
	/* 커스텀 연산 구조체를 적용한 문자 디바이스 생성 */
	int result = register_chrdev(0, "virtual_device", &vd_fops);
	if (result < 0) {
		printk( KERN_ALERT "driver init failed\n" );
		return result;
	}

	major = result;
	printk( KERN_ALERT "driver init successful: %d\n", major );
	return 0;
}

/**
 * 모듈 종료 루틴 함수
 */
void __exit virtual_device_exit(void)
{
	unregister_chrdev(major, "virtual_device");
	printk( KERN_ALERT "driver cleanup successful\n" );
}

/* 초기화, 종료 루틴 함수 등록 */
module_init(virtual_device_init);
module_exit(virtual_device_exit);

MODULE_LICENSE("GPL");
