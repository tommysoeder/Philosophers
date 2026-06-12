/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomamart <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/12 12:36:34 by tomamart          #+#    #+#             */
/*   Updated: 2026/06/12 12:36:36 by tomamart         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	init_mutexes(t_table *table)
{
	int	i;

	if (pthread_mutex_init(&table->print_mutex, NULL) != 0)
		return (1);
	if (pthread_mutex_init(&table->state_mutex, NULL) != 0)
	{
		pthread_mutex_destroy(&table->print_mutex);
		return (1);
	}
	i = 0;
	while (i < table->philo_count)
	{
		if (pthread_mutex_init(&table->forks[i], NULL) != 0)
		{
			while (i > 0)
			{
				i--;
				pthread_mutex_destroy(&table->forks[i]);
			}
			pthread_mutex_destroy(&table->state_mutex);
			pthread_mutex_destroy(&table->print_mutex);
			return (1);
		}
		i++;
	}
	return (0);
}

static void	init_philos(t_table *table)
{
	int	i;
	int	right_fork;

	i = 0;
	while (i < table->philo_count)
	{
		table->philos[i].id = i + 1;
		table->philos[i].meals_eaten = 0;
		table->philos[i].is_eating = 0;
		table->philos[i].last_meal = 0;
		table->philos[i].left_fork = &table->forks[i];
		right_fork = (i + 1) % table->philo_count;
		table->philos[i].right_fork = &table->forks[right_fork];
		table->philos[i].table = table;
		i++;
	}
}

int	init_table(t_table *table)
{
	table->stop = 0;
	table->forks = malloc(sizeof(pthread_mutex_t) * table->philo_count);
	if (table->forks == NULL)
		return (1);
	table->philos = malloc(sizeof(t_philo) * table->philo_count);
	if (table->philos == NULL)
	{
		free(table->forks);
		return (1);
	}
	if (init_mutexes(table) != 0)
	{
		free(table->philos);
		free(table->forks);
		return (1);
	}
	init_philos(table);
	return (0);
}
