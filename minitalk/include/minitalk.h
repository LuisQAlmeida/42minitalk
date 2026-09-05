#ifndef MINITALK_H
# define MINITALK_H

# include "libft.h"
# include <signal.h>
# include <unistd.h>
# include <stdlib.h>

/**
 * @file minitalk.h
 * @brief Shared state and utility interface for mandatory Minitalk.
 *
 * The mandatory implementation transfers each message byte from the client to
 * the server one bit at a time using SIGUSR1 for bit 0 and SIGUSR2 for bit 1.
 *
 * After consuming each signal, the server acknowledges that bit with SIGUSR2.
 * The client waits for that acknowledgement before sending the next bit.
 *
 * State written by signal handlers and observed by normal execution uses
 * volatile sig_atomic_t. These fields are used for simple scalar communication
 * between asynchronous handlers and the main control flow; this does not make
 * compound multi-field operations atomic.
 */

/**
 * @brief Server-side state used to reconstruct the incoming signal stream.
 *
 * The signal handler records the received signal, originating client PID, and
 * readiness flag. The main server loop consumes that state and reconstructs
 * bytes outside the handler.
 */
typedef struct s_server
{
	/**
	 * @brief Number of bits already consumed for the current byte.
	 *
	 * The value advances from 0 to 8 and is reset to 0 after a complete byte
	 * has been reconstructed.
	 */
	int						bit_idx;

	/**
	 * @brief Accumulator used to reconstruct the current message byte.
	 *
	 * SIGUSR2 sets the corresponding bit while SIGUSR1 leaves it cleared.
	 * The value is reset after eight bits have been consumed.
	 */
	unsigned char			full_char;

	/**
	 * @brief Most recently received transport signal.
	 *
	 * Written by the server signal handler and inspected by the main server
	 * flow to determine the next reconstructed bit value.
	 */
	volatile sig_atomic_t	signal_nbr;

	/**
	 * @brief PID of the client associated with the most recently handled bit.
	 *
	 * Captured from siginfo_t by the server signal handler and later used by
	 * the main server flow when sending the per-bit acknowledgement.
	 */
	volatile sig_atomic_t	client_pid;

	/**
	 * @brief Indicates that a received signal is ready for main-loop handling.
	 *
	 * Set by the signal handler and cleared by the server after the pending bit
	 * has been consumed.
	 */
	volatile sig_atomic_t	signal_ready;
}	t_server;

/**
 * @brief Mandatory client-side acknowledgement state.
 */
typedef struct s_client
{
	/**
	 * @brief Indicates receipt of the server acknowledgement for one bit.
	 *
	 * The client clears this flag before transmitting each bit. Its SIGUSR2
	 * handler sets the flag, allowing the waiting client flow to continue with
	 * the next bit.
	 */
	volatile sig_atomic_t	bit_ack_received;
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
