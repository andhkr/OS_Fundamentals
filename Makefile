K = kernel
C = cpu
Q = queue

OBJS=\
$K/interrupt_hndl.o\
$K/lock.o\
$K/proc_simltr.o\
$K/round_robin.o\
$C/cpu.o\
$Q/proc_queue.o

all:$(OBJS) 
	g++ $^ -o os

OBJS:%.o:%.c
	g++ $^ -c -o $@
	

	
