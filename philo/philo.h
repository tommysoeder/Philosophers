#ifndef PHILO_H
# define PHILO_H

# include <limits.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_table	t_table;

typedef struct s_philo
{
	int				id;
	int				meals_eaten;
	int				is_eating;
	long			last_meal;
	pthread_t		thread;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	t_table			*table;
}	t_philo;

struct s_table
{
	int		philo_count;
	long	time_to_die;
	long	time_to_eat;
	long	time_to_sleep;
	int		must_eat;
	long	start_time;
	int		stop;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	state_mutex;
	t_philo	*philos;
};

int	parse_args(int argc, char **argv, t_table *table);
long	get_time_ms(void);
long	timestamp(t_table *table);
void	precise_sleep(t_table *table, long duration_ms);
int		init_table(t_table *table);
void	cleanup_table(t_table *table);
int		start_simulation(t_table *table);
void	*philo_routine(void *arg);
void	print_status(t_philo *philo, char *status);
int		simulation_stopped(t_table *table);
void	stop_simulation(t_table *table);
void	monitor_simulation(t_table *table);

#endif
