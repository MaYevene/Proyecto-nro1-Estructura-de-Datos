CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LDFLAGS = -lallegro -lallegro_primitives -lallegro_font -lallegro_ttf
TARGET = reproductor

SRCS = main.c Lista_Albumes.c Generar.c Fila_de_Reproduccion.c
OBJS = main.o Lista_Albumes.o Generar.o Fila_de_Reproduccion.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

main.o: main.c Reproductor.h
	$(CC) $(CFLAGS) -c main.c -o main.o

Lista_Albumes.o: Lista_Albumes.c Reproductor.h
	$(CC) $(CFLAGS) -c Lista_Albumes.c -o Lista_Albumes.o

Generar.o: Generar.c Reproductor.h
	$(CC) $(CFLAGS) -c Generar.c -o Generar.o

Fila_de_Reproduccion.o: Fila_de_Reproduccion.c Reproductor.h
	$(CC) $(CFLAGS) -c Fila_de_Reproduccion.c -o Fila_de_Reproduccion.o

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f *.o $(TARGET)

.PHONY: all clean run
