/*CHECKLIST:

- Write a program (main) in which the client takes two parameters/arguments
	- The PID of the server to which it wants to send the message;
	- The message;
- Encrypt the message;
	- bitwise operations to encrypt and decrypt message??
- Send the message to the server (via its PID);
	- Maybe use kill function to send the signals to server.
- Create a stop condition so that the server knows when it has finished receiving the message;
	- Maybe the stop condition is signal SIGUSR2 and SIGUSR1 send each char.
- Error handling and memory leaks handling.

*/

#include "libft.h"
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
