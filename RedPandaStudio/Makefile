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
all_linux: LDFLAGS +=  -lsfml-graphics -lsfml-window -lsfml-graphics -lassimp

all_linux:
	$(CXX) $(CXXFLAGS) -shared -o $(BIN_PATH)$(TARGET).so $(addprefix $(BIN_PATH)$(OBJ_PATH),$(shell ls $(BIN_PATH)$(OBJ_PATH))) $(LDFLAGS)

clean: clean_linux
	$(warning Cleaned!)

clean_linux:
	rm -rf $(BIN_PATH)$(OBJ_PATH)

info:
	$(info $(shell ls $(BIN_PATH)$(OBJ_PATH)))

################################################################################
# Windows 
################################################################################
#all_windows: LDFLAGS += -L./lib/win -lIrrlicht -lopengl32 -lm -lfmod -lfmodstudio
#all_windows:
#	$(CXX) $(CXXFLAGS) $(addprefix $(BIN_PATH)$(OBJ_PATH),$(shell ls $(BIN_PATH)$(OBJ_PATH))) -o $(BIN_PATH)$(TARGET) $(LDFLAGS)


hashtag := "\#"
newline := \"\\n
includeCommand := $(hashtag)include \"
sh_value := "export LD_LIBRARY_PATH=./bin/ && ./bin/test"

test: remdir makedir copyfiles includefiles compile makesh
	$(warning Done!)
	
remdir:
	rm -rf ./inc

makedir:
	mkdir -p ./inc

copyfiles:
	cp -r ./src/*.h ./inc
	cp -r ./src/Facade/*.h ./inc

includefiles:
	@echo -e $(addprefix $(includeCommand), $(addsuffix $(newline),$(shell ls ./inc))) >> ./inc/RedPanda.h
	
compile:
	g++ -I./inc -c -o bin/main.o main.cpp
	g++ -o bin/test bin/main.o -L./bin -lRedPanda -Wl,-rpath=./bin

makesh:
	rm -rf ./bin/test.sh
	@echo -e $(sh_value) >> ./bin/test.sh
	chmod +x ./bin/test.sh


.PHONY: all clean clean_linux info install win