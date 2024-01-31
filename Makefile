CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -pedantic -O3
LDFLAGS = -lm -fopenmp
SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)
EXEC = k_means

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ $(LDFLAGS)

clean:
	rm -f $(OBJ) $(EXEC)