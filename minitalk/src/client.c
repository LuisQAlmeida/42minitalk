#include "minitalk.h"

int	main(int argc, char **argv)
{
	int	i;

	i = 0;
	if (argc != 3)
	{
		write(2, "Please type ./client <Server PID> <The message you want to send to server>\n", 75);
		return (1);
	}
	while (argv[2][i])
	{
		write(1, &argv[2][i], 1);
		i++;
	}
	return (0);
}
