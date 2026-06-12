#include "philo.h"

static void	join_threads(t_table *table, int created)
{
	int	i;

	i = 0;
	while (i < created)
	{
		pthread_join(table->philos[i].thread, NULL);
		i++;
	}
}

static void	set_start_time(t_table *table)
{
	int	i;

	table->start_time = get_time_ms();
	i = 0;
	while (i < table->philo_count)
	{
		pthread_mutex_lock(&table->state_mutex);
		table->philos[i].last_meal = table->start_time;
		pthread_mutex_unlock(&table->state_mutex);
		i++;
	}
}

int	start_simulation(t_table *table)
{
	int	i;

	set_start_time(table);
	i = 0;
	while (i < table->philo_count)
	{
		if (pthread_create(&table->philos[i].thread, NULL,
				philo_routine, &table->philos[i]) != 0)
		{
			stop_simulation(table);
			join_threads(table, i);
			return (1);
		}
		i++;
	}
	monitor_simulation(table);
	join_threads(table, table->philo_count);
	return (0);
}
