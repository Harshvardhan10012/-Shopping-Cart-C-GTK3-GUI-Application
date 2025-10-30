CC = gcc
CFLAGS = `pkg-config --cflags gtk+-3.0` -Wall
LIBS = `pkg-config --libs gtk+-3.0`

SRC = main.c ui.c cart.c payment.c
OBJ = $(SRC:.c=.o)

all: shopping_cart

shopping_cart: $(OBJ)
	$(CC) -o $@ $(OBJ) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o shopping_cart
