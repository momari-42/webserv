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
	@printf "\e[31mDo you want to execute the program? (y/n) : \e[0m" && read answer; \
	if [ "$$answer" = "y" ]; then \
		make start; \
	fi

$(NAME): $(OBJ) 
	@$(CPP) $(CPPFLAGS) $(OBJ) -o $(NAME)

%.o: %.cpp $(HDERS)
	@$(CPP) $(CPPFLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJ)

fclean: clean
	@rm -rf $(NAME)

re: fclean $(NAME)


start:
	@printf "\033[0;32m" # Set text color to green
	@for i in $$(seq 1 3); do \
		printf "\rServer starting "; \
		for j in $$(seq 1 $$i); do printf "."; done; \
		sleep 0.7; \
	done; \
	printf "\n\033[0m" # Reset text color
	@./webserv config/config.conf 

rm :
	@rm -rf ./upload/*