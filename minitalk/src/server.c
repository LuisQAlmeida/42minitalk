/* CHECKLIST:

- Display its PID: use getpid() function and maybe ft_putnbr to display PID.
- Create an endless loop so that the server can receive signals at any time;
- Receive signals;
- Decrypt signals;
	- For each signal received (SIGUSR1 and SIGUSR2) it should take a certain action.
- Error handling and memory leaks handling

*/

#include "../includes/Libft/libft.h"
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
	printf("\nServer's PID is %d\n\n", server_pid); //Replace with ft_putnbr. Include Libft library.
	return (0);
}
