CFLAGS=-Wall -Wextra -ansi -pedantic $(OPT_OR_DEBUG)
OBJS=$(patsubst %.c,%.o,$(wildcard *.c))

parser_li3: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)
clean:
	rm parser_li3 $(wildcard *.o) $(wildcard *~)
