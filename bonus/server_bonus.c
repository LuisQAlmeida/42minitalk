#include "minitalk_bonus.h"

static t_server	g_srv;

static void	signal_handler(int sig_nbr, siginfo_t *info, void *context)
{
	(void)context;
	g_srv.signal_nbr = sig_nbr;
	if (info)
		g_srv.client_pid = (sig_atomic_t)info->si_pid;
	g_srv.signal_ready = 1;
}

static char	*message_build(char *msg, size_t msg_len)
{
	char	*tmp;
	size_t	i;

	tmp = malloc(msg_len + 2);
	if (!tmp)
		ft_error("Error: Malloc Failed.", 6);
	i = 0;
	while (i < msg_len)
	{
		tmp[i] = msg[i];
		i++;
	}
	if (msg)
		free(msg);
	return (tmp);
}

static void	message_store(char **msg, size_t *msg_len,
		unsigned char c, pid_t client_pid)
{
	char	*tmp;

	tmp = message_build(*msg, *msg_len);
	tmp[*msg_len] = (char)c;
	tmp[*msg_len + 1] = '\0';
	*msg = tmp;
	if (c == '\0')
	{
		if (*msg_len > 0)
			write(1, *msg, *msg_len);
		write(1, "\n", 1);
		free(*msg);
		*msg = NULL;
		*msg_len = 0;
		if (client_pid > 0 && kill(client_pid, SIGUSR1) == -1)
			write(2, "Error: ACK Failed.\n", 19);
	}
	else
		(*msg_len)++;
}

static void	bit_consume(char **msg, size_t *msg_len, pid_t client_pid)
{
	unsigned char	complete_char;

	if (g_srv.signal_nbr == SIGUSR2)
		g_srv.full_char |= (1 << (7 - g_srv.bit_idx));
	g_srv.bit_idx++;
	if (g_srv.bit_idx == 8)
	{
		complete_char = g_srv.full_char;
		g_srv.bit_idx = 0;
		g_srv.full_char = 0;
		message_store(msg, msg_len, complete_char, client_pid);
	}
}

int	main(void)
{
	char				*msg;
	size_t				msg_len;
	pid_t				client_pid;
	struct sigaction	sigact;
	sigset_t			block_mask;
	sigset_t			old_mask;
	sigset_t			wait_mask;

	msg = NULL;
	msg_len = 0;
	sigemptyset(&block_mask);
	sigaddset(&block_mask, SIGUSR1);
	sigaddset(&block_mask, SIGUSR2);
	if (sigprocmask(SIG_BLOCK, &block_mask, &old_mask) == -1)
		ft_error("Error: Signal Mask Failed.", 4);
	wait_mask = old_mask;
	sigdelset(&wait_mask, SIGUSR1);
	sigdelset(&wait_mask, SIGUSR2);
	sigemptyset(&sigact.sa_mask);
	sigaddset(&sigact.sa_mask, SIGUSR1);
	sigaddset(&sigact.sa_mask, SIGUSR2);
	sigact.sa_flags = SA_SIGINFO;
	sigact.sa_sigaction = signal_handler;
	if (sigaction(SIGUSR1, &sigact, NULL) == -1
		|| sigaction(SIGUSR2, &sigact, NULL) == -1)
		ft_error("Error: Sigaction Failed.", 4);
	ft_printf("Server PID is %d\n", getpid());
	while (1)
	{
		while (!g_srv.signal_ready)
			sigsuspend(&wait_mask);
		client_pid = (pid_t)g_srv.client_pid;
		bit_consume(&msg, &msg_len, client_pid);
		g_srv.signal_ready = 0;
		if (client_pid > 0 && kill(client_pid, SIGUSR2) == -1)
			write(2, "Error: Bit ACK Failed.\n", 23);
	}
	return (0);
}
