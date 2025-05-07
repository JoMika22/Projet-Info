CC = gcc
CFLAGS = -Wall -Wextra -g
TARGET = main
SRC = affichage.c combat.c equipe.c main.c perso.c tournoi.c utilis.c
OBJ = $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean
