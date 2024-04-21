#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>

/**
 * 모듈 초기화 함수
 * __init 매크로 필요
 * 모듈이 커널에 설치될 때 호출된다.
 *
 * 시간 지연 예제
 * 모듈 설치할 때 10초 시간지연을 설정하는 예제이다.
 * 단 실제로는 지연을 위해서는 아래와 같은 코드를 쓰지는 않음
 * 보통은 schedule_timeout_interruptible(delay); 사용
 */
int __init delay_init(void)
{
	/**
	 * jiffies 값을 얻어온다.
	 * 10초 딜레이 설정
	 */
	unsigned long delay = get_jiffies_64() + 10 * HZ;

	/**
	 * 프로세스를 인터럽트 가능 상태로 설정
	 */
	set_current_state(TASK_INTERRUPTIBLE);

	/**
	 * cond_resched(): schedule()와 비슷함.
	 * need_resched() 함수로 스케줄링이 필요한지 판단하여
	 * 스케줄링이 필요할 때만 schedule()를 호출한다.
	 */
	while (time_before(get_jiffies_64(), delay))
		cond_resched();

	return 0;
}

/**
 * 모듈 종료 함수
 * __exit 매크로 필요
 * 모듈이 커널에서 제거될 때 호출된다.
 */
void __exit delay_exit(void)
{
}

/**
 * 모듈 초기화, 종료 함수 등록하고,
 * 모듈 정보 명시하는 매크로
 */
module_init(delay_init);
module_exit(delay_exit);
MODULE_LICENSE("GPL");
