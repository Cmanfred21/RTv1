NAME = RTv1

FILES = main.cpp rtv1.cpp

OBJ = $(addprefix objectives/, $(FILES:.cpp=.o))
HEADERS = -I include

LFT = -L libft/src -lft

all:
	make startuem -j8

startuem: $(NAME)

$(NAME): $(OBJ)
	#$(FLAGS) -fsanitize=address -g
	#-fsanitize=address -g
	g++ -o $(NAME) $(OBJ)

objectives/%.o: src/%.cpp | objectives
	#-fsanitize=address -g
	g++  $(INCL) $(HEADERS)  -o $@ -c $< -std=c++14


clean:
	rm -rf objectives/

fclean: clean
	rm -rf $(NAME)
	rm -rf $(NAME).dSYM

objectives:
	mkdir -p objectives/

re: fclean
	make all
