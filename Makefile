CC = cc
CFLAGS= -lSDL -lSDL_ttf -lSDL_image -Wall -Wextra -ansi -pedantic
SOURCE = src/main.c src/vvk_graphics.c src/vvk_game.c
EXE = -o runewarz

all:	$(SOURCE)
	$(CC) $(SOURCE) $(CFLAGS) $(EXE)

debug:	$(SOURCE)
	$(CC) $(SOURCE) $(CFLAGS) -g $(EXE)