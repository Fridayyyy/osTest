CC=g++
CFLAGS = -g -Wall -lpthread
OBJS = Consumer.o Producer.o ProducerConsumerUtilities.o mainProg.o
TARGET = mainProg

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(CFLAGS)

clean:
	rm  -rf  *.o  $(TARGET)