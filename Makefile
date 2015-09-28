# Makefile template for shared library
 
CC =arm-linux-gnueabihf-gcc # C compiler
CFLAGS = -fPIC -Wall -Wextra -O3 -g # C flags
LDFLAGS = -shared  # linking flags
RM = rm -f  # rm command
TARGET_LIB = libcrypto.so # target lib
 
SRCS = main.c dstu/kalina_256_256.c dstu/common.c # source files
OBJS = $(SRCS:.c=.o)

TSRCS = test.c
TOBJS = test.o

 
.PHONY: all
all: ${TARGET_LIB}
 
$(TARGET_LIB): $(OBJS)
	$(CC) ${LDFLAGS} -o $@ $^

test: 	$(TOBJS)
	$(CC) -o $@ $^ -ldl 
 
$(SRCS:.c=.d):%.d:%.c
	$(CC) $(CFLAGS) -MM $< >$@
 
include $(SRCS:.c=.d)
 
.PHONY: clean
clean:
	-${RM} ${TARGET_LIB} ${OBJS} $(SRCS:.c=.d) *.out *.o
 
