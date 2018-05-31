#！ /usr/bin/env python
# -*- coding:utf-8 -*-  
from socket import *
from time import ctime
import pymysql

def GetSock():
       HOST = '127.0.0.1'
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
                     #msg = '{ }服务器已接收'.format(ctime())
                     msg = ' the sever accept'.format(ctime())
                     tcpAcceptSock.send(msg.encode() )
              tcpAcceptSock.close()
       tcpSock.close()
def UpdateSql():
       ##连接服务器
       DB = pymysql.connect("123.206.178.103","root","040912240sc","ImpedanceInfo")
       ##游标
       cursor = DB.cursor
       #循环插入
       BufSize = 60
       while i < BufSize: 
              Sql = "insert impedance_value(Impedance_Imag,Impedance_Real)values(%d,%d)where Impedance_Num = %d"%(Impedance_Real[i],Impedance_Imag[i],Impedance_Num[i])
              try:
                     cursor.execute(Sql)
                     DB.commit()
              except:
                     DB.rollback()
              i = i+1
       ##关闭数据库连接
       DB.close()

def UpdateLogfile():
       i = 0
       fp = open("Logfile.txt",'a',encoding = 'UTF-8')
       fp.write("----------------------------------------\n")
       fp.write("    日志时间：%s             ",time())
       fp.write("-----------------------------------------\n")
       fp.write("实部    虚部")
       while i < 60:
              fp.write("%d    %d\n",Impedance_Real[i],Impedance_Imag[i])
       fp.close()
def StringtoInt():
       

       
if __name__ == '__main__':
       
       GetSock()
       UpdateSql()
       UpdateLogfile()
       
       
