#Makefile for X-Kating Project

# Name of the executable created (.exe will be added automatically if necessary)
Target := XKating
# List of source files, separated by spaces
Source1 := GameObject.cpp
Source2 := main.cpp
Source3 := InputManager.cpp
# Path to Irrlicht directory, should contain include/ and lib/
IrrlichtHome := .
# Path for the executable. Note that Irrlicht.dll should usually also be there for win32 systems
BinPath = bin
#Path to the source code
SourcePath = src/

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
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(SourcePath)$(Source1) $(SourcePath)$(Source2) $(SourcePath)$(Source3) -o $(DESTPATH) $(LDFLAGS)

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