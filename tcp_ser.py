#！ /usr/bin/env python
# -*- coding:utf-8 -*-  
 from socket import *
 from time import ctime
 
 HOST = '127.0.0.0'
 PORT = 8080
 BufSize = 1024
 Addr = (HOST,PORT)
 tcpSock = socket(AF_INET,SOCK_STREAM)
 tcpSock.bind(Addr)
 tcpSock.listen(10);
 while True:
	tcpAcceptSock ,addr = tcpSock.accept()
	print('连接成功，客户端地址为: ',addr);
	while True:
		Data = tcpAcceptSock.recv(BufSize)
		if not Data:
			break
		print(Data.decode())
		msg = '{} 服务器已接收 '.format(ctime())
		tcpAcceptSock.send(msg.encode())
	tcpAcceptSock.close()
tcpSock.close()