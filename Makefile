install:
	gcc -o shc processcmd.c shc.c -I.
test:
	gcc -o test testfunc.c -I.