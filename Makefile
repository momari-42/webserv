# Colors
GREEN	= \033[0;32m
RED		= \033[0;31m
BLUE	= \033[0;34m
CYAN	= \033[0;36m
YELLOW	= \033[0;33m
PURPLE	= \033[0;35m
RESET	= \033[0m

DOTS	= "⠋ ⠙ ⠹ ⠸ ⠼ ⠴ ⠦ ⠧ ⠇ ⠏"

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

define animate
	@i=0; \
	while [ $$i -lt 10 ]; do \
		for c in $(DOTS); do \
			printf "\r$(CYAN)[$$c]$(RESET) $(BLUE)$(1)$(RESET)"; \
			sleep 0.1; \
		done; \
		i=$$((i+1)); \
	done
endef

define USAGE_MESSAGE
	@printf "\n\n$(PURPLE)╔═══════════════════════════════════════════════════╗\n"; \
	printf "║$(YELLOW)         🚀 WEBSERV LAUNCH INSTRUCTIONS 🚀         $(PURPLE)║\n"; \
	printf "╠═══════════════════════════════════════════════════╣\n"; \
	printf "║                                                   ║\n"; \
	printf "║$(CYAN)     To start the webserver with config file:      $(PURPLE)║\n"; \
	printf "║                                                   ║\n"; \
	printf "║$(GREEN)         ./webserv config/config.conf$(PURPLE)              ║\n"; \
	printf "║                                                   ║\n"; \
	printf "╚═══════════════════════════════════════════════════╝$(RESET)\n\n"
endef

all : $(NAME)
	$(USAGE_MESSAGE)

$(NAME): $(OBJ) 
	@$(call animate,"Linking object files...")
	@$(CPP) $(CPPFLAGS) $(OBJ) -o $(NAME)
	@printf "\r$(GREEN)[✓]$(RESET) $(BLUE)Successfully built $(NAME)$(RESET)"

%.o: %.cpp $(HDERS)
	@$(call animate,"Compiling $<...")
	@$(CPP) $(CPPFLAGS) -c $< -o $@
	@printf "\r$(GREEN)[✓]$(RESET) $(BLUE)Compiled $<$(RESET)\n"

clean:
	@$(call animate,"Cleaning object files...")
	@rm -rf $(OBJ)
	@printf "\r$(GREEN)[✓]$(RESET) $(BLUE)Removed object files$(RESET)\n"

fclean: clean
	@$(call animate,"Removing executable...")
	@rm -rf $(NAME)
	@printf "\r$(GREEN)[✓]$(RESET) $(BLUE)Removed $(NAME)$(RESET)\n"

re: fclean all

.PHONY: all clean fclean re