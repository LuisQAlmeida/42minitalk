# **************************************************************************** #
# ****************************** VARIABLES: ********************************** #
# **************************************************************************** #
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

BUILD_STATE_DIR = .build-state
MANDATORY_STAMP = $(BUILD_STATE_DIR)/mandatory
BONUS_STAMP = $(BUILD_STATE_DIR)/bonus

MODE_STAMP = $(MANDATORY_STAMP)
OTHER_MODE_STAMP = $(BONUS_STAMP)

ifeq ($(BONUS), 1)
SERVER_SRC_USED = $(SERVER_SRC_BONUS)
CLIENT_SRC_USED = $(CLIENT_SRC_BONUS)
MODE_STAMP = $(BONUS_STAMP)
OTHER_MODE_STAMP = $(MANDATORY_STAMP)
endif

SERVER_OBJ_USED = $(SERVER_SRC_USED:.c=.o)
CLIENT_OBJ_USED = $(CLIENT_SRC_USED:.c=.o)

SERVER_OBJ_ALL = $(SERVER_SRC:.c=.o) $(SERVER_SRC_BONUS:.c=.o)
CLIENT_OBJ_ALL = $(CLIENT_SRC:.c=.o) $(CLIENT_SRC_BONUS:.c=.o)

INCLUDE = -Iinclude -Ilibft

LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

# **************************************************************************** #
# ****************************** RULES: ************************************** #
# **************************************************************************** #
all: $(SERVER_NAME) $(CLIENT_NAME)

bonus:
	$(MAKE) BONUS=1 all

$(BUILD_STATE_DIR):
	mkdir -p $(BUILD_STATE_DIR)

$(MODE_STAMP): | $(BUILD_STATE_DIR)
	rm -f $(OTHER_MODE_STAMP)
	touch $(MODE_STAMP)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(SERVER_NAME): $(SERVER_OBJ_USED) $(LIBFT) $(MODE_STAMP)
	$(CC) $(CFLAGS) $(SERVER_OBJ_USED) $(LIBFT) -o $(SERVER_NAME)

$(CLIENT_NAME): $(CLIENT_OBJ_USED) $(LIBFT) $(MODE_STAMP)
	$(CC) $(CFLAGS) $(CLIENT_OBJ_USED) $(LIBFT) -o $(CLIENT_NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

clean:
	rm -f $(SERVER_OBJ_ALL) $(CLIENT_OBJ_ALL)
	rm -rf $(BUILD_STATE_DIR)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(SERVER_NAME) $(CLIENT_NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all bonus clean fclean re
