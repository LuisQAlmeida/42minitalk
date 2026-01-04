#ifndef MINITALK_H
# define MINITALK_H

# include "libft.h"
# include <signal.h>
# include <unistd.h>
# include <stdlib.h>

typedef struct s_server
{
	int				bit_idx;
	unsigned char	full_char;
}	t_server;

typedef struct s_client
{
	volatile sig_atomic_t	ack_received;
}	t_client;

void	ft_error(const char *error_msg, int error_nbr);

#endif
