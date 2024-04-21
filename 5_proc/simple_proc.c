#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>

static struct proc_dir_entry *simple_proc = NULL;
static char buf[100];

/**
 * 매개변수
 * page: procfs에서 항목이 사용하는 메모리 공간, 페이지 크기
 * off: 파일 오프셋
 * count: read or write할 때 지정한 count
 * eof: 파일의 끝을 의미하며 0으로 설정하면 반복해서 값이 출력, 1이면 한번만 출력
 * data: 전달할 데이터가 있을 때 사용되며 읽기에서는 사용하지 않는다.
 */
static int my_read(char *page, char **start, off_t off, int count, int *eof, void *data)
{
	int len;
	len = sprintf(page, "[Message] %s\n", (char *)data);
	*eof = 1;
	return len;
}

static int my_write(struct file *file, const char *buffer, unsigned long count, void *data)
{
	char *kernel_data;
	copy_from_user(kernel_data, buffer, count);

	kernel_data[count] = '\0';

	if (kernel_data[count-1] == '\n')
		kernel_data[count-1] = '\0';

	return count;
}

int __init simple_init(void)
{
	/*
	 * simple: 생성할 procfs 이름
	 * 0: 권한설정 (0으로 설정하면 기본값으로 0444)
	 * NULL: 상위 디렉토리 지정 (NULL이면 /proc)
	 */
	simple_proc = proc_create("simple", 0, NULL);
	if (simple_proc = NULL)
		return -ENOMEM;

	simple_proc->data = buf;
	simple_proc->read_proc = my_read;
	simple_proc->write_proc = my_write;
	simple_proc->owner = THIS_MODULE;

	return 0;
}

void __exit simple_exit(void)
{
	/*
	 * simple: 삭제할 procfs
	 * NULL: 상위 디렉토리 (NULL이면 /proc)
	 */
	remove_proc_entry("simple", NULL);
}

module_init(simple_init);
module_exit(simple_exit);
MODULE_LICENSE("GPL");
