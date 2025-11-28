CC = gcc
CFLAGS = -Wall -g
LIBS = `sdl2-config --cflags --libs` -lm -lSDL2_ttf -lSDL2main -lSDL2_image -lSDL2_mixer

TARGET = Asteroid
SRCDIR = src
OBJDIR = obj
SRC = $(wildcard $(SRCDIR)/*.c) main.c
OBJS = $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS)
	$(MAKE) clean
	clear
	./Asteroid

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -f $(OBJS)

fclean:
	rm -f $(TARGET) $(OBJS)

re: clean all

check:
	@sdl2-config --version > /dev/null || (echo "SDL2 não encontrado. Instale a biblioteca SDL2." && exit 1)
