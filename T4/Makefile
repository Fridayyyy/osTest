CC=gcc
CFLAGS = -g -Wall
OBJS = command.o Main.o
TARGET = Main

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(CFLAGS)

clean:
	rm  -rf  *.o  $(TARGET)