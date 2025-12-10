#include "minitalk.h"

int	valid_pid_format_(const char *srv_pid)
{
	int	i;

	i = 0;
	if (!srv_pid || !srv_pid[i])
		return (0);
	while (srv_pid[i] != '\0')
	{
		if (!is_digit(srv_pid[i]))
			return (0);
		i++;
	}
	return (1);
}

int	main(int argc, char **argv)
{
	pid_t	server_pid;

	if (argc != 3)
		ft_error("Please type: ./client <Server PID> <Message>", 1);
	if (!valid_pid_format)
		ft_error("Error: Invalid Server PID format.", 2);
	server_pid = (pid_t)ft_atoi(argv[1]);
	if (server_pid <= 0)
		ft_error("Error: Invalid Server PID value.", 3);
	/* ------> Function to send message to Server PID here. <------- */
	return (0);
}
