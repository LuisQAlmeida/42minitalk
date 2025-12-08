#include "minitalk.h"

int	main(void)
{
	struct sigaction	sigact;

	sigemptyset(&sigact.sa_mask);
	sigact.sa_flags = SA_SIGINFO;
	sigact.sa_sigaction = signal_handler_function;
	ft_printf("Server PID is %d\n", getpid());
	while (1)
		pause();
	return (0);
}
