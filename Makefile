# Makefile for building the Numberlink binary
# Credits: 
#    - Joshua J. Lelipaly(s3329941)
#    - Original Makefile template from slides "Programmeertechnieken Week 13" (2022)  by J.K. Vis
# Date: 2025-10-10

#workspace settings
INC_DIR = include
SRC_DIR = src
BIN_DIR = build
SOURCES = $(shell find $(SRC_DIR)/ -name '*.cc')
OBJECTS = $(patsubst $(SRC_DIR)/%.cc,$(BIN_DIR)/%.o,$(SOURCES))
DEPS = $(OBJECTS:.o=.d)
TARGET = numberlink

# CXX compiler settings
CC = g++
CFLAGS = -march=native
CPPFLAGS = -I $(INC_DIR) -Wall -Wextra -pedantic

.PHONY: all exec clean debug release

# Release build
release: CFLAGS += -O3 -DNDEBUG
release: all

# Debugging build
debug: CFLAGS += -O0 -DDEBUG -ggdb3
debug: all

# Builds the TARGET binary
all: $(BIN_DIR) $(TARGET)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ $^

# Compiles every .cc file in SRC_DIR
-include $(DEPS)
$(BIN_DIR)/%.o: $(SRC_DIR)/%.cc
	$(CC) $(CFLAGS) $(CPPFLAGS) -MMD -o $@ -c $<

# Utility commands
exec:
	./${TARGET}
clean:
	rm -f ${BIN_DIR}/*.o ${BIN_DIR}/*.d ${TARGET}