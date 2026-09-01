# minitalk

## Description

**minitalk** is a small client–server communication project that uses only **UNIX signals** (`SIGUSR1` and `SIGUSR2`) to transfer data between processes.

The goal of the project is to:

- Implement a **server** that receives a string sent by a **client**.
- Encode the message at the client side **bit by bit** into signals.
- Reconstruct the message on the server side and **print the full string** once it is completely received.
- (Bonus) Implement an **acknowledgement (ACK)** mechanism so the server notifies the client when the whole message has been received and printed.

The project aims to deepen understanding of:

- UNIX signals and asynchronous event handling.
- Inter-process communication (IPC) without using sockets or pipes.
- Bitwise operations and character encoding.
- Robust error handling and signal-safe design.
- Respecting the 42 Norm (coding style, globals, Makefile rules, etc.).

---

## Dependency Management

The maintained portfolio version uses
[42Libft](https://github.com/LuisQAlmeida/42Libft) as an external Git
dependency instead of keeping a duplicated Libft source tree inside this
repository.

The dependency is stored as a Git submodule at:

    external/libft

Minitalk is pinned to the exact Libft revision:

    890089c0d12a29874e3a92facd92f9f455d1ff1c

This means a given Minitalk commit resolves to a specific Libft source state
rather than implicitly following the latest remote `main` branch.

The Minitalk build uses Libft through its public boundary:

- `external/libft/libft/libft.h`;
- `external/libft/libft/libft.a`;
- Libft's own `Makefile`.

Minitalk does not depend on individual Libft implementation files.

### Clone with dependencies

The recommended clone command is:

    git clone --recurse-submodules https://github.com/LuisQAlmeida/42minitalk.git

For an existing clone whose submodule has not yet been initialized:

    git submodule update --init --recursive

If Libft is missing, the Minitalk Makefile stops before compilation and prints
the initialization command rather than failing later on a missing header.

Once initialized, normal builds are local and do not require fetching a moving
dependency revision.

### Historical project state

The original project bundled its Libft implementation directly inside the
Minitalk repository.

That exact pre-modernization state remains preserved at:

    portfolio-baseline-2026-09

External dependency management is post-baseline portfolio maintenance and does
not rewrite the historical submission.

## Instructions

### 1. Repository structure

```text
minitalk/
├── Makefile
├── include/
│   ├── minitalk.h          # Mandatory header
│   └── minitalk_bonus.h    # Bonus header
├── src/
│   ├── client.c            # Mandatory client
│   ├── server.c            # Mandatory server
│   └── utils.c             # Error handling (ft_error)
├── bonus/
│   ├── client_bonus.c      # Bonus client (ACK support)
│   └── server_bonus.c      # Bonus server (ACK support)
└── libft/
    ├── libft.a             # Static library (built by its own Makefile)
    └── ...
```

The project depends on a working `libft` in the `libft/` directory, with its own `Makefile` producing `libft.a`.

---

### 2. Compilation

From inside the `minitalk/` directory:

#### Mandatory part

```bash
make
```

This will:

- Build `libft` using `make -C libft`.
- Compile and link:
  - `src/server.c` + `src/utils.c` + `libft.a` → `server`
  - `src/client.c` + `src/utils.c` + `libft.a` → `client`

Compilation flags:

```text
cc -Wall -Wextra -Werror
```

#### Bonus part

```bash
make bonus
```

This recompiles `server` and `client` using the bonus sources:

- `bonus/server_bonus.c` + `src/utils.c` + `libft.a` → `server`
- `bonus/client_bonus.c` + `src/utils.c` + `libft.a` → `client`

#### Cleaning

```bash
make clean     # Remove object files (mandatory + bonus)
make fclean    # clean + remove server/client and libft.a
make re        # fclean + make
```

---

### 3. Usage

#### Step 1 – Start the server

```bash
./server
```

The server prints its PID and waits for incoming signals:

```text
Server PID is 12345
```

#### Step 2 – Run the client

In another terminal:

```bash
./client <SERVER_PID> "<message>"
```

Examples:

```bash
./client 12345 "Hello, world!"
./client 12345 "42 Porto - minitalk"
./client 12345 ""
```

**Mandatory behavior:**

- The client:
  - Validates the PID.
  - Encodes each character of the message into bits and sends them as signals:
    - `SIGUSR1` → bit `0`
    - `SIGUSR2` → bit `1`
  - Sends an additional `\0` character to mark the **end of the message**.
- The server:
  - Reconstructs characters bit by bit in a global state structure.
  - Builds the message in a dynamically allocated buffer in the main loop.
  - Prints the **entire message at once** when the terminating `\0` is received, then prints a newline.

#### Bonus behavior (ACK)

With the bonus compiled (`make bonus`):

- The **server** uses `sigaction` with `SA_SIGINFO` to retrieve the client PID.
- After receiving and printing the **full message**, the server sends `SIGUSR1` back to the client as an ACK.
- The **bonus client**:
  - Installs a handler for `SIGUSR1`.
  - Sends the message as in the mandatory part.
  - Waits in a loop using `pause()` until it receives the ACK.
  - Prints a confirmation, e.g. `Message delivered to server.`

Usage is the same:

```bash
./server
./client <SERVER_PID> "Hello from bonus"
```

---

### 4. Memory checks

To check for memory leaks on Linux, you can use `valgrind`:

```bash
make fclean
make CFLAGS="-Wall -Wextra -Werror -g"

valgrind --leak-check=full --show-leak-kinds=all ./server
# In another terminal:
./client <PID> "Test message"
./client <PID> ""
./client <PID> "Another test"
```

The goal is to have:

```text
definitely lost: 0 bytes
indirectly lost: 0 bytes
possibly lost:   0 bytes
```

---

## Resources

### Documentation & references

- `man 2 kill` – send signals to processes.
- `man 2 sigaction` – examine and change a signal action.
- `man 7 signal` – overview of signals.
- `man 2 pause` – wait for a signal.
- 42 subject PDF for **minitalk**.
- 42 **libft** project subject for the custom library used in this project.

These references cover:

- How to send and handle signals (`SIGUSR1`, `SIGUSR2`).
- How to safely use `sigaction` (including `SA_SIGINFO`).
- How to correctly use blocking calls like `pause()` in an event-driven context.
- How to implement and reuse your own C library (`libft`).

### Use of AI in this project

AI (ChatGPT from OpenAI) was used in the following ways:

- **Concept clarification**
  - Understanding the behavior and differences between `signal` and `sigaction`.
  - Reviewing how to correctly interpret `siginfo_t` and `SA_SIGINFO`.
  - Discussing safe patterns for using global state with signal handlers.

- **Design and structure**
  - Refining the overall architecture of the client–server protocol using signals.
  - Structuring the use of a single global `t_server` per server program and a single global `t_client` for the bonus client.

- **README drafting**
  - Helping draft and organize this `README.md` according to the subject’s rules:
    - Required sections (“Description”, “Instructions”, “Resources”).
    - Clear explanation of usage and behavior for both mandatory and bonus parts.
    - Explicit documentation of how AI was used.

All final code was written, read, and understood by the author.
AI was used as a tool for explanation, feedback, and drafting, not as a replacement for understanding or decision-making.
