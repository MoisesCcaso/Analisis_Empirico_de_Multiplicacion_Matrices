CC = gcc
CFLAGS = -02 -Wall

TARGET = experimento
SRC = experimento.c

all:
	$(CC) $(CLFAGS) $(SRC) -o $(TARGET)

run: all
	./$(TARGET)

clean:
	rm -f $(TARGET)
