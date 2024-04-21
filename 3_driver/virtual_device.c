#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>		// using file system
#include <asm/uaccess.h>	// for copy_to_user()
#include <linux/slab.h>		// for kmalloc()

static char *buffer = NULL;
static int major = 0;

/**
 * 가상 파일 시스템
 * dev 파일에 대해 사용할 파일 연산 함수들을 정의할 수 있다.
 */

/**
 * open() 시스템콜 호출 시 호출될 함수 정의
 */
int virtual_device_open(struct inode *inode, struct file *flip)
{
	printk( KERN_ALERT "virtual device open function called\n" );
	return 0;
}

/**
 * close() 시스템콜 호출 시 호출될 함수 정의
 */
int virtual_device_release(struct inode *inode, struct file *flip)
{
	printk( KERN_ALERT "virtual device release function called\n" );
	return 0;
}

/**
 * read() 시스템콜 호출 시 호출될 함수 정의
 */
ssize_t virtual_device_read(struct file *flip, char *buf, size_t count, loff_t *f_pos)
{
    printk(KERN_ALERT "virtual device read function called\n");
    if (copy_to_user(buf, buffer, 1024))
        return -EFAULT;
    return count;
}

/**
 * write() 시스템콜 호출 시 호출될 함수 정의
 */
ssize_t virtual_device_write(struct file *flip, const char *buf, size_t count, loff_t *f_pos)
{
	printk( KERN_ALERT "virtual device write function called\n" );
	strcpy(buffer, buf);
	return count;
}

/**
 * file_operations 구조체: 디바이스 드라이버에서 구현해야 할 기본 함수들의 목록
 * 여기서는 위에 정의한 함수 네 개만 연결한다.
 */
static struct file_operations vd_fops = {
	.open = virtual_device_open,
	.release = virtual_device_release,
	.read = virtual_device_read,
	.write = virtual_device_write,
};

/**
 * 모듈 초기화 함수
 * __init 매크로 필요
 * 모듈이 커널에 설치될 때 호출된다.
 */
int __init virtual_device_init(void)
{
	int result = register_chrdev(0, "virtual_device", &vd_fops);
	if (result < 0) {
		printk( KERN_ALERT "driver init failed: %d\n", result );
		return result;
	}

	major = result;
	printk( KERN_ALERT "driver init successful: %d\n", major );

	/**
	 * 드라이버 설치 시 파일시스템에서 사용할 메모리 공간을 차지한다.
	 * 해당 드라이버 예제에서는 write() 호출 시 데이터 저장할 공간을 할당,
	 * 또한 read() 호출 시 데이터를 읽을 메모리 공간임.
	 */
	buffer = (char *)kmalloc(1024, GFP_KERNEL);
	if (buffer)
		memset(buffer, 0, 1024);

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

	if (buffer)
		kfree(buffer);
}

/**
 * 모듈 초기화, 종료 함수 등록하고,
 * 모듈 정보 명시하는 매크로
 */
module_init(virtual_device_init);
module_exit(virtual_device_exit);

MODULE_LICENSE("GPL");
