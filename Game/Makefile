#Makefile for X-Kating Project

# Name of the executable created (.exe will be added automatically if necessary)
Target := XKating
#Paths to the source code
ObjectPath := src/GameObject
ManagerPath := src/GameManager
StatePath := src/GameState
FacadePath := src/GameFacade
MainPath := src
# List of source files, separated by spaces
_SourceObject := GameObject.cpp
_SourceManager := InputManager.cpp RenderManager.cpp NetworkManager.cpp ResourceManager.cpp AudioManager.cpp LevelManager.cpp AIManager.cpp PhysicsManager.cpp ObjectManager.cpp
_SourceState := IntroState.cpp MenuState.cpp PauseState.cpp MatchState.cpp
_SourceFacade := RenderIrrlicht.cpp AudioFMOD.cpp
_SourceMain := main.cpp Game.cpp
Source := $(patsubst %,$(ObjectPath)/%,$(_SourceObject)) $(patsubst %,$(ManagerPath)/%,$(_SourceManager)) $(patsubst %,$(StatePath)/%,$(_SourceState)) $(patsubst %,$(FacadePath)/%,$(_SourceFacade)) $(patsubst %,$(MainPath)/%,$(_SourceMain))

# Path to Irrlicht directory, should contain include/ and lib/
IrrlichtHome := .
# Path for the executable. Note that Irrlicht.dll should usually also be there for win32 systems
BinPath := bin


# general compiler settings (might need to be set when compiling the lib, too)
CPPFLAGS += -I$(IrrlichtHome)/include -I/usr/X11R6/include
ifndef NDEBUG
CXXFLAGS += -g -Wall
else
CXXFLAGS += -O3
endif

#default target is Linux
all: all_linux

# target specific settings
all_linux all_win32 static_win32: LDFLAGS += -L$(IrrlichtHome)/lib/$(SYSTEM) -lIrrlicht
all_linux: LDFLAGS += -L/usr/X11R6/lib$(LIBSELECT) -lGL -lXxf86vm -lXext -lX11 -lXcursor
all_linux clean_linux: SYSTEM=Linux

# name of the binary - only valid for targets which set SYSTEM
DESTPATH = $(BinPath)/$(Target)

all_linux:
	$(warning Building...)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(Source) -o $(DESTPATH) $(LDFLAGS)

clean: clean_linux
	$(warning Cleaning...)

clean_linux:
	@$(RM) $(DESTPATH)

.PHONY: all clean clean_linux

#multilib handling
ifeq ($(HOSTTYPE), x86_64)
LIBSELECT=64
endif
#solaris real-time features
ifeq ($(HOSTTYPE), sun4)
LDFLAGS += -lrt
endif