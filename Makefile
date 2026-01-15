
NAME = challenge

SRC = main.cpp helper_functions.cpp Company.cpp

OBJ = $(SRC:.cpp=.o) 

FLAGS = -Wall -Wextra -Werror

all : $(NAME)

$(NAME) : $(OBJ)
	g++ $(FLAGS) $(OBJ) -o $(NAME)
	rm -f $(OBJ)

clean :
	rm -f $(OBJ)

fclean : clean
	rm -f $(NAME)

re: fclean all