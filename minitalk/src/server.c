#include "minitalk.h"

int	main(void)
{
	ft_printf("Server PID is %d\n", getpid());

	while (1)
		pause();
	return (0);
}
