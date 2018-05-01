
midi: LDFLAGS += -L./lib -lmidifile

midi:
	g++ -I./include -c -o main.o midiParser.cpp
	g++ -o midiParser main.o $(LDFLAGS)
	rm main.o
