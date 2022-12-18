##################################################
#			GLOBAL VARIABLES DEFINITION			 #
##################################################

TARGET	= ft_ssl

CFLAGS	= -Wall -Wextra -g #-Werror
CC		= gcc

INC 	= -I ./include -I ./libft

SRCDIR	= ./src/
SRC		= main.c parameters.c
HEADERS = 
DEPS = ${addprefix include/, ${HEADERS}}

OBJDIR	= ./objs/
OBJS	= ${addprefix ${OBJDIR}, ${SRC:.c=.o}}

LIBFT	= libft/libft.a

##################################################
#			OUTPUT VARIABLES DEFINITION			 #
##################################################

RED = \x1b[1;31m
GREEN = \x1b[1;32m
BLUE = \x1b[1;34m
PURPLE = \x1b[0;35m
RESET = \x1b[0;0m

COMPILE = ${GREEN}Compiling${RESET}
BUILD = ${BLUE}Building${RESET}
CLEAN = ${RED}Cleaning${RESET}

##################################################
#				COMPILATION RULES				 #
##################################################

${OBJDIR}%.o: ${SRCDIR}%.c ${DEPS}
	@echo "${COMPILE} $<"
	@${CC} ${CFLAGS} -c $< ${INC} -o $@

${TARGET}: ${OBJDIR} ${OBJS} ${LIBFT}
	@${CC} ${OBJS} ${LIBFT} -o $@
	@echo "${GREEN}$@${PURPLE} was built successfully${RESET}"

${OBJDIR}:
	@mkdir -p ${OBJDIR}

${LIBFT}:
	@echo "${BUILD} libft"
	@${MAKE} --no-print-directory -C libft

##################################################
#  				   USUAL RULES					 #
##################################################

all: ${TARGET}

clean:
	@echo "${CLEAN} objects"
	@rm -rf ${OBJDIR}
	@echo "${CLEAN} libft"
	@${MAKE} --no-print-directory clean -C libft

fclean: clean
	@echo "${CLEAN} ${TARGET}"
	@rm -rf ${TARGET}
	@${MAKE} --no-print-directory fclean -C libft

re: fclean all

.PHONY:	all clean fclean re
