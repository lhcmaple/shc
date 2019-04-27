install:
	gcc -o shc processcmd.c stringproc.c shc.c -I. -std=c11
test:testfunc.c
	gcc -o test testfunc.c -I. -std=c11