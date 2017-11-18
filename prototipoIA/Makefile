SOURCES:= $(wildcard *.cpp)
OBJECTS := $(subst .cpp,.o,$(SOURCES))

CPPFLAGS += -I./../Game/ -I/usr/X11R6/include -I./../Game/include


ejecutable: $(OBJECTS)
	g++ -o $@ $^

%.o: %.cpp
	 g++ -c $^ -I. $(CPPFLAGS)

info:
		$(info $(SOURCES))
		$(info $(OBJECTS))
