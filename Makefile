EXEC = test-pqueue
SRC = $(shell find . -name "*.c")
CXX = gcc
CXX_OPTIONS = -std=c99 -g -O2 -Wall -Wextra -Wpedantic -Wshadow -Werror -fsanitize=address -lm 

all : ${EXEC}

${EXEC} : ${SRC}
	${CXX} $^ -o $@ ${CXX_OPTIONS}

clean :
	rm -rf ${EXEC}

