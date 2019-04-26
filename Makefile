install:
	gcc -o shc processcmd.c stringproc.c shc.c -I. -std=c11
test:
	gcc -o test testfunc.c -I. -std=c11