#include "minitalk.h"

void	ft_error(const char *error_msg, int error_nbr)
{
	if (error_msg)
		ft_putstr_fd((char *)error_msg, 2);
	ft_putstr_fd("\n", 2);
	exit(error_nbr);
}
