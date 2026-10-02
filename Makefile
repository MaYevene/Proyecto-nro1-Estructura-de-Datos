CC = gcc
ALLEGRO_CFLAGS := $(shell pkg-config --cflags allegro-5 allegro_primitives-5 allegro_font-5 allegro_ttf-5)
ALLEGRO_LIBS := $(shell pkg-config --libs allegro-5 allegro_primitives-5 allegro_font-5 allegro_ttf-5)
CFLAGS = -Wall -Wextra -std=c99 $(ALLEGRO_CFLAGS)
LDLIBS = $(ALLEGRO_LIBS)
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj
TARGET = $(BUILD_DIR)/reproductor

SRCS = main.c Lista_Albumes.c Generar.c Fila_de_Reproduccion.c
OBJS = $(SRCS:%.c=$(OBJ_DIR)/%.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDLIBS)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: %.c Reproductor.h | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f *.o reproductor $(OBJS) $(TARGET)
	rmdir $(OBJ_DIR) $(BUILD_DIR) 2>/dev/null || true

.PHONY: all clean run
