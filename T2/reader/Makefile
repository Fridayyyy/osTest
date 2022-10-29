CC=g++
CFLAGS = -g -Wall -lpthread
OBJS = reader.o writer.o common.o mainProg.o
TARGET = mainProg

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(CFLAGS)

clean:
	rm  -rf  *.o  $(TARGET)