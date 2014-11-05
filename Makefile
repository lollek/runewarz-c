CC = cc
CFLAGS = -Wall -Wextra -Werror -ansi -pedantic -std=c99 -O3
LDFLAGS = -lSDL -lSDL_ttf -lSDL_image

SRC = $(wildcard src/*.c)
OBJS = $(addsuffix .o, $(basename $(SRC)))
CTARGET = runewarz

EMCC = emcc
EMTARGET = output/index.html
EMFLAGS = -Wall -Wextra -Werror -std=c99 --preload-file img/ --preload-file maps/ -g

$(CTARGET):	$(OBJS)
	$(CC) $^ $(LDFLAGS) -o $(CTARGET)

emscripten:	$(SRC)
	$(EMCC) $^ $(EMFLAGS) -o $(EMTARGET)

clean:
	$(RM) $(OBJS) $(CTARGET)

.PHONY: clean
