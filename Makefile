SRC			= main.cpp server/Server.cpp socket/Socket.cpp  request/Request.cpp \
				request/body/Body.cpp request/header/Header.cpp request/requestLine/RequestLine.cpp \
				response/Response.cpp error/Error.cpp client/Client.cpp configFile/ConfigFile.cpp \
				errorHandling/ErrorHandling.cpp utils/utils.cpp http/MimeTypes.cpp http/EmimTypes.cpp http/HttpResponse.cpp
OBJ			= $(SRC:.cpp=.o)
NAME		= webserv
CPP			= c++
CPPFLAGS	= -Wall -Wextra -Werror -std=c++98 -fsanitize=address -g
HDERS		= webserver.hpp server/Server.hpp socket/Socket.hpp request/Request.hpp request/body/Body.hpp \
				request/header/Header.hpp request/requestLine/RequestLine.hpp response/Response.hpp \
				error/Error.hpp client/Client.hpp configFile/ConfigFile.hpp \
				errorHandling/ErrorHandling.hpp utils/utils.hpp http/MimeTypes.hpp http/EmimTypes.hpp http/HttpResponse.hpp

all : $(NAME)

$(NAME): $(OBJ) 
	$(CPP) $(CPPFLAGS) $(OBJ) -o $(NAME)

%.o: %.cpp $(HDERS)
	$(CPP) $(CPPFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all


