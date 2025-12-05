#include "minitalk.h"

int	main(void)
{
	pid_t	server_pid;

	server_pid = getpid();
	while (1)
	{
		//Find a way to receive each signal.
		//Find a way to Decrypt signals.
	}
	ft_printf("\nServer's PID is %d\n\n", server_pid);
	return (0);
}
