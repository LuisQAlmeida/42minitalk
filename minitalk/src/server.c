#include "minitalk.h"

static void	signal_handler(int sig_nbr, siginfo_t *info, void *context)
{
	static int				bit_idx;
	static unsigned char	full_char;

	bit_idx = 0;
	full_char = 0;
	(void)context;
	if (sig_nbr == SIGUSR2)
		full_char |= (1 << (7 - bit_idx));
	bit_idx++;
	if (bit_idx == 8)
	{
		if (full_char == 0)
		{
			write(1, "\n", 1);
			if (info && info->si_pid != 0)
			{
				if (kill(info->si_pid, SIGUSR1) == -1)
					write(2, "Error: ACK Failed\n", 18);
			}
		}
		else
			write(1, &full_char, 1);
		bit_idx = 0;
		full_char = 0;
	}
}

int	main(void)
{
	struct sigaction	sigact;

	sigemptyset(&sigact.sa_mask);
	sigact.sa_flags = SA_SIGINFO;
	sigact.sa_sigaction = signal_handler;
	ft_printf("Server PID is %d\n", getpid());
	if (sigaction(SIGUSR1, &sigact, NULL) == -1
		|| sigaction(SIGUSR2, &sigact, NULL) == -1)
		write(2, "Error: Sigaction Failed\n", 24);
	while (1)
		pause();
	return (0);
}
