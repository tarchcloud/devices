/*
* Copyright(c) 2016 www.tarchcloud.com. All rights reserved 
* Created by Jason.Chen
*/

#include "../include/opera.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

/************************************************
	函数:	
		CreateSocket
	参数：
		szServerIP：	服务器ip
		nPort:	端口
	说明：
		创建socket
*************************************************/
int CreateSocket(char *szServerIP, int nPort)
{
	int nClient = -1 ;
	struct sockaddr_in ServerAddr;

	nClient = socket(AF_INET,SOCK_STREAM,0) ; 
	memset(&ServerAddr,0,sizeof(ServerAddr)) ;

	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port=htons(nPort) ;
	ServerAddr.sin_addr.s_addr = inet_addr(szServerIP) ;

	if(connect(nClient,(struct sockaddr *)&ServerAddr,sizeof(ServerAddr)) < 0)
	{
		nClient = -1 ;
	}

	return nClient  ;
}


int CloseSocket(int nSocketfd)
{
	return close(nSocketfd) ;
}


long  SelectRead(int nSocketfd,int nMaxFd,fd_set *fdRead,struct timeval *timeout,char *szRecv,int nRecvBuf)
{
	int nReturn = -1 ;
	FD_ZERO(fdRead) ;
	FD_SET(nSocketfd,fdRead) ;

	struct timeval tv ;
	tv.tv_sec = timeout->tv_sec  ;
	tv.tv_usec = timeout->tv_usec ;

	int nSelect = select(nMaxFd,fdRead,NULL,NULL,&tv) ;
	//printf("select :%d\n",nSelect );

	if(-1 == nSelect )
	{

	}else if(0 == nSelect)
	{
		nReturn = 0 ;
	}else
	{
		if(FD_ISSET(nSocketfd,fdRead))
		{
			long byte_readNumber = recv(nSocketfd,szRecv,nRecvBuf,0) ;
				
			if(byte_readNumber > 0)
			{
				if(byte_readNumber > nRecvBuf)
				{
					byte_readNumber = nRecvBuf ;
				}

				szRecv[nRecvBuf - 1] = '\0' ;

			}else if(byte_readNumber < 0)
			{
				printf("%s\n","接受消息出错" );
			}else
			{
				printf("%s\n", "服务器退出");
			}

			nReturn = byte_readNumber ;
		}
	}

	return nReturn ;
}

