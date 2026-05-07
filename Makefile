CC=gcc
CFLAGS=-O2 -std=c11 -Wall -Wextra -pedantic

TARGET=experimento
OUTPUT=resultados.dat

all: run plot

$(TARGET): experimento.c
	$(CC) $(CFLAGS) experimento.c -o $(TARGET)

run: $(TARGET)
	./$(TARGET) > $(OUTPUT)

plot:
	gnuplot plot.gp

clean:
	rm -f $(TARGET) $(OUTPUT) grafico.png
