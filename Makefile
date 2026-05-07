CC = gcc
CFLAGS = -O2 -Wall

TARGET = experimento
SRC = experimento.c

all:
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

run: all
	./$(TARGET)

clean:
	rm -f $(TARGET)
