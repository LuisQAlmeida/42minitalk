#include "minitalk.h"

static t_server	g_srv;

static void	signal_handler(int sig_nbr)
{
	if (sig_nbr == SIGUSR2)
		g_srv.full_char |= (1 << (7 - g_srv.bit_idx));
	g_srv.bit_idx++;
	if (g_srv.bit_idx == 8)
	{
		g_srv.full_char_ready = 1;
		g_srv.bit_idx = 0;
	}
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

static void	message_print(char **msg, size_t *msg_len)
{
	if (*msg_len > 0)
		write(1, *msg, *msg_len);
	write(1, "\n", 1);
	free(*msg);
	*msg = NULL;
	*msg_len = 0;
}

static void	message_handler(char **msg, size_t *msg_len)
{
	char	*tmp;

	tmp = message_build(*msg, *msg_len);
	tmp[*msg_len] = (char)g_srv.full_char;
	tmp[*msg_len + 1] = '\0';
	*msg = tmp;
	if (g_srv.full_char == '\0')
		message_print(msg, msg_len);
	else
		(*msg_len)++;
	g_srv.full_char = 0;
	g_srv.full_char_ready = 0;
}

int	main(void)
{
	char				*msg;
	size_t				msg_len;
	struct sigaction	sigact;

	msg = NULL;
	msg_len = 0;
	sigemptyset(&sigact.sa_mask);
	sigaddset(&sigact.sa_mask, SIGUSR1);
	sigaddset(&sigact.sa_mask, SIGUSR2);
	sigact.sa_flags = 0;
	sigact.sa_handler = signal_handler;
	ft_printf("Server PID is %d\n", getpid());
	if (sigaction(SIGUSR1, &sigact, NULL) == -1
		|| sigaction(SIGUSR2, &sigact, NULL) == -1)
		ft_error("Error: Sigaction Failed.", 4);
	while (1)
	{
		pause();
		if (g_srv.full_char_ready)
			message_handler(&msg, &msg_len);
	}
	return (0);
}
