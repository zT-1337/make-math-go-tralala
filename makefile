# Compiler and flags
CC      = gcc
CFLAGS  = -Wall -Wpedantic -Wextra -std=c11 -Iinclude -MMD -MP
LDFLAGS = -lm

# Directories
SRC_DIR = src
OBJ_DIR = obj
INC_DIR = include

# Target
TARGET  = tralala-maker

# Source, object, and dependency files
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))
DEPS = $(OBJS:.o=.d)

# Default rule
all: $(TARGET)

# Link
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Compile source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Create object directory if it doesn't exist
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Include auto-generated header dependencies
-include $(DEPS)

# Clean
clean:
	rm -rf $(OBJ_DIR) $(TARGET) *.wav

run: all
	./$(TARGET)

.PHONY: all clean

