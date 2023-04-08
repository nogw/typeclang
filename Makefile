CC = gcc
CCFLAGS = -g -Wall
TARGET = typeclass

all: compile
	./$(TARGET)

compile:
	$(CC) $(TARGET).c $(CCFLAGS) -o $(TARGET)

clean:
	rm -rf $(TARGET)

