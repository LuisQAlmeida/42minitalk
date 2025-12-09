#include "minitalk.h"

static void	signal_handler(int sig_nbr)
{
	static int				bit_idx;
	static unsigned char	full_char;

	bit_idx = 0;
	full_char = 0;
	if (sig_nbr == SIGUSR2)
		full_char |= (1 << (7 - bit_idx));
	bit_idx++;
	if (bit_idx == 8)
	{
		if (full_char == 0)
			write(1, "\n", 1);
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
	sigact.sa_flags = 0;
	sigact.sa_handler = signal_handler;
	ft_printf("Server PID is %d\n", getpid());
	if (sigaction(SIGUSR1, &sigact, NULL) == -1
		|| sigaction(SIGUSR2, &sigact, NULL) == -1)
		ft_error("Error: Sigaction Failed\n", 3);
	while (1)
		pause();
	return (0);
}
