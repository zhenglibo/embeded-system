#ifndef _TCP_SER_H_
#define _TCP_SER_H_

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
//#include <mutex.h>
#include <signal.h>
#include <mysql/mysql.h>
//#include <algorithm>
#include <stddef.h>

#define MaxGramLen 1024

#define uint unsigned int
#define uchar unsigned char
#define ulong unsigned long
//阻抗信息
struct client_data{
	uint RealValue[MaxGramLen];
	uint ImagValue[MaxGramLen];
	uint ImpedanceValue[MaxGramLen];
};
//服务器参数
struct server_parameter{
	uint ports;
	uchar capacity;
	uchar parameter;
};
//数据库参数
struct server_inadress
{
	char IP[1024];
	char Server_account[1024];
	char password[1024];
};

struct client_data*  tcp_link(struct server_parameter* );
void tcp_reserve(void);
void Writetosql(void);
void pthread_SqlTrans(void);
#endif//tcp_ser.h