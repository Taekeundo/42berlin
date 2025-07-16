# [ COMMAND ]
CC			= c++
CFLAGS		= -Wall -Wextra -Werror -std=c++98
CDEBUG		= -fsanitize=address -g -pedantic
RM			= rm -f

# [ FILE ]
NAME		= PmergeMe
SRCS		= $(shell find . -name "*.cpp")
OBJS		= $(SRCS:.cpp=.o)

# [ COMPILE ]
%.o			: %.cpp
				$(CC) $(CFLAGS) $(CDEBUG) -o $@ -c $<

$(NAME)		: $(OBJS)
				$(CC) $(CFLAGS) $(CDEBUG) -o $(NAME) $(OBJS)

# [ RULE ]
all			: $(NAME)

clean		:
				$(RM) $(OBJS)

fclean		: clean
				$(RM) $(NAME)

re			: fclean all

.PHONY		: all clean fclean re