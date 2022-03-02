CC	= g++ 
CFLAGS	= -O3
CPPSRC	= main.cpp  
OBJS	= main.o  

BIN	= aa 

all: aa

aa: aa.o
	$(CC) -o aa $(OBJS)

aa.o:
	$(CC) -c $(CPPSRC) $(CFLAGS)

clean:
	rm -f *.o $(BIN)
