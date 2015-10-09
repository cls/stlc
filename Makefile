CFLAGS  = -pedantic -std=c99 -Wall
LDFLAGS = -g

SRC = infer.c main.c show.c weigh.c
OBJ = $(SRC:.c=.o)

all: stlc

stlc: $(OBJ)
	cc -o $@ $(OBJ) $(LDFLAGS)

.c.o:
	cc -c $< $(CFLAGS)

$(OBJ): stlc.h

clean:
	rm -f $(OBJ)
