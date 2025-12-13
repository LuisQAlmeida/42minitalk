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

void	ft_error(const char *error_msg, int error_nbr);

#endif
