#include "minitalk.h"

int	main(int argc, char **argv)
{
	if (argc != 3)
		ft_error("Please type: ./client <Server PID> <Message>", 1);
	return (0);
}
