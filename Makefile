CC = gcc
CFLAGS = -Wall -Wextra -std=c99
TARGET = reproductor

# Agrega aquí el nuevo archivo .c (por ejemplo Generar.c)
SRCS = main.c Lista_Albumes.c Generar.c
OBJS = main.o Lista_Albumes.o Generar.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

main.o: main.c Reproductor.h
	$(CC) $(CFLAGS) -c main.c -o main.o

Lista_Albumes.o: Lista_Albumes.c Reproductor.h
	$(CC) $(CFLAGS) -c Lista_Albumes.c -o Lista_Albumes.o

Generar.o: Generar.c Reproductor.h
	$(CC) $(CFLAGS) -c Generar.c -o Generar.o

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f *.o $(TARGET)

.PHONY: all clean run
