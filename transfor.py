#! /usr/bin/env python3
# -*- coding:utf-8 -*-
##本代码旨在不断分批接收GPRS
##模块发送来的阻抗数据
import time
import sys
import pymysql
from socket import *
from time import ctime
import re
import pthreading
##全局数组
StaticList = range(60)
AcceptNum = 6
Flag = True
##同步日志
def UpdateLogfile():
        i = 0
        Pos = len(StaticList)//2
        fp = open("Logfile.txt",'a',encoding = 'UTF-8')
        fp.write("-------------------------------------------------\n")	
        string = "          日志时间:%s          \n "%(time.asctime(time.localtime(time.time()))) 
        fp.write(string)
        fp.write("-------------------------------------------------\n")
        fp.write("实部  虚部\n")
        while i < 3:
                str1 = "%d  %d\n"%(StaticList[i],StaticList[i+Pos])
                fp.write(str1)
                i = i+1
        fp.close()
## 同步数据库
def UpdateSql():
        i = 0
        Pos = len(StaticList)//2
        dbconn = pymysql.connect("123.206.178.103","root","040912240sc","ImpedanceInfo",charset = 'utf8')
        Cursor = dbconn.cursor()
        Bufsize = AcceptNum
        while i < Bufsize/2:
                Sql = "INSERT INTO impedance_value(Impedance_Real,Impedance_Imag)VALUES(%d,%d)"%(StaticList[i],StaticList[i+Pos])
                try:
                        Cursor.execute(Sql)
                        dbconn.commit()
		except:
                        dbconn.rollback()
                        ##打印一次即可
                        if(Flag == True):
                                print("Fail to Insert!")
                                Flag = False
                i = i + 1
                Flag = True
        dbconn.close()
##字符串转换成整型
def StringtoInt(String,Num):
        i = 0
        List = []
        String = String.strip()
        StrList = String.split(b',')
        print(StrList)
        while i < Num:
                List.append(int(StrList[i]))
                i= i+1
        return List
####监听端口
def ListenPort():
        HOST = ''
        PORT = 8080
        BufSize =1024
        Addr = (HOST,PORT)
        TcpSock = socket(AF_INET,SOCK_STREAM)
        TcpSock.bind(Addr)
        TcpSock.listen(10)
        print ("Listening Ports: 8080")
        while True:
                TcpAcceptSock ,Acceptaddr = TcpSock.accept()
                print ('连接成功,客户端地址为: ',Acceptaddr)
                while True:
                        Data = TcpAcceptSock.recv(BufSize)
                        ###加锁,同步
                        Lock.acquire()
                        #分批接收数据
                        StaticList += StringtoInt(Data,Data.count(b',',0,len(Data)))
                        print (Data.decode())
                        if len(StaticList) > AcceptNum:
                                print("接收过多数据!\n")
                                ###解锁
                                Lock.release()
                                break
                        elif len(StaticList) == AcceptNum:
                                print("数据全部接收完毕!\n")
                                ###解锁
                                Lock.release()
                                break
                        else:
                                print("数据还未接收完毕,请等待...\n")
                        msg = '{0}: The Server Accept!'.format(ctime())
                TcpAcceptSock.send(msg.encode())	
                TcpAcceptSock.close()
                Lock.release()######解锁
                break
        TcpSock.close()
####锁          
Lock = threading.Lock()
threads = []
##创建线程
ListenThread = threading.Thread(target=ListenPort,args=())
UpdateSqlPtd = threading.Thread(target=UpdateSql,args=())
UpdateLogPtd = threading.Thread(target=UpdateLogfile,args=())
##添加线程到线程列表
threads.append(ListenThread)
threads.append(UpdateSqlPtd)
threads.append(UpdateLogPtd)
if __name__ == '__main__':
	##等待线程完成
	for Pt in threads:
		Pt,setDaemon(True)
		##开启线程
		Pt.start()
	print("退出主线程")


