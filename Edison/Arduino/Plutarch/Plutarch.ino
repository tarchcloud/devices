/*
* Copyright(c) 2016 www.tarchcloud.com. All rights reserved 
* Created by Jason.Chen
*/


#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

const int PORT = 9003 ;                 //  tcp port
char SERVER_IP[] = "139.196.211.136" ;  //  tcp ip
const int SELECT_SEC = 1 ;
const int USELECT_SEC=500 ;
const int BUFFER_SIZE = 255 ;

const char COMMAND_PAY[] = "pay" ;      //command line
const char COMMAND_BLINK[] = "blink" ;  //command line

char DEVICEID[] = "5ad64be6acff213d6d32000489dc74ad" ;  //deviceid
char PACKAGEID[] = "cb6d4e29eaef5580da3d" ;             //数据接口ID  packageid


int led=13 ;                                        

#define BUFFIZE_SIZE 255
const int DEVICE_ONLINE= 1001 ;
const int DEVICE_BEAT =  1004 ;
const int DEVICE_UPLOAD= 1003 ;
const int DEVICE_CONTROL= 1002 ;


void CreateOnLine(char *szDeviceId,char *szCommand)
{
  if(!szDeviceId)
  {
    return  ;
  }

  sprintf(szCommand,"#%d#%s#",DEVICE_ONLINE,szDeviceId) ;

  return  ;
}

void  CreateBeat(char *szCommand)
{
  if(!szCommand)
    return  ;

  sprintf(szCommand,"#%d#",DEVICE_BEAT) ;

  return  ;
}

char *CreateUpload(char *szData[],int nCount)
{
  int i = 0 ;
  char szPackageData[BUFFIZE_SIZE] ;
  memset(szPackageData,0,BUFFIZE_SIZE) ;

  strcat(szPackageData,"#") ;
  for(i=0;i<nCount;i++)
  {
    strcat(szPackageData,szData[i]) ;
    strcat(szPackageData,"#") ;
  }

  static char szCommand[BUFFIZE_SIZE] ;
  
  sprintf(szCommand,"#%d%s",DEVICE_UPLOAD,szPackageData) ;

  return szCommand ;

}


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

int Socket_Client = 0 ;
fd_set Client_fd_set ;

char szCommand[BUFFER_SIZE] ;
struct timeval tv ;
char szRecvMsg[BUFFER_SIZE] ;
long nRead = 0 ;

void PowerLenBlink(int nCount)
{
  for(int i=0 ;i < nCount ;i++)
  {
    digitalWrite(led,HIGH) ;
    delay(500) ;
    digitalWrite(led,LOW) ;
    delay(500) ;
  }
}

void setup() {
  // put your setup code here, to run once:
  pinMode(led,OUTPUT) ;
  //char *pCommand;
  Socket_Client = CreateSocket(SERVER_IP,PORT) ;

  if(Socket_Client < 0)
  {
    while(1)
    {
      digitalWrite(led,HIGH) ;
      delay(1000) ;
      digitalWrite(led,LOW) ;
      delay(1000) ;
    }
    perror("connect") ;
    exit(-1) ;
  }

  CreateOnLine(DEVICEID,szCommand) ;
  if(send(Socket_Client,szCommand,strlen(szCommand),0) == -1)
  {
    while(1)
    {
      digitalWrite(led,HIGH) ;
      delay(500) ;
      digitalWrite(led,LOW) ;
      delay(500) ;
    }
  }

  tv.tv_sec = SELECT_SEC  ;
  tv.tv_usec = USELECT_SEC ;

  SelectRead(Socket_Client,Socket_Client+1,&Client_fd_set,&tv,szRecvMsg,BUFFER_SIZE) ;

  
}

void loop() {
  // put your main code here, to run repeatedly:
    while(1)
    {
      delay(1000) ;
      
      memset(szCommand,0,255) ;
      char szTmp[100] ;
      sprintf(szTmp,"%d",rand()%100) ;
      char *szUploadData[]={PACKAGEID,"0",szTmp,"abc"} ;
      strcpy(szCommand,CreateUpload(szUploadData,4)) ;
      printf("upload:%s\n",szCommand );
  
      if(send(Socket_Client,szCommand,strlen(szCommand),0) == -1)
      {
        printf("%s\n", "send error");
      }
  
      nRead = SelectRead(Socket_Client,Socket_Client+1,&Client_fd_set,&tv,szRecvMsg,BUFFER_SIZE) ;
      if(nRead < 0)
      {
        break ;
      }
  
      printf("recv: %s\n",szRecvMsg);
  
      memset(szRecvMsg,0,sizeof(szRecvMsg)) ;
  
  
  
      memset(szCommand,0,255) ;
      CreateBeat(szCommand) ;
      if(send(Socket_Client,szCommand,strlen(szCommand),0) == -1)
      {
        printf("%s\n", "send error");
      }
  
      nRead = SelectRead(Socket_Client,Socket_Client+1,&Client_fd_set,&tv,szRecvMsg,BUFFER_SIZE) ;
      if(nRead < 0)
      {
        break ;
      }
      printf("recv: %s\n",szRecvMsg);
  
      memset(szRecvMsg,0,sizeof(szRecvMsg)) ;
  
      nRead = SelectRead(Socket_Client,Socket_Client+1,&Client_fd_set,&tv,szRecvMsg,BUFFER_SIZE) ;
      //printf("READ size:%d\n",nRead) ;
      if(nRead < 0)
      {
        printf("%s\n", "select error");
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
          printf("%s\n", "PAY");
          PowerLenBlink(3) ;
        }
  
        pCommand = NULL ;
        pCommand = strstr(szRecvMsg,COMMAND_BLINK) ;
        if(pCommand)
        {
          PowerLenBlink(5) ;
        }
      }
  
      printf("recv: %s\n",szRecvMsg);
      memset(szRecvMsg,0,sizeof(szRecvMsg)) ;
    }
 }

 
