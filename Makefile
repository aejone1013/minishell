# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jaoh <jaoh@student.42.fr>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/08 11:20:19 by okoca             #+#    #+#              #
#    Updated: 2025/01/25 15:20:25 by jaoh             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SHELL=  /bin/bash

FBlack			=   $(shell echo -e "\033[1;30m")
Black			=   $(shell echo -e "\033[0;30m")
FRed			=   $(shell echo -e "\033[1;31m")
Red				=   $(shell echo -e "\033[0;31m")
FGreen          =   $(shell echo -e "\033[1;32m")
Green           =   $(shell echo -e "\033[0;32m")
FBrown		    =   $(shell echo -e "\033[1;33m")
Brown           =   $(shell echo -e "\033[0;33m")
FYellow         =   $(shell echo -e "\033[1;33m")
Yellow          =   $(shell echo -e "\033[0;33m")
FBlue           =   $(shell echo -e "\033[1;34m")
Blue            =   $(shell echo -e "\033[0;34m")
FPurple         =   $(shell echo -e "\033[1;35m")
Purple          =   $(shell echo -e "\033[0;35m")
FCyan           =   $(shell echo -e "\033[1;36m")
Cyan            =   $(shell echo -e "\033[0;36m")
FWhite          =   $(shell echo -e "\033[1;37m")
White           =   $(shell echo -e "\033[0;37m")
RESET           =   $(shell echo -e "\033[0m")

NAME 		= minishell

SRCS_M		= main.c

SRCS_BT		= bi_func.c bi_func_utils.c bi_func_utils2.c bi_err_utils.c bi_check_utils.c

SRCS_EX 	= exec.c exec_fdio_utils.c exec_pipe_utils.c exec_child_utils.c exec_err_utils.c exec_helper.c

SRCS_LEX 	= lexer.c tokens.c lex_helper.c

SRCS_PAR 	= parser.c expansion.c quotes.c parser_helper.c expand_helper.c heredoc.c

SRCS_UTI 	= utils.c helper.c env.c env_helper.c

SRCS_BLD 	= builder.c builder_helper.c filenames.c args.c

SRCS_SIG 	= signal.c

SRC 		= src/

BUILTINS_PATH	= src/builtins/
EXEC_PATH	= src/exec/
LEXER_PATH	= src/lexer/
PARSER_PATH	= src/parser/
UTILS_PATH	= src/utils/
BUILDER_PATH	= src/builder/
SIGNAL_PATH	= src/signals/

SRCS		= $(addprefix $(SRC), $(SRCS_M)) \
		  $(addprefix $(BUILTINS_PATH), $(SRCS_BT)) \
		  $(addprefix $(EXEC_PATH), $(SRCS_EX)) \
		  $(addprefix $(LEXER_PATH), $(SRCS_LEX)) \
		  $(addprefix $(PARSER_PATH), $(SRCS_PAR)) \
		  $(addprefix $(UTILS_PATH), $(SRCS_UTI)) \
		  $(addprefix $(BUILDER_PATH), $(SRCS_BLD)) \
		  $(addprefix $(SIGNAL_PATH), $(SRCS_SIG)) \

HEADERS		= builder.h builtins.h exec.h lexer.h minishell.h parser.h signals.h

OBJS		= $(SRCS:.c=.o)

HEAD		= includes/

CFLAGS		= -Wall -Wextra -Werror

CC		= cc

H_DEPS	= $(addprefix $(HEAD), $(HEADERS))

LIBFT_PATH	= libft/
LIBFT_H_PATH	= libft/includes/
LIBFT		= $(LIBFT_PATH)libft.a

all: $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFT_PATH)

$(NAME): $(LIBFT) $(OBJS) $(H_DEPS)
	$(CC) $(CFLAGS) $(OBJS) -lreadline $(LIBFT) -o $(NAME)
	@echo -e "${FBlue}${NAME}${Blue} compiled\n${RESET}"

%.o: %.c
	@$(CC) $(CFLAGS) -I$(HEAD) -I$(LIBFT_H_PATH) -c $< -o $@

clean:
	rm -f ${OBJS}
	$(MAKE) -C $(LIBFT_PATH) clean
	@echo -e "$(FPurple).o files $(Purple)cleaned\n${RESET}"

fclean: 
	rm -f ${OBJS}
	@echo -e "$(FPurple).o files $(Purple)cleaned\n${RESET}"
	rm -f ${NAME}
	$(MAKE) -C $(LIBFT_PATH) fclean
	@echo -e "$(FRed)${NAME}$(Red) cleaned${RESET}\n"

re : fclean all

.PHONY: all clean fclean re
