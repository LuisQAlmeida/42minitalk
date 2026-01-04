#include "minitalk.h"

static t_server	g_srv;

static void	signal_handler(int sig_nbr)
{
	if (sig_nbr == SIGUSR2)
		g_srv.full_char |= (1 << (7 - g_srv.bit_idx));
	g_srv.bit_idx++;
	if (g_srv.bit_idx == 8)
	{
		if (g_srv.full_char == '\0')
			write(1, "\n", 1);
		else
			write(1, &g_srv.full_char, 1);
		g_srv.bit_idx = 0;
		g_srv.full_char = 0;
	}
}

int	main(void)
{
	struct sigaction	sigact;

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
		pause();
	return (0);
}
