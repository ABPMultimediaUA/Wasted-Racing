#Makefile for X-Kating Project

# Path to Irrlicht directory, should contain include/ and lib/
IrrlichtHome := .

# Our .cpp and where to store them
SOURCES:= $(wildcard *.cpp)
_OBJECTS := $(subst .cpp,.o,$(SOURCES))
BIN_DIR := ../../bin/
OBJ_DIR := obj/
OBJECTS := $(patsubst %,$(BIN_DIR)$(OBJ_DIR)%,$(_OBJECTS))

# general compiler settings (might need to be set when compiling the lib, too)
CPPFLAGS += -I./../../ -I/usr/X11R6/include -I./../../include
ifndef NDEBUG
CXXFLAGS += -g -Wall
else
CXXFLAGS += -O3
endif

# Instruction to create directories
MKDIR_P = mkdir -p

#default target is Linux
all: make_bin_dir make_obj_dir all_linux

make_bin_dir:
	$(warning Creating bin directory... (if it doesn't exist))
	$(MKDIR_P) $(BIN_DIR)

make_obj_dir:
	$(warning Creating obj directory... (if it doesn't exist))
	$(MKDIR_P) $(BIN_DIR)$(OBJ_DIR)

all_linux: $(OBJECTS)
	$(warning Compilation done!)

$(BIN_DIR)$(OBJ_DIR)%.o: %.cpp
	$(warning Compiling...)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $^

clean: clean_linux
	$(warning Cleaned!)

clean_linux:
	rm -r $(BIN_DIR)$(OBJ_DIR)

info:
		$(info $(SOURCES))
		$(info $(OBJECTS))

.PHONY: all clean clean_linux make_dir info 