# Define the compiler and flags
CC = gcc
CFLAGS = -Wall -g

# Define the directories
SRC_DIR = linux/src
BUILD_DIR = build
DEMO_DIR = linux/demo
TEST_SCRIPT = $(DEMO_DIR)/test.sh

# Define the source and output filenames
SRC_FILE = $(SRC_DIR)/main.c
OUT_FILE = $(BUILD_DIR)/main

# Default target: build the program
all: $(OUT_FILE)

# Build target: compile the main program
$(OUT_FILE): $(SRC_FILE)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(OUT_FILE) $(SRC_FILE)

# Target to run the test script (for -demo flag)
demo: $(OUT_FILE)
	@echo "Running demo..."
	@bash $(TEST_SCRIPT)

# Clean target: remove the build directory
clean:
	rm -rf $(BUILD_DIR)

# If `-demo` flag is provided, build and then run the demo
.PHONY: all demo clean

# To use the Makefile with the `-demo` flag, run `make demo`
