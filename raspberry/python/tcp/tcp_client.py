#copyright@Catosoft.com reserved, 2016
#history:
#2016.08.26, created by Jason.Chen

import socket
import time
import fcntl,os
import select 

tcpClient=socket.socket(socket.AF_INET,socket.SOCK_STREAM) ;
#tcpClient.setblocking(0) ;


def tcpConnect(ServerAddress,ServerPort):
	bReturn = False ;
	count = 0
	ServerInfo = (ServerAddress,ServerPort) ;
	while count < 10:
		print 'reconnect count:',count
		try:
			tcpClient.connect(ServerInfo) ;
			bReturn = True ;
			break ;
		except Exception, e:
			print e ;
			time.sleep(0.1) ;
		else:
			pass
		finally:
			pass
		count=count+1 ;

	return bReturn ;

def tcpRead(nMax):
	tcpClient.settimeout(1)
	str='' ;
	#print 'test tcpRead:'
	try:
		str = tcpClient.recv(nMax) ;
		#print '*************************'
		pass
	except socket.error, e:
		print e
	else:
		pass
	finally:
		pass
	return str 

def tcpReadBlock(nMax):
	str='' ;
	#print 'test tcpRead:'
	try:
		str = tcpClient.recv(nMax) ;
		#print '*************************'
		pass
	except socket.error, e:
		print e
	else:
		pass
	finally:
		pass
	return str 

def send(szContent):
	tcpClient.send(szContent) ;

def tcpClose():
	tcpClient.close() ;


def tcpSelect(nMax):
	output=[] ;
	inputs = [tcpClient] ;
	str='' ;
	try:
		readable,writeable,exceptional = select.select(inputs,output,inputs,1) ;
	except select.error,e:
		print e ;
	if tcpClient in readable:
		str = tcpClient.recv(nMax) ;
	return str ;

