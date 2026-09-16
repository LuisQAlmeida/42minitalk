#ifndef MINITALK_BONUS_H
# define MINITALK_BONUS_H

# include "libft.h"
# include <signal.h>
# include <unistd.h>
# include <stdlib.h>

/**
 * @file minitalk_bonus.h
 * @brief Shared state and utility interface for bonus Minitalk.
 *
 * The bonus implementation retains the mandatory per-bit synchronization and
 * adds a second acknowledgement level for complete-message delivery.
 *
 * SIGUSR2 is used by the server to acknowledge each received bit.
 *
 * After the terminating null byte has been reconstructed and the complete
 * message has been printed, the bonus server sends SIGUSR1 to acknowledge
 * final delivery.
 *
 * State written by signal handlers and observed by normal execution uses
 * volatile sig_atomic_t. These fields provide simple scalar communication
 * between asynchronous handlers and the main control flow; they do not make
 * compound multi-field operations atomic.
 */

/**
 * @brief Server-side state used to reconstruct the incoming signal stream.
 *
 * The same transport state is used by mandatory and bonus server operation.
 * Bonus behaviour additionally sends a final SIGUSR1 acknowledgement when the
 * terminating null byte completes a message.
 */
typedef struct s_server
{
	/**
	 * @brief Number of bits already consumed for the current byte.
	 *
	 * Reset to 0 whenever eight received signals have reconstructed one byte.
	 */
	int						bit_idx;

	/**
	 * @brief Accumulator for the byte currently being reconstructed.
	 */
	unsigned char			full_char;

	/**
	 * @brief Most recently received transport signal.
	 */
	volatile sig_atomic_t	signal_nbr;

	/**
	 * @brief PID of the client that sent the most recently handled bit.
	 *
	 * The server uses this value for per-bit acknowledgements and for the
	 * bonus complete-message acknowledgement.
	 */
	volatile sig_atomic_t	client_pid;

	/**
	 * @brief Indicates that received signal state is ready for consumption.
	 *
	 * Set inside the signal handler and cleared after the main server flow has
	 * consumed the pending bit.
	 */
	volatile sig_atomic_t	signal_ready;
}	t_server;

/**
 * @brief Bonus client-side acknowledgement state.
 *
 * The bonus client distinguishes transport synchronization from confirmation
 * that the complete message has been delivered.
 */
typedef struct s_client
{
	/**
	 * @brief Indicates receipt of the SIGUSR2 acknowledgement for one bit.
	 *
	 * Cleared before each bit transmission and set by the client signal
	 * handler after the server acknowledges that bit.
	 */
	volatile sig_atomic_t	bit_ack_received;

	/**
	 * @brief Indicates receipt of the final SIGUSR1 delivery acknowledgement.
	 *
	 * The client waits for this flag after transmitting the complete message,
	 * including its terminating null byte.
	 */
	volatile sig_atomic_t	ack_received;
}	t_client;

/**
 * @brief Reports a fatal error and terminates the process.
 *
 * If error_msg is non-NULL, the function writes it to standard error. It then
 * writes a newline and terminates the process using error_nbr as the exit
 * status.
 *
 * @param error_msg Error message to write, or NULL.
 * @param error_nbr Process exit status.
 */
void	ft_error(const char *error_msg, int error_nbr);

#endif
