#include "minitalk_bonus.h"

static t_client	g_clt;

static void	ack_handler(int sig)
{
	if (sig == SIGUSR2)
		g_clt.bit_ack_received = 1;
	else if (sig == SIGUSR1)
		g_clt.ack_received = 1;
}

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

static void	send_char(pid_t srv_pid, unsigned char c,
		const sigset_t *wait_mask)
{
	int	bit_idx;
	int	sig;

	bit_idx = 7;
	while (bit_idx >= 0)
	{
		if ((c >> bit_idx) & 1)
			sig = SIGUSR2;
		else
			sig = SIGUSR1;
		g_clt.bit_ack_received = 0;
		if (kill(srv_pid, sig) == -1)
			ft_error("Error: Failed To Send Signal.", 3);
		while (!g_clt.bit_ack_received)
			sigsuspend(wait_mask);
		bit_idx--;
	}
}

static void	send_msg(pid_t srv_pid, const char *msg,
		const sigset_t *wait_mask)
{
	int	i;

	i = 0;
	if (!msg)
		return ;
	while (msg[i] != '\0')
	{
		send_char(srv_pid, (unsigned char)msg[i], wait_mask);
		i++;
	}
	send_char(srv_pid, '\0', wait_mask);
}

int	main(int argc, char **argv)
{
	pid_t				server_pid;
	struct sigaction	sigact;
	sigset_t			block_mask;
	sigset_t			old_mask;
	sigset_t			bit_wait_mask;
	sigset_t			ack_wait_mask;

	if (argc != 3)
		ft_error("Please type: ./client <Server PID> <Message>", 1);
	if (!valid_pid_format(argv[1]))
		ft_error("Error: Invalid Server PID format.", 2);
	server_pid = (pid_t)ft_atoi(argv[1]);
	if (server_pid <= 0)
		ft_error("Error: Invalid Server PID format.", 2);
	g_clt.ack_received = 0;
	sigemptyset(&block_mask);
	sigaddset(&block_mask, SIGUSR1);
	sigaddset(&block_mask, SIGUSR2);
	if (sigprocmask(SIG_BLOCK, &block_mask, &old_mask) == -1)
		ft_error("Error: Signal Mask Failed.", 4);
	bit_wait_mask = old_mask;
	sigaddset(&bit_wait_mask, SIGUSR1);
	sigdelset(&bit_wait_mask, SIGUSR2);
	ack_wait_mask = old_mask;
	sigaddset(&ack_wait_mask, SIGUSR2);
	sigdelset(&ack_wait_mask, SIGUSR1);
	sigemptyset(&sigact.sa_mask);
	sigact.sa_flags = 0;
	sigact.sa_handler = ack_handler;
	if (sigaction(SIGUSR1, &sigact, NULL) == -1
		|| sigaction(SIGUSR2, &sigact, NULL) == -1)
		ft_error("Error: Sigaction Failed.", 4);
	send_msg(server_pid, argv[2], &bit_wait_mask);
	while (!g_clt.ack_received)
		sigsuspend(&ack_wait_mask);
	if (sigprocmask(SIG_SETMASK, &old_mask, NULL) == -1)
		ft_error("Error: Signal Mask Failed.", 4);
	ft_printf("Message delivered to server.\n");
	return (0);
}
