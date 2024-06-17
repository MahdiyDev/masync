CC = gcc

CFLAGS=

SOURCE_DIR = src
EXAMPLE_SOURCE_DIR = examples
BUILD_DIR = build
BUILD_OBJ_DIR=$(BUILD_DIR)/obj
BUILD_EXAMPLE_DIR=$(BUILD_DIR)/$(EXAMPLE_SOURCE_DIR)

INCLUDES+=-I./$(SOURCE_DIR)

CSOURCES = $(wildcard $(SOURCE_DIR)/*.c)
EXAMPLE_CSOURCES = $(wildcard $(EXAMPLE_SOURCE_DIR)/*.c)

COBJECTS = $(patsubst $(SOURCE_DIR)/%.c,$(BUILD_OBJ_DIR)/%.o,$(CSOURCES))
EXAMPLE_COBJECTS = $(patsubst $(EXAMPLE_SOURCE_DIR)/%.c,$(BUILD_EXAMPLE_DIR)/%,$(EXAMPLE_CSOURCES))

all: make_build_dir $(COBJECTS) $(EXAMPLE_COBJECTS)

print_obj:
	@echo $(COBJECTS) $(EXAMPLE_COBJECTS)

$(BUILD_OBJ_DIR)/%.o: $(SOURCE_DIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -g -Og -c $< -o $@

$(BUILD_EXAMPLE_DIR)/%: $(EXAMPLE_SOURCE_DIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) $< $(COBJECTS) -o $@

make_build_dir:
	mkdir -p $(BUILD_OBJ_DIR)
	mkdir -p $(BUILD_EXAMPLE_DIR)

.PHONY: clean
clean:
	rm -rf *.o
	rm -rf build/*
