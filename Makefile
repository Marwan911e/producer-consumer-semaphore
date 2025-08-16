CC = gcc
CFLAGS = -Wall -Wextra -O2 -pthread
TARGET = producer_consumer
SOURCE = producer_consumer.c

all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE)

clean:
	rm -f $(TARGET) Producer_activity.txt Consumer_activity.txt

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
