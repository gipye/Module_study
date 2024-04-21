#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

/**
 * 외부에 정의된 함수를 사용하기 위한 헤더 선언
 */
int add(int, int);
int sub(int, int);

/**
 * 모듈 초기화 함수
 * __init 매크로 필요
 * 모듈이 커널에 설치될 때 호출된다.
 */
int __init init_caller(void)
{
	printk( KERN_ALERT "[caller Module] call add(), sub()\n" );
	printk( KERN_ALERT "[caller Module] add: %d\n", add( 3, 2 ) );
	printk( KERN_ALERT "[caller Module] sub: %d\n", sub( 3, 2 ) );

	return 0;
}

/**
 * 모듈 종료  함수
 * __exit 매크로 필요
 * 모듈이 커널에서 제거될 때 호출된다.
 */
void __exit exit_caller(void)
{
}

/**
 * 모듈 초기화, 종료 함수 등록
 */
module_init(init_caller);
module_exit(exit_caller);

/**
 * 모듈 정보를 명시하는 매크로
 */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Gipyong");
MODULE_DESCRIPTION("caller Module");
MODULE_VERSION("0.1.0");
