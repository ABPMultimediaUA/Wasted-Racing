#Makefile for X-Kating Project

# Name of the executable created (.exe will be added automatically if necessary)
TARGET := XKating

# Path to Irrlicht directory, should contain include/ and lib/
IrrlichtHome := .
# Path for the executable. Note that Irrlicht.dll should usually also be there for win32 systems
BIN_PATH := bin/
OBJ_PATH := obj/


# general compiler settings (might need to be set when compiling the lib, too)
CPPFLAGS += -I$(IrrlichtHome)/include -I/usr/X11R6/include -I.
ifndef NDEBUG
CXXFLAGS += -g -Wall
else
CXXFLAGS += -O3
endif

# Instruction to create directories
MKDIR_P = mkdir -p

#default target is Linux
all: create_dir make_src make_gameF make_gameM make_gameS make_gameO make_gameE make_gameP all_linux
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

make_gameF:
	$(MAKE) -C src/GameFacade

make_gameM:
	$(MAKE) -C src/GameManager

make_gameS:
	$(MAKE) -C src/GameState

make_gameO:
	$(MAKE) -C src/GameObject
	
make_gameE:
	$(MAKE) -C src/GameEvent
	
make_gameP:
	$(MAKE) -C src/GamePhysics

# target specific settings
all_linux all_win32 static_win32: LDFLAGS += -L$(IrrlichtHome)/lib/$(SYSTEM) -lIrrlicht -L/usr/lib -lfmod -lgainput -lfmodstudio 
all_linux: LDFLAGS += -L/usr/X11R6/lib$(LIBSELECT) -lGL -lXxf86vm -lXext -lX11 -lXcursor
all_linux clean_linux: SYSTEM=Linux

all_linux:
	$(CXX) $(CXXFLAGS) $(addprefix $(BIN_PATH)$(OBJ_PATH),$(shell ls $(BIN_PATH)$(OBJ_PATH))) -o $(BIN_PATH)$(TARGET) $(LDFLAGS)

clean: clean_linux
	$(warning Cleaned!)

clean_linux:
	rm -r $(BIN_PATH)$(OBJ_PATH)

info:
	$(info $(shell ls $(BIN_PATH)$(OBJ_PATH)))

install: install_linux

install_linux:
	$(warning Installing Irrlicht...)
	$(shell sudo cp lib/libIrrlicht.a /usr/lib/)
	$(warning Installing FMOD...)
	$(shell sudo cp lib/fsbank/lib/x86_64/* /usr/lib/)
	$(shell sudo cp lib/lowlevel/lib/x86_64/* /usr/lib/)
	$(shell sudo cp lib/studio/lib/x86_64/* /usr/lib/)
	$(warning Installing Gainput...)
	$(shell sudo cp lib/gainput/* /usr/lib/)

.PHONY: all clean clean_linux info install

#multilib handling
ifeq ($(HOSTTYPE), x86_64)
LIBSELECT=64
endif
#solaris real-time features
ifeq ($(HOSTTYPE), sun4)
LDFLAGS += -lrt
endif
