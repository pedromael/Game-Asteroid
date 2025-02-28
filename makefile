CC = gcc
CFLAGS = -Wall -g
LIBS = `sdl2-config --cflags --libs` -lm -lSDL2_ttf -lSDL2main -lSDL2_image -lSDL2_mixer

TARGET = Asteroid
SRCDIR = src
OBJDIR = obj
SRC = $(wildcard $(SRCDIR)/*.c) main.c
OBJS = $(patsubst %.c, $(OBJDIR)/%.o, $(notdir $(SRC)))

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/main.o: main.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -f $(TARGET) $(OBJS)

check:
	@sdl2-config --version > /dev/null || (echo "SDL2 não encontrado. Instale a biblioteca SDL2." && exit 1)
