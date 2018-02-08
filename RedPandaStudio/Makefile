#Makefile for RedPandaStudio Project

# Name of the executable created (.exe will be added automatically if necessary)
TARGET := libRedPanda

# Path for the executable
BIN_PATH := bin/
OBJ_PATH := obj/


# general compiler settings (might need to be set when compiling the lib, too)
CPPFLAGS += -I.
ifndef NDEBUG
CXXFLAGS += -g -Wall
else
CXXFLAGS += -O3
endif

# Instruction to create directories
MKDIR_P = mkdir -p

#default target is Linux
all: create_dir make_src make_fac all_linux
	$(warning Linking done!)

#we can also have Windows
win: create_dir make_src make_fac all_windows
	$(warning Linking done!)

create_dir: make_bin_dir make_obj_dir

make_bin_dir:
	$(warning Creating bin directory... (if it doesn't exist))
	$(MKDIR_P) $(BIN_PATH)

make_obj_dir:
	$(warning Creating obj directory... (if it doesn't exist))
	$(MKDIR_P) $(BIN_PATH)$(OBJ_PATH)

#Makes to execute
make_src:
	$(MAKE) -C src

make_fac:
	$(MAKE) -C src/Facade

# target specific settings
#all_linux: LDFLAGS += -L/usr/X11R6/lib$(LIBSELECT) -lGL -lXxf86vm -lXext -lX11 -lXcursor
#all_linux: LDFLAGS += -L./lib/linux/irrlicht -lIrrlicht -Wl,-rpath=./lib/linux/irrlicht
#all_linux: LDFLAGS += -L./lib/linux/fmod -lfmod -lfmodstudio -Wl,-rpath=./lib/linux/fmod
#all_linux: LDFLAGS += -L./lib/linux/raknet -lRakNetDLL -Wl,-rpath=./lib/linux/raknet

all_linux:
	$(CXX) $(CXXFLAGS) -shared -o $(BIN_PATH)$(TARGET).so $(addprefix $(BIN_PATH)$(OBJ_PATH),$(shell ls $(BIN_PATH)$(OBJ_PATH))) $(LDFLAGS)

clean: clean_linux
	$(warning Cleaned!)

clean_linux:
	rm -r $(BIN_PATH)$(OBJ_PATH)

info:
	$(info $(shell ls $(BIN_PATH)$(OBJ_PATH)))

################################################################################
# Windows 
################################################################################
#all_windows: LDFLAGS += -L./lib/win -lIrrlicht -lopengl32 -lm -lfmod -lfmodstudio
#all_windows:
#	$(CXX) $(CXXFLAGS) $(addprefix $(BIN_PATH)$(OBJ_PATH),$(shell ls $(BIN_PATH)$(OBJ_PATH))) -o $(BIN_PATH)$(TARGET) $(LDFLAGS)



.PHONY: all clean clean_linux info install win