CC = cc
SDLFLAG = -lSDL -lSDL_ttf -lSDL_image
DEBUG = $(SDLFLAG) -Wall -Wextra -ansi -pedantic -g
CFLAGS = $(SDLFLAG) -O3
SOURCE = src/main.c src/vvl_link.c src/vvx_graphics.c src/vvg_game.c 
EXE = -o runewarz

all:	$(SOURCE)
	$(CC) $(SOURCE) $(DEBUG) $(EXE)

final:	$(SOURCE)
	$(CC) $(SOURCE) $(CFLAGS) $(EXE)