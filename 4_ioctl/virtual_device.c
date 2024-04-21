#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>

static int major = 0;

/**
 * 가상 파일 시스템
 * dev 파일에 대해 사용할 파일 연산 함수들을 정의할 수 있다.
 */

/**
 * open() 시스템콜 호출 시 호출될 함수 정의
 */
int virtual_device_open(struct inode *inode, struct file *filp)
{
	printk( KERN_ALERT "virtual device open function called\n" );
	return 0;
}

/**
 * close() 시스템콜 호출 시 호출될 함수 정의
 */
int virtual_device_release(struct inode *inode, struct file *filp)
{
	printk( KERN_ALERT "virtual device release function called\n" );
	return 0;
}

/**
 * ioctl() 시스템 콜 호출 시 호출될 함수 정의
 */
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
 * file_operations 구조체: 디바이스 드라이버에서 구현해야 할 기본 함수들의 목록
 * 여기서는 위에 정의한 함수 세 개만 연결한다.
 */
static struct file_operations vd_fops = {
	.owner = THIS_MODULE,
	.open = virtual_device_open,
	.release = virtual_device_release,
	.unlocked_ioctl = virtual_device_ioctl,
};

/**
 * 모듈 초기화 함수
 * __init 매크로 필요
 * 모듈이 커널에 설치될 때 호출된다.
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
 * 모듈 종료 함수
 * __exit 매크로 필요
 * 모듈이 커널에서 제거될 때 호출된다.
 */
void __exit virtual_device_exit(void)
{
	unregister_chrdev(major, "virtual_device");
	printk( KERN_ALERT "driver cleanup successful\n" );
}

/**
 * 모듈 초기화, 종료 함수 등록하고,
 * 모듈 정보 명시하는 매크로
 */
module_init(virtual_device_init);
module_exit(virtual_device_exit);

MODULE_LICENSE("GPL");
