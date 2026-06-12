#include "philo.h"

static int	is_digit(char c)
{
	return (c >= '0' && c <= '9');
}

static int	parse_positive_long(char *str, long *out)
{
	long	nbr;
	int		digit;
	int		i;

	if (str == NULL || str[0] == '\0')
		return (1);
	nbr = 0;
	i = 0;
	while (str[i] != '\0')
	{
		if (!is_digit(str[i]))
			return (1);
		digit = str[i] - '0';
		if (nbr > (INT_MAX - digit) / 10)
			return (1);
		nbr = nbr * 10 + digit;
		i++;
	}
	if (nbr <= 0)
		return (1);
	*out = nbr;
	return (0);
}

static int	print_usage(void)
{
	printf("Usage: ./philo number_of_philosophers time_to_die ");
	printf("time_to_eat time_to_sleep ");
	printf("[number_of_times_each_philosopher_must_eat]\n");
	return (1);
}

int	parse_args(int argc, char **argv, t_table *table)
{
	long	value;

	if (argc != 5 && argc != 6)
		return (print_usage());
	if (parse_positive_long(argv[1], &value) != 0)
		return (print_usage());
	table->philo_count = (int)value;
	if (parse_positive_long(argv[2], &table->time_to_die) != 0)
		return (print_usage());
	if (parse_positive_long(argv[3], &table->time_to_eat) != 0)
		return (print_usage());
	if (parse_positive_long(argv[4], &table->time_to_sleep) != 0)
		return (print_usage());
	table->must_eat = -1;
	if (argc == 6)
	{
		if (parse_positive_long(argv[5], &value) != 0)
			return (print_usage());
		table->must_eat = (int)value;
	}
	return (0);
}
