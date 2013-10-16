CC=gcc
CFLAGS=-g
SOURCES=main.c parser.c
EXECUTABLE=hackasm

all: $(EXECUTABLE)

$(EXECUTABLE): $(SOURCES)
	$(CC) $(CFLAGS) -o $(EXECUTABLE) $(SOURCES)

clean:
	rm -f $(EXECUTABLE)
