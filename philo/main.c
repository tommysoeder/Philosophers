/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomamart <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/12 12:38:04 by tomamart          #+#    #+#             */
/*   Updated: 2026/06/12 12:38:06 by tomamart         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{
	t_table	table;

	if (parse_args(argc, argv, &table) != 0)
		return (1);
	if (init_table(&table) != 0)
		return (1);
	if (start_simulation(&table) != 0)
	{
		cleanup_table(&table);
		return (1);
	}
	cleanup_table(&table);
	return (0);
}
