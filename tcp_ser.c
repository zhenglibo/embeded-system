#include "tcp_ser.h"

pthread_mutex_t mutex;
pthread_cond_t cond;

struct client_data* tcp_link(struct server_parameter* ser_para)
{
	uint linknum = 0;
	int listenfd,connectfd;
	struct sockaddr_in clientaddr,serveraddr;
	time_t ticks;
	char writeBuf[MaxGramLen];
	int clientlen;
	
	struct client_data* client;
	memset(client->ImpedanceValue,0,sizeof(client->ImpedanceValue)); 
	//pthread_mutex_t mutex;
	
	if((listenfd = socket(AF_INET,SOCK_STREAM,0))< 0) 
	{
		perror("socket"),exit(EXIT_FAILURE);
	}
	
	memset(&serveraddr,0,sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(ser_para->ports);
	
	if(bind(listenfd,(struct sockaddr*)&serveraddr,sizeof(serveraddr))<0)
	{
		perror("bind"),exit(EXIT_FAILURE);
	}
	if(listen(listenfd,20)<0)//the maxium num of link 20
	{
		perror("listen"),exit(EXIT_FAILURE);
	}
	for(;;)
	{
		if(accept(listenfd,(struct sockaddr*)&clientaddr,&clientlen) < 0)
		{
			perror("accept");
			exit(EXIT_FAILURE);
		}
		//add metux lock
		pthread_cleanup_push(pthread_mutex_unlock, (void *)&mutex);
		pthread_mutex_lock(&mutex);
		
		if(read(listenfd,client->ImpedanceValue,MaxGramLen) < 0 )
		{
			perror("read");
			memset(client->ImpedanceValue,0,sizeof(client->ImpedanceValue));
			exit(EXIT_FAILURE);
		}
		pthread_mutex_unlock(&mutex);
		//sim900a info
		snprintf(writeBuf,sizeof(writeBuf),"%d",linknum);
		if(write(connectfd,writeBuf,sizeof(writeBuf)) < 0)
		{
			perror("write");
			exit(EXIT_FAILURE);
		}
	}
	printf("finished to communicate!\n");
	if(client == NULL)
	{
		printf("No data come\n");
	}else if(sizeof(client->ImpedanceValue) > ser_para->parameter)
	{	
			close(connectfd);//关闭连接
			//pthread_cond_signal(&cond);
	}
		//sigaction(SIG_USR1,&tcp_server,NULL);
		//if(pthread_kill(SIG_USR1,pthread_LogWrite)==-1) perror("kill"),exit(0);		
	return client;
}

void tcp_reserve(void)
{
	int fd,i;
	time_t timeptr;
	char logBuf[1024*1024];
	pthread_rwlock_t logLock;
	pthread_rwlock_rdlock(&logLock);
	time(&timeptr);
	//write logfile
	if((fd = open("./logfile",O_RDWR|O_CREAT|O_APPEND))<0)
	{
		perror("open"),exit(-1);
	}
	sprintf(logBuf,"---------------%s--------------\n",asctime(gmtime(&timeptr)));
	for(i = 0; i < sizeof(client->ImpedanceValue) ;i++)
	{
		sprintf(logBuf,"%d ",client->ImpedanceValue[i]);
	}
		sprintf(logBuf,"\n");
	if(write(fd,client->ImpedanceValue,sizeof(client->ImpedanceValue))<0)
	{
		perror("write"),exit(-1);
	}
	pthread_rwlock_unlock(&logLock);
	//if(pthread_kill(SIG_USR2,pthread_SqlTrans)==-1) perror("kill"),exit(0);

}

void Writetosql(void)
{
	MYSQL conn;
    char query[1024];
    int res,i;
	pthread_rwlock_t sqlLock;
   	mysql_init(&conn);
	pthread_rwlock_rdlock(&sqlLock);
    if(mysql_real_connect(&conn,"123.206.178.103","sansha","040912240sc",\
	"IMPEDANCE",0,NULL,CLIENT_FOUND_ROWS)) //"root":数据库管理员 "":root密码 "test":数据库的名字
	{
		printf("connect success!\n");
        for(i=0;i<=60;i++)
        {
			sprintf(query,"insert into IMPEDANCE(impedance_imag,impedance_real)values(%d,%d)",client->ImpedanceValue[i],client->ImpedanceValue[60+i]);
		}
        res=mysql_query(&conn,query);
        if(res)
        {
            printf("error\n");
			exit(0);
         }
          mysql_close(&conn);
     }else{
		 perror("mysql_real_connect");
		 exit(EXIT_FAILURE);
	 }
	 pthread_rwlock_unlock(&sqlLock);
}

void pthread_SqlTrans(void)
{
	pthread_t ptd_sql;
	int ret1 = pthread_create(&ptd_sql,NULL,Writetosql,NULL);
	if(ret1 < 0)
	{
		perror("pthread_create"),exit(-1);
	}
	pthread_join(ptd_sql,NULL);
}
void pthread_LogWrite(void)
{
	pthread_t ptd_reserve;
	int	ret2 = pthread_create(&ptd_reserve,NULL,tcp_reserve,NULL);
	if(ret2 < 0)
	{
		perror("pthread_create"),exit(-1);
	}
	pthread_join(ptd_reserve,NULL);
	//pthread_exit(0);
}

int main(int argc,char** argcv)
{
	pthread_t ptd_tcp;
	int ret;
	
	struct server_parameter* ser_para;
	ser_para->parameter = 120;
	ser_para->ports = 8080;
	
	struct sigaction siga;
	siga.sa_handler = tcp_reserve ;
	sigemptyset(&siga.sa_mask);
	sigaddset(&siga.sa_mask,SIGUSR2);
	
	struct sigaction siga1;
	siga1.sa_handler = Writetosql ;
	sigemptyset(&siga1.sa_mask);
	sigaddset(&siga1.sa_mask,SIGUSR1);
	
	pthread_mutex_init(&mutex,NULL);
	pthread_cond_init(&cond,NULL);
	
	ret = pthread_create(&ptd_tcp,NULL,tcp_link,&ser_para->ports);
	if(ret < 0)
	{
		perror("pthread_create"),exit(-1);
	}
	pthread_join(ptd_tcp,NULL);
	//pthread_exit(0);
	return 0;
}

