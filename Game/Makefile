#Makefile for X-Kating Project

# Name of the executable created (.exe will be added automatically if necessary)
TARGET := XKating

# Path to Irrlicht directory, should contain include/ and lib/
IrrlichtHome := .
# Path for the executable. Note that Irrlicht.dll should usually also be there for win32 systems
BIN_PATH := bin/
OBJ_PATH := obj/
#


# general compiler settings (might need to be set when compiling the lib, too)
CPPFLAGS += -I$(IrrlichtHome)/include -I/usr/X11R6/include -I.
ifndef NDEBUG
CXXFLAGS += -g -Wall
else
CXXFLAGS += -O3
endif

#default target is Linux
all: make_src make_gameF make_gameM make_gameS make_gameO all_linux

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

# target specific settings
all_linux all_win32 static_win32: LDFLAGS += -L$(IrrlichtHome)/lib/$(SYSTEM) -lIrrlicht
all_linux: LDFLAGS += -L/usr/X11R6/lib$(LIBSELECT) -lGL -lXxf86vm -lXext -lX11 -lXcursor
all_linux clean_linux: SYSTEM=Linux

_OBJECTS := $(shell ls $(BIN_PATH)$(OBJ_PATH))
OBJECTS := $(addprefix $(BIN_PATH)$(OBJ_PATH),$(_OBJECTS))

all_linux:
	$(warning Compilation done!)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(BIN_PATH)$(TARGET) $(LDFLAGS)

$(BIN_DIR)$(OBJ_DIR)%.o:


clean: clean_linux
	$(warning Cleaned!)

clean_linux:
	rm -r $(BIN_PATH)$(OBJ_PATH)

info:
	$(info $(OBJECTS))

.PHONY: all clean clean_linux info

#multilib handling
ifeq ($(HOSTTYPE), x86_64)
LIBSELECT=64
endif
#solaris real-time features
ifeq ($(HOSTTYPE), sun4)
LDFLAGS += -lrt
endif