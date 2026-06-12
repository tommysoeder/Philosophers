#include "philo.h"

static void	take_forks(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->right_fork);
		print_status(philo, "has taken a fork");
		pthread_mutex_lock(philo->left_fork);
		print_status(philo, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(philo->left_fork);
		print_status(philo, "has taken a fork");
		pthread_mutex_lock(philo->right_fork);
		print_status(philo, "has taken a fork");
	}
}

static void	drop_forks(t_philo *philo)
{
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

static int	ate_enough(t_philo *philo)
{
	int	ate_enough;

	if (philo->table->must_eat == -1)
		return (0);
	pthread_mutex_lock(&philo->table->state_mutex);
	ate_enough = (philo->meals_eaten >= philo->table->must_eat);
	pthread_mutex_unlock(&philo->table->state_mutex);
	return (ate_enough);
}

static void	*single_philo_routine(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	print_status(philo, "has taken a fork");
	while (!simulation_stopped(philo->table))
		usleep(100);
	pthread_mutex_unlock(philo->left_fork);
	return (NULL);
}

static void	eat(t_philo *philo)
{
	t_table	*table;

	table = philo->table;
	take_forks(philo);
	pthread_mutex_lock(&table->state_mutex);
	philo->last_meal = get_time_ms();
	philo->is_eating = 1;
	pthread_mutex_unlock(&table->state_mutex);
	print_status(philo, "is eating");
	precise_sleep(table, table->time_to_eat);
	pthread_mutex_lock(&table->state_mutex);
	philo->is_eating = 0;
	philo->meals_eaten++;
	pthread_mutex_unlock(&table->state_mutex);
	drop_forks(philo);
}

static void	stagger_start(t_philo *philo)
{
	if (philo->id % 2 == 0)
		precise_sleep(philo->table, philo->table->time_to_eat);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->table->philo_count == 1)
		return (single_philo_routine(philo));
	stagger_start(philo);
	while (!simulation_stopped(philo->table) && !ate_enough(philo))
	{
		eat(philo);
		print_status(philo, "is sleeping");
		precise_sleep(philo->table, philo->table->time_to_sleep);
		print_status(philo, "is thinking");
	}
	return (NULL);
}
