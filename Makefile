CC = gcc
CFLAGS = -Wall -Wextra -g -std=c11
LIBS = -lallegro -lallegro_primitives -lallegro_font -lallegro_ttf -lallegro_image -lallegro_main
BUILD_DIR = build
TARGET = $(BUILD_DIR)/reproductor

SRCS = main.c Lista_Albumes.c Generar.c diseño.c
OBJS = $(BUILD_DIR)/main.o $(BUILD_DIR)/Lista_Albumes.o $(BUILD_DIR)/Generar.o $(BUILD_DIR)/diseño.o

all: $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/main.o: main.c Reproductor.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c main.c -o $(BUILD_DIR)/main.o

$(BUILD_DIR)/Lista_Albumes.o: Lista_Albumes.c Reproductor.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c Lista_Albumes.c -o $(BUILD_DIR)/Lista_Albumes.o

$(BUILD_DIR)/Generar.o: Generar.c Reproductor.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c Generar.c -o $(BUILD_DIR)/Generar.o

$(BUILD_DIR)/diseño.o: diseño.c Reproductor.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c diseño.c -o $(BUILD_DIR)/diseño.o

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean run
