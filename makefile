CC = gcc

CFLAGS = -Wall -g

LIBS = `sdl2-config --cflags --libs` -lm -lSDL2_ttf -lSDL2main -lSDL2_image -lSDL2_mixer

TARGET = Asteroid

HEADERS =  

SRCS = main.c src/desenhar.c src/control.c src/verificador.c src/novos.c src/inicializador.c

OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS)

# Regra para compilar arquivos .c em arquivos .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ $(LIBS)

# Regra para limpar arquivos temporários
clean:
	rm -f $(TARGET) $(OBJS)

# Regra para verificar se o compilador e as bibliotecas estão disponíveis
check:
	@sdl2-config --version > /dev/null || (echo "SDL2 não encontrado. Instale a biblioteca SDL2." && exit 1)
