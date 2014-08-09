CC=g++
CFLAGS=-c -Wall
LDFLAGS=
SOURCES=main.cpp ledhandler.cpp
OBJECTS=$(SOURCES:.cpp=.o)
TARGET=led_server

all: $(SOURCES) $(TARGET)

$(TARGET): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(OBJECTS) $(TARGET)
