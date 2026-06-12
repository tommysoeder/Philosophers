#include "philo.h"

long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

long	timestamp(t_table *table)
{
	return (get_time_ms() - table->start_time);
}

void	precise_sleep(t_table *table, long duration_ms)
{
	long	start;

	start = get_time_ms();
	while (!simulation_stopped(table)
		&& get_time_ms() - start < duration_ms)
		usleep(100);
}
