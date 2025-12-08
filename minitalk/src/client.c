#include "minitalk.h"

int	main(int argc, char **argv)
{
	int	i;

	i = 0;
	if (argc != 3)
		ft_error("Please type: ./client <Server PID> <Message>\n", 1);
	while (argv[2][i])
	{
		write(1, &argv[2][i], 1);
		i++;
	}
	return (0);
}
