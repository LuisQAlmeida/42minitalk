#include "minitalk.h"

static t_server	g_srv;

static void	signal_handler(int sig_nbr, siginfo_t *info, void *context)
{
	(void)context;
	if (sig_nbr == SIGUSR2)
		g_srv.full_char |= (1 << (7 - g_srv.bit_idx));
	g_srv.bit_idx++;
	if (g_srv.bit_idx == 8)
	{
		if (g_srv.full_char == '\0')
		{
			write(1, "\n", 1);
			if (info && info->si_pid != 0)
			{
				if (kill(info->si_pid, SIGUSR1) == -1)
					write(2, "Error: ACK Failed\n", 18);
			}
		}
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
	sigact.sa_flags = SA_SIGINFO;
	sigact.sa_sigaction = signal_handler;
	ft_printf("Server PID is %d\n", getpid());
	if (sigaction(SIGUSR1, &sigact, NULL) == -1
		|| sigaction(SIGUSR2, &sigact, NULL) == -1)
		ft_error("Error: Sigaction Failed.", 5);
	while (1)
		pause();
	return (0);
}
