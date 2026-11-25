# Makefile do Jogo do Milhao
# Uso: make (compila) | make run (executa) | make clean (limpa)
# Precisa da raylib instalada (parte grafica do jogo).

CC      = gcc
CFLAGS  = -Wall -Wextra -std=c99 -g
SRC_DIR = src
OUT_DIR = output
TARGET  = $(OUT_DIR)/jogo_do_milhao

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OUT_DIR)/%.o,$(SRCS))

ifeq ($(OS),Windows_NT)
    # Windows: raylib instalada pelo instalador oficial em C:\raylib
    RAYLIB_DIR ?= C:/raylib/raylib/src
    CFLAGS  += -I$(RAYLIB_DIR)
    # Compile com o make do w64devkit (C:\raylib\w64devkit\bin), que ja tem rm e sh
    TARGET  := $(TARGET).exe
    LDLIBS   = -L$(RAYLIB_DIR) -lraylib -lopengl32 -lgdi32 -lwinmm
else
    # Linux: raylib instalada no sistema (ex.: /usr/local/lib)
    LDLIBS   = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
endif

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

$(OUT_DIR):
	mkdir $(OUT_DIR)

$(OUT_DIR)/%.o: $(SRC_DIR)/%.c | $(OUT_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

run: all
	./$(TARGET)

clean:
	rm -f $(OUT_DIR)/*.o $(TARGET)

.PHONY: all run clean
