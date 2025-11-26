#******************************************************#
#			VARIABLES                      #
#******************************************************#
CC = cc
CFLAGS = -Wall -Wextra -Werror

SERVER_NAME = server
SERVER_SRC = src/server.c src/utils.c
SERVER_OBJ = $(SERVER_SRC:.c=.o)

CLIENT_NAME = client
CLIENT_SRC = src/client.c src/utils.c
CLIENT_OBJ = $(CLIENT_SRC:.c=.o)

INCLUDES = -Iincludes -Iincludes/Libft

LIBFT_DIR = includes/Libft
LIBFT = $(LIBFT_DIR)/libft.a

#******************************************************#
#                       RULES                          #
#******************************************************#
all: $(SERVER_NAME) $(CLIENT_NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(SERVER_NAME): $(SERVER_OBJ) $(LIBFT)
	$(CC) $(CFLAGS) $(SERVER_OBJ) $(LIBFT) -o $(SERVER_NAME)

$(CLIENT_NAME): $(CLIENT_OBJ) $(LIBFT)
	$(CC) $(CFLAGS) $(CLIENT_OBJ) $(LIBFT) -o $(CLIENT_NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(SERVER_OBJ) $(CLIENT_OBJ)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(SERVER_NAME) $(CLIENT_NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
