INCLUDE=-I./etc -I./header

install:source/*
	gcc -o /usr/bin/shc source/* -std=c11 ${INCLUDE}
test:testfunc/testfunc.c
	gcc -o test testfunc/testfunc.c -I. -std=c11
uninstall:
	rm /usr/bin/shc