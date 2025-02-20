CC = gcc
CFLAGS = -Wall -g
LIBS = `sdl2-config --cflags --libs` -lm -lSDL2_ttf -lSDL2main -lSDL2_image -lSDL2_mixer

TARGET = Asteroid

SRCDIR = src
SRC = $(wildcard $(SRCDIR)/*.c) main.c
OBJS = $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)

# Regra para verificar se o compilador e as bibliotecas estão disponíveis
check:
	@sdl2-config --version > /dev/null || (echo "SDL2 não encontrado. Instale a biblioteca SDL2." && exit 1)
