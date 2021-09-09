NAME	= libftprintf.a

SRCS	= ft_printf.c
OBJS	= $(SRCS:.c=.o)

HEADER	= ft_printf.h

all:	$(NAME)

.c.o:
	gcc -Wall -Wextra -Werror -c $< -o $(<:.c=.o) -I $(HEADER)

$(NAME): $(OBJS)
	ar rc $(NAME) $(OBJS)

clean:
	rm -f $(OBJS)

fclean:	 clean
	rm -f $(NAME)

re:	fclean all

.PHONY:	all clean fclean re
