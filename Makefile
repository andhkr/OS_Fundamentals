K = kernel
C = cpu
D = Datastructures

OBJS1=\
$K/interrupt_hndl.o\
$K/lock.o\
$K/proc_simltr.o\
$K/round_robin.o

OBJS2=\
$C/cpu.o\

OBJS3=\
$D/proc_queue.o\
$D/tree.o

OBJS = $(OBJS1) $(OBJS2) $(OBJS3)
all:$(OBJS)
	g++ $^ -o os

$K/%.o:$K/%.c
	g++ $^ -c -o $@

$C/%.o:$C/%.c
	g++ $^ -c -o $@

$D/%.o:$D/%.c
	g++ $^ -c -o $@
	

	
