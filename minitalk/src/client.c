#include "minitalk.h"

static int	valid_pid_format(const char *srv_pid)
{
	int	i;

	i = 0;
	if (!srv_pid || !srv_pid[i])
		return (0);
	while (srv_pid[i] != '\0')
	{
		if (!ft_isdigit((unsigned char)srv_pid[i]))
			return (0);
		i++;
	}
	return (1);
}

static void	send_char(pid_t srv_pid, unsigned char c)
{
	int	bit_idx;

	bit_idx = 7;
	while (bit_idx >= 0)
	{
		if ((c >> bit_idx) & 1)
		{
			if (kill(srv_pid, SIGUSR2) == -1)
				ft_error("Error: Failed To Send Signal.", 3);
		}
		else
		{
			if (kill(srv_pid, SIGUSR1) == -1)
				ft_error("Error: Failed To Send Signal.", 3);
		}
		usleep(100);
		bit_idx--;
	}
}

static void	send_msg(pid_t srv_pid, const char *msg)
{
	int	i;

	i = 0;
	if (!msg)
		return ;
	while (msg[i] != '\0')
	{
		send_char(srv_pid, (unsigned char)msg[i]);
		i++;
	}
	send_char(srv_pid, '\0');
}

int	main(int argc, char **argv)
{
	pid_t	server_pid;

	if (argc != 3)
		ft_error("Please type: ./client <Server PID> <Message>", 1);
	if (!valid_pid_format(argv[1]))
		ft_error("Error: Invalid Server PID format.", 2);
	server_pid = (pid_t)ft_atoi(argv[1]);
	if (server_pid <= 0)
		ft_error("Error: Invalid Server PID format.", 2);
	send_msg(server_pid, argv[2]);
	return (0);
}
