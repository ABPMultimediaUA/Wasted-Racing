SOURCES:= $(wildcard *.cpp)
OBJECTS := $(subst .cpp,.o,$(SOURCES))

ejecutable: $(OBJECTS)
	g++ -o $@ $^

%.o: %.cpp
	g++ -c $^ -I.

info:
		$(info $(SOURCES))
		$(info $(OBJECTS))