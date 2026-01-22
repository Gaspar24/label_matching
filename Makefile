
NAME = challenge

SRC = main.cpp helper_functions.cpp Company.cpp

OBJ = $(SRC:.cpp=.o) 

FLAGS = -Wall -Wextra -Werror

COMPANIES = ml_insurance_challenge.csv
LABELS = insurance_taxonomy.csv
OUTPUT = classified_*.csv

all : $(NAME)

$(NAME) : $(OBJ)
	g++ $(FLAGS) $(OBJ) -o $(NAME)
	rm -f $(OBJ)
	
run: $(NAME)
	./$(NAME) $(COMPANIES) $(LABELS)
	rm -f $(NAME)

clean :
	rm -f $(OBJ)

fclean : clean
	rm -f $(NAME) 
	rm -f $(OUTPUT)



re: fclean all