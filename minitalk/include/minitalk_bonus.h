#ifndef MINITALK_BONUS_H
# define MINITALK_BONUS_H

# include "libft.h"
# include <signal.h>
# include <unistd.h>
# include <stdlib.h>

typedef struct s_server
{
	int						bit_idx;
	pid_t					client_pid;
	unsigned char			full_char;
	volatile sig_atomic_t	full_char_ready;
}	t_server;

typedef struct s_client
{
	volatile sig_atomic_t	ack_received;
}	t_client;

void	ft_error(const char *error_msg, int error_nbr);

#endif
