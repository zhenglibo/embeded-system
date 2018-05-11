#makefile edited by sansha

tcp.o: tcp_ser.h tcp_ser.c
	cc -c -I/usr/include/mysql *.c -L/usr/lib/mysql \
	-lmysqlclient -o  -lpthread
clean:
	rm tcp.o