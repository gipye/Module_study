#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

/**
 * 모듈 초기화 함수
 * __init 매크로 필요
 * 모듈이 커널에 설치될 때 호출된다.
 */
int __init init_callee(void)
{
	return 0;
}

/**
 * 모듈 종료 함수
 * __exit 매크로 필요
 * 모듈이 커널에서 삭제될 때 호출된다.
 */
void __exit exit_callee(void)
{
}

int add(int a, int b)
{
	printk( KERN_ALERT "[callee Module] add called..\n" );
	return a + b;
}

int sub(int a, int b)
{
	printk( KERN_ALERT "[callee Module] sub called..\n" );
	return a - b;
}

/**
 * 심볼로 등록하는 매크로
 * add(), sub() 함수를 다른 모듈에서 사용할 수 있도록
 * 커널에 심볼을 노출시킨다.
 */
EXPORT_SYMBOL(add);
EXPORT_SYMBOL(sub);

/**
 * 모듈 초기화, 종료 함수 등록 매크로
 */
module_init(init_callee);
module_exit(exit_callee);

/**
 * 모듈 정보 명시하는 매크로
 */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Gipyong");
MODULE_DESCRIPTION("callee Module");
MODULE_VERSION("0.1.0");
