#include "philo.h"

static int	philo_died(t_philo *philo)
{
	long	now;
	long	last_meal;
	int		is_eating;

	now = get_time_ms();
	pthread_mutex_lock(&philo->table->state_mutex);
	last_meal = philo->last_meal;
	is_eating = philo->is_eating;
	pthread_mutex_unlock(&philo->table->state_mutex);
	if (is_eating && philo->table->time_to_die > philo->table->time_to_eat)
		return (0);
	return (now - last_meal >= philo->table->time_to_die);
}

static int	all_philos_ate_enough(t_table *table)
{
	int	i;
	int	meals_eaten;

	if (table->must_eat == -1)
		return (0);
	i = 0;
	while (i < table->philo_count)
	{
		pthread_mutex_lock(&table->state_mutex);
		meals_eaten = table->philos[i].meals_eaten;
		pthread_mutex_unlock(&table->state_mutex);
		if (meals_eaten < table->must_eat)
			return (0);
		i++;
	}
	return (1);
}

void	monitor_simulation(t_table *table)
{
	int	i;

	while (!simulation_stopped(table))
	{
		i = 0;
		while (i < table->philo_count)
		{
			if (philo_died(&table->philos[i]))
			{
				stop_simulation(table);
				print_status(&table->philos[i], "died");
				return ;
			}
			i++;
		}
		if (all_philos_ate_enough(table))
		{
			stop_simulation(table);
			return ;
		}
		usleep(100);
	}
}
