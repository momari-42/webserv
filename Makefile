SRC			= main.cpp server/Server.cpp socket/Socket.cpp  request/Request.cpp request/body/Body.cpp request/header/Header.cpp request/requestLine/RequestLine.cpp
OBJ			= $(SRC:.cpp=.o)
NAME		= webserv
CPP			= c++
CPPFLAGS	= -Wall -Wextra -Werror -std=c++98 -fsanitize=address
HDERS		= webserver.hpp server/server.hpp socket/Socket.hpp request/Request.hpp request/body/Body.hpp request/header/Header.hpp request/requestLine/RequestLine.hpp

all : $(NAME)
	@clear
	./webserv

$(NAME): $(OBJ) 
	$(CPP) $(CPPFLAGS) $(OBJ) -o $(NAME)

%.o: %.cpp $(HDERS)
	$(CPP) $(CPPFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean $(NAME)