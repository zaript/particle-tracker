CC = gcc
CFLAGS = -Wall -c

OBJS = readInputData.o test_NASTRAN_read.o global.o 

ALL: $(OBJS)
	$(CC) $(OBJS) -o particle_tracker

inputtest: readInputData.o test_NASTRAN_read.o global.o 
	$(CC) readInputData.o test_NASTRAN_read.o global.o -o test_nastran

clean:
	\rm -rf *o *~ test_nastran particle_tracker

.c.o:
	$(CC) $(CFLAGS) $<
