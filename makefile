# Makefile do Jogo do Milhao
# Uso: make (compila) | make run (executa) | make clean (limpa)

CC      = gcc
CFLAGS  = -Wall -Wextra -std=c99 -g
SRC_DIR = src
OUT_DIR = output
TARGET  = $(OUT_DIR)/jogo_do_milhao

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OUT_DIR)/%.o,$(SRCS))

ifeq ($(OS),Windows_NT)
    TARGET := $(TARGET).exe
    RM     = del /Q
    FIXPATH = $(subst /,\,$1)
    MKDIR  = if not exist $(OUT_DIR) mkdir $(OUT_DIR)
else
    RM     = rm -f
    FIXPATH = $1
    MKDIR  = mkdir -p $(OUT_DIR)
endif

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(OUT_DIR)/%.o: $(SRC_DIR)/%.c
	@$(MKDIR)
	$(CC) $(CFLAGS) -c $< -o $@

run: all
	$(call FIXPATH,./$(TARGET))

clean:
	-$(RM) $(call FIXPATH,$(OUT_DIR)/*.o) $(call FIXPATH,$(TARGET))

.PHONY: all run clean
