NAME = minitalk.a

SRC = 
BONUS_SRC = 

OBJ = $(SRC:.c=.o)
BONUS_OBJ = $(BONUS_SRC:.c=.o)

CC = cc
CFLAGS = -Wall -Wextra -Werror
AR = ar rcs

all: $(NAME)

$(NAME): $(OBJ)
	$(AR) $(NAME) $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(BONUS_OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

bonus:
	$(CC) $(CFLAGS) -c $(SRC) $(BONUS_SRC)
	$(AR) $(NAME) $(OBJ) $(BONUS_OBJ)
