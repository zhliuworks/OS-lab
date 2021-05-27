all: proc1 proc2
	make proc1
	make proc2
proc1: proc1.c
	gcc -std=c11 proc1.c -o proc1
proc2: proc2.c
	gcc -std=c11 proc2.c -o proc2
clean:
	rm proc1 proc2