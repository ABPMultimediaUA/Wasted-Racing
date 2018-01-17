#Makefile for X-Kating Project

# Name of the executable created (.exe will be added automatically if necessary)
TARGET := XKating

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
all: create_dir make_src make_gameF make_gameM make_gameS make_gameO make_gameO_R make_gameO_A make_gameO_I make_gameO_F make_gameE make_gameP make_gameB make_gameA all_linux
	$(warning Linking done!)

#we can also have Windows
win: create_dir make_src make_gameF make_gameM make_gameS make_gameO make_gameO_R make_gameO_A make_gameO_I make_gameO_F make_gameE make_gameP make_gameB make_gameA all_windows
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

make_gameO_R:
	$(MAKE) -C src/GameObject/RenderComponent

make_gameO_A:
	$(MAKE) -C src/GameObject/AIComponent

make_gameO_I:
	$(MAKE) -C src/GameObject/ItemComponent
	
make_gameO_F:
	$(MAKE) -C src/GameObject/PhysicsComponent

make_gameE:
	$(MAKE) -C src/GameEvent
	
make_gameP:
	$(MAKE) -C src/GamePhysics

make_gameB:
	$(MAKE) -C src/GameBehaviourTrees

make_gameA:
	$(MAKE) -C src/GameAudio

# target specific settings
all_linux: LDFLAGS += -L/usr/X11R6/lib$(LIBSELECT) -lGL -lXxf86vm -lXext -lX11 -lXcursor
all_linux: LDFLAGS += -L./lib/linux/irrlicht -lIrrlicht -Wl,-rpath=./lib/linux/irrlicht
all_linux: LDFLAGS += -L./lib/linux/fmod -lfmod -lfmodstudio -Wl,-rpath=./lib/linux/fmod
all_linux: LDFLAGS += -L./lib/linux/raknet -lRakNetDLL -Wl,-rpath=./lib/linux/raknet

all_linux:
	$(CXX) $(CXXFLAGS) $(addprefix $(BIN_PATH)$(OBJ_PATH),$(shell ls $(BIN_PATH)$(OBJ_PATH))) -o $(BIN_PATH)$(TARGET) $(LDFLAGS)

clean: clean_linux
	$(warning Cleaned!)

clean_linux:
	rm -r $(BIN_PATH)$(OBJ_PATH)

info:
	$(info $(shell ls $(BIN_PATH)$(OBJ_PATH)))

################################################################################
# Windows 
################################################################################
all_windows: LDFLAGS += -L./lib/win -lIrrlicht -lopengl32 -lm -lfmod -lfmodstudio
all_windows:
	$(CXX) $(CXXFLAGS) $(addprefix $(BIN_PATH)$(OBJ_PATH),$(shell ls $(BIN_PATH)$(OBJ_PATH))) -o $(BIN_PATH)$(TARGET) $(LDFLAGS)



.PHONY: all clean clean_linux info install win

#multilib handling
ifeq ($(HOSTTYPE), x86_64)
LIBSELECT=64
endif
#solaris real-time features
ifeq ($(HOSTTYPE), sun4)
LDFLAGS += -lrt
endif
