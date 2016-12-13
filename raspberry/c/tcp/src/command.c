/*
* Copyright(c) 2016 www.tarchcloud.com. All rights reserved 
* Created by Jason.Chen
*/

#include <stdlib.h>
#include "../include/command.h"

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
