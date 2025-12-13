#******************************************************#
#			VARIABLES                      #
#******************************************************#
CC = cc
CFLAGS = -Wall -Wextra -Werror

SERVER_NAME = server
CLIENT_NAME = client

SERVER_SRC = src/server.c src/utils.c
SERVER_SRC_BONUS = bonus/server_bonus.c src/utils.c
CLIENT_SRC = src/client.c src/utils.c
CLIENT_SRC_BONUS = bonus/client_bonus.c src/utils.c

SERVER_SRC_USED = $(SERVER_SRC)
CLIENT_SRC_USED = $(CLIENT_SRC)

ifeq ($(BONUS), 1)
SERVER_SRC_USED = $(SERVER_SRC_BONUS)
CLIENT_SRC_USED = $(CLIENT_SRC_BONUS)
endif

SERVER_OBJ_USED = $(SERVER_SRC_USED:.c=.o)
CLIENT_OBJ_USED = $(CLIENT_SRC_USED:.c=.o)

SERVER_OBJ_ALL = $(SERVER_SRC:.c=.o) $(SERVER_SRC_BONUS:.c=.o)
CLIENT_OBJ_ALL = $(CLIENT_SRC:.c=.o) $(CLIENT_SRC_BONUS:.c=.o)

INCLUDE = -Iinclude -Iinclude/Libft

LIBFT_DIR = include/Libft
LIBFT = $(LIBFT_DIR)/libft.a

#******************************************************#
#                       RULES                          #
#******************************************************#
all: $(SERVER_NAME) $(CLIENT_NAME)

bonus:
	$(MAKE) BONUS=1 all

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(SERVER_NAME): $(SERVER_OBJ_USED) $(LIBFT)
	$(CC) $(CFLAGS) $(SERVER_OBJ_USED) $(LIBFT) -o $(SERVER_NAME)

$(CLIENT_NAME): $(CLIENT_OBJ_USED) $(LIBFT)
	$(CC) $(CFLAGS) $(CLIENT_OBJ_USED) $(LIBFT) -o $(CLIENT_NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

clean:
	rm -f $(SERVER_OBJ_ALL) $(CLIENT_OBJ_ALL)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(SERVER_NAME) $(CLIENT_NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all bonus clean fclean re
