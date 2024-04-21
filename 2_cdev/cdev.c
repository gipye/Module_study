#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>

dev_t virtual_device_number;
struct cdev *virtual_device;

/**
 * 가상 파일 시스템
 * dev 파일에 대해 사용할 파일 연산 함수들을 정의할 수 있다.
 */

/**
 * open() 시스템콜 호출 시 호출될 함수 정의
 */
int virtual_device_open(struct inode *inode, struct file *filp)
{
	return 0;
}

/**
 * close() 시스템콜 호출 시 호출될 함수 정의
 */
int virtual_device_release(struct inode *inode, struct file *filp)
{
	return 0;
}

/**
 * write() 시스템콜 호출 시 호출될 함수 정의
 */
ssize_t virtual_device_write(struct file *filp, const char *buf, size_t count, loff_t *f_pos)
{
	printk( KERN_ALERT "virtual_device write function called\n" );
	return count;
}

/**
 * read() 시스템콜 호출 시 호출될 함수 정의
 */
ssize_t virtual_device_read(struct file *filp, char *buf, size_t count, loff_t *f_pos)
{
	printk( KERN_ALERT "virtual_device read function called\n" );
	return count;
}

/**
 * file_operations 구조체: 디바이스 드라이버에서 구현해야 할 기본 함수들의 목록
 * 여기서는 위에 정의한 함수 네 개만 연결한다.
 */
static struct file_operations vd_fops = {
	.read = virtual_device_read,
	.write = virtual_device_write,
	.open = virtual_device_open,
	.release = virtual_device_release,
};

/**
 * 모듈 초기화 함수
 * __init 매크로 필요
 * 모듈이 커널에 설치될 때 호출된다.
 */
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

/**
 * 모듈 종료 함수
 * __exit 매크로 필요
 * 모듈이 커널에서 제거될 때 호출된다.
 */
void __exit dev_exit(void)
{
	cdev_del(virtual_device);
	unregister_chrdev_region(virtual_device_number, 1);
}

/**
 * 모듈 초기화, 종료 함수 등록하고,
 * 모듈 정보 명시하는 매크로
 */
module_init(dev_init);
module_exit(dev_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("KIPYEONG");
MODULE_DESCRIPTION("cdev example");
