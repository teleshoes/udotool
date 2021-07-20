TARGET = udotool

CC = gcc
CFLAGS  = -Wall

all: $(TARGET)

$(TARGET): src/$(TARGET).cpp
	$(CC) $(CFLAGS) -o $@ $^

clean:
	$(RM) $(TARGET)
