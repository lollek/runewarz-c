CC = cc
CFLAGS= -lSDL -lSDL_ttf -lSDL_image -Wall -Wextra -ansi -pedantic
SOURCE = src/main.c src/vvl_link.c src/vvx_graphics.c src/vvg_game.c 
EXE = -o runewarz

all:	$(SOURCE)
	$(CC) $(SOURCE) $(CFLAGS) $(EXE)

debug:	$(SOURCE)
	$(CC) $(SOURCE) $(CFLAGS) -g $(EXE)