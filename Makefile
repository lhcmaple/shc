install:source/* header/* etc/*
	gcc -o /usr/bin/shc source/* -std=c11 -I.
	chown lhc /usr/bin/shc
	chgrp lhc /usr/bin/shc
test:testfunc/testfunc.c
	gcc -o test testfunc/testfunc.c -I. -std=c11
uninstall:
	rm /usr/bin/shc