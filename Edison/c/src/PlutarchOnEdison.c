/*
* Copyright(c) 2016 www.tarchcloud.com. All rights reserved 
* Created by Jason.Chen
*/


#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

#include "../include/opera.h"
#include "../include/command.h"
#include "../include/led.h"

char SERVER_IP[] = "139.196.211.136" ;
const int PORT = 9003 ;
const int SELECT_SEC = 1 ;
const int USELECT_SEC=500 ;
const int BUFFER_SIZE = 255 ;
const char COMMAND_PAY[] = "pay" ;
const char COMMAND_BLINK[] = "blink" ;

char DEVICEID[] = "5ad64be6acff213d6d32000489dc74ad" ;
char PACKAGEID[] = "cb6d4e29eaef5580da3d" ;


int main() {
  /* 
  	Setup your example here, code that should run once
   */


  /* 
  	Code in this loop will run repeatedly
   */

	struct timeval tv ;
	char szRecvMsg[BUFFER_SIZE] ;
	//char *pCommand;
	int Socket_Client = CreateSocket(SERVER_IP,PORT) ;

	if(Socket_Client < 0)
	{
		perror("connect") ;
		exit(-1) ;
	}

	fd_set Client_fd_set ;

	char szCommand[BUFFER_SIZE] ;
	CreateOnLine(DEVICEID,szCommand) ;

	printf(">>COMMAND: %s\n", szCommand);
	//if(send(Socket_Client,szCommand,sizeof(szCommand),0) == -1)
	if(send(Socket_Client,szCommand,strlen(szCommand),0) == -1)
	{
		printf("%s\n", "send error");
	}

	tv.tv_sec = SELECT_SEC  ;
	tv.tv_usec = USELECT_SEC ;
	long nRead = SelectRead(Socket_Client,Socket_Client+1,&Client_fd_set,&tv,szRecvMsg,BUFFER_SIZE) ;
	printf("<<OnLine result:%s\n",szRecvMsg );

	tv.tv_sec = SELECT_SEC ;
	tv.tv_usec = USELECT_SEC ;

	while(1)
	{
		sleep(1) ;

		memset(szCommand,0,255) ;
		char szTmp[100] ;
		sprintf(szTmp,"%d",rand()%100) ;
		char *szUploadData[]={PACKAGEID,"0",szTmp,"abc"} ;
		strcpy(szCommand,CreateUpload(szUploadData,4)) ;

		if(send(Socket_Client,szCommand,strlen(szCommand),0) == -1)
		{
			printf(">>%s\n", "send error");
		}

		nRead = SelectRead(Socket_Client,Socket_Client+1,&Client_fd_set,&tv,szRecvMsg,BUFFER_SIZE) ;
		if(nRead < 0)
		{
			break ;
		}

		memset(szRecvMsg,0,sizeof(szRecvMsg)) ;



		memset(szCommand,0,255) ;
		CreateBeat(szCommand) ;
		if(send(Socket_Client,szCommand,strlen(szCommand),0) == -1)
		{
			printf(">>%s\n", "send error");
		}

		nRead = SelectRead(Socket_Client,Socket_Client+1,&Client_fd_set,&tv,szRecvMsg,BUFFER_SIZE) ;
		if(nRead < 0)
		{
			break ;
		}
		memset(szRecvMsg,0,sizeof(szRecvMsg)) ;
		nRead = SelectRead(Socket_Client,Socket_Client+1,&Client_fd_set,&tv,szRecvMsg,BUFFER_SIZE) ;
		//printf("READ size:%d\n",nRead) ;
		if(nRead < 0)
		{
			printf(">>%s\n", "select error");
			break ;
		}else if(0 == nRead)
		{
			continue ;
		}else
		{
			char *pCommand;
			pCommand = strstr(szRecvMsg,COMMAND_PAY) ;
			if(pCommand)
			{
				printf("<<%s\n", "PAY");
				PowerLenBlink(1) ;
			}

			pCommand = NULL ;
			pCommand = strstr(szRecvMsg,COMMAND_BLINK) ;
			if(pCommand)
			{
				printf(">>%s\n", "BLINK");
				PowerLenBlink(2) ;
			}
		}
		memset(szRecvMsg,0,sizeof(szRecvMsg)) ;
	}

	close(Socket_Client) ;

  return 0;
}
