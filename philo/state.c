#include "philo.h"

int	simulation_stopped(t_table *table)
{
	int	stop;

	pthread_mutex_lock(&table->state_mutex);
	stop = table->stop;
	pthread_mutex_unlock(&table->state_mutex);
	return (stop);
}

void	stop_simulation(t_table *table)
{
	pthread_mutex_lock(&table->state_mutex);
	table->stop = 1;
	pthread_mutex_unlock(&table->state_mutex);
}
