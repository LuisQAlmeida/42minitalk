#include "minitalk.h"

int	main(int argc, char **argv)
{
	pid_t	server_pid;

	if (argc != 3)
		ft_error("Please type: ./client <Server PID> <Message>", 1);
	server_pid = (pid_t)ft_atoi(argv[1]);
	return (0);
}
