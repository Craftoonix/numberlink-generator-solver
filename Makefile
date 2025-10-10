# Makefile for building the Numberlink binary
# Credits: 
#    - Joshua J. Lelipaly(s3329941)
#    - Original Makefile template from slides "Programmeertechnieken Week 13" (2022)  by J.K. Vis
# Date: 2025-10-10

#workspace settings
INC_DIR = include
SRC_DIR = src
SOURCES = $(shell find $(SRC_DIR)/ -name '*.cc')
OBJECTS = $(SOURCES:.cc=.o)
DEPS = $(OBJECTS:.o=.d)
TARGET = numberlink

# CXX compiler settings
CC = g++
CFLAGS = -march=native
CPPFLAGS = -I $(INC_DIR) -Wall -Wextra -pedantic

.PHONY: all exec clean debug release

#release build
release: CFLAGS += -O3 -DNDEBUG
release: all

#debugging build
debug: CFLAGS += -O0 -DDEBUG -ggdb3
debug: all

#builds the TARGET binary
all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ $^

#compiles every .cc file in SRC_DIR
-include $(DEPS)
%.o: %.cc
	$(CC) $(CFLAGS) $(CPPFLAGS) -MMD -o $@ -c $<

#utility commands
exec:
	./${TARGET}
clean:
	rm -f ${OBJECTS} ${DEPS} ${TARGET}

