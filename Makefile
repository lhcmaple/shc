INCLUDE=-I./etc -I./header

install:source/*
	gcc -o shc source/* -std=c11 ${INCLUDE}
test:testfunc.c/testfunc.c
	gcc -o test testfunc/testfunc.c -I. -std=c11