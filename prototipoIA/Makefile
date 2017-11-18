SOURCES:= $(wildcard *.cpp)
OBJECTS := $(subst .cpp,.o,$(SOURCES))

CPPFLAGS += -I./../Game/ -I/usr/X11R6/include -I./../Game/include


ejecutable: $(OBJECTS)
	g++ -o $@ $^ -L/usr/X11R6/lib64 -lIrrlicht -lGL -lXxf86vm -lXext -lX11 -lXcursor

%.o: %.cpp
	 g++ -c $^ $(CPPFLAGS) -I.

info:
		$(info $(SOURCES))
		$(info $(OBJECTS))
