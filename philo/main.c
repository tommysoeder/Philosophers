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
