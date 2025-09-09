#include <sys/time.h>

unsigned long	time_now_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, (void *)0);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}
