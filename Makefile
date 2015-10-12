CFLAGS  = -g -ansi -pedantic -Wall -Werror
LDFLAGS = -g

SRC = infer.c main.c show.c valid.c
OBJ = $(SRC:.c=.o)

all: stlc

stlc: $(OBJ)
	cc -o $@ $(OBJ) $(LDFLAGS)

.c.o:
	cc -c $< $(CFLAGS)

$(OBJ): stlc.h

clean:
	rm -f stlc $(OBJ)
