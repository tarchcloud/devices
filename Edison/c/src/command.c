/*
* Copyright(c) 2016 www.tarchcloud.com. All rights reserved 
* Created by Jason.Chen
*/

#include <stdlib.h>
#include "../include/command.h"
#define BUFFIZE_SIZE 255
const int DEVICE_ONLINE= 1001 ;
const int DEVICE_BEAT =  1004 ;
const int DEVICE_UPLOAD= 1003 ;
const int DEVICE_CONTROL= 1002 ;


/************************************************
	函数:	
		CreateOnLine
	参数：	
		szDeviceId: 	设备ID。
		szCommand:		生成的命令字符串
	说明：
		创建发送设备上线的命令字符串
*************************************************/
void CreateOnLine(char *szDeviceId,char *szCommand)
{
	if(!szDeviceId)
	{
		return  ;
	}

	sprintf(szCommand,"#%d#%s#",DEVICE_ONLINE,szDeviceId) ;

	return  ;
}

/************************************************
	函数:	
		CreateBeat
	参数：	
		szCommand:		生成的命令字符串
	说明：
		创建发送心跳的命令字符串
*************************************************/
void  CreateBeat(char *szCommand)
{
	if(!szCommand)
		return  ;

	sprintf(szCommand,"#%d#",DEVICE_BEAT) ;

	return  ;
}

/************************************************
	函数:	
		CreateUpload
	参数：
		szData:			上传的数据	
		nCount:			szData的长度
	说明：
		创建上传数据字符串
*************************************************/
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
