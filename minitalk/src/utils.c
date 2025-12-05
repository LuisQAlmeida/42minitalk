#include "minitalk.h"

void	ft_error(const char *error_msg, int error_nbr)
{
	if (msg)
		write(2, &msg, ft_strlen(msg));
	write(2, "\n", 1);
	exit(error_nbr)
}
