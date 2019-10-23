# Target library
lib := libuthread.a
OBJS = queue.o 

libuthread.a: $(OBJS)
	ar rcs libuthread.a $(OBJS)

queue.o: queue.c
	gcc -c queue.c

clean:
	rm -rf *.o *.a

## TODO: Phase 1.1
