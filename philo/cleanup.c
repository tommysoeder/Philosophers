#include "philo.h"

void	cleanup_table(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->philo_count)
	{
		pthread_mutex_destroy(&table->forks[i]);
		i++;
	}
	pthread_mutex_destroy(&table->state_mutex);
	pthread_mutex_destroy(&table->print_mutex);
	free(table->philos);
	free(table->forks);
}
