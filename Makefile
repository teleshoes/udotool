TARGET = udotool

CC = gcc
CFLAGS = -Wall

PREFIX = /usr/local
DIR_BIN = $(PREFIX)/bin
INSTALL = /usr/bin/install -c

all: $(TARGET)

$(TARGET): src/$(TARGET).cpp
	$(CC) $(CFLAGS) -o $@ $^

clean:
	$(RM) $(TARGET)

install: all
	$(INSTALL) -m 755 $(TARGET) $(DIR_BIN)

uninstall:
	$(RM) $(DIR_BIN)/$(TARGET)
