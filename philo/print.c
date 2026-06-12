/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomamart <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/12 12:37:54 by tomamart          #+#    #+#             */
/*   Updated: 2026/06/12 12:37:56 by tomamart         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	append_long(char *buffer, long nbr, int i)
{
	char	digits[20];
	int		j;

	if (nbr == 0)
	{
		buffer[i] = '0';
		return (i + 1);
	}
	j = 0;
	while (nbr > 0)
	{
		digits[j++] = (nbr % 10) + '0';
		nbr /= 10;
	}
	while (j > 0)
		buffer[i++] = digits[--j];
	return (i);
}

static int	append_str(char *buffer, char *str, int i)
{
	int	j;

	j = 0;
	while (str[j] != '\0')
		buffer[i++] = str[j++];
	return (i);
}

void	print_status(t_philo *philo, char *status)
{
	char	buffer[128];
	int		i;

	if (!simulation_stopped(philo->table) || status[0] == 'd')
	{
		i = 0;
		i = append_long(buffer, timestamp(philo->table), i);
		buffer[i++] = ' ';
		i = append_long(buffer, philo->id, i);
		buffer[i++] = ' ';
		i = append_str(buffer, status, i);
		buffer[i++] = '\n';
		write(1, buffer, i);
	}
}
