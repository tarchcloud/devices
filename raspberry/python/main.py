#copyright@tarchcloud.com reserved, 2016
#history:
#2016.08.26, created by Jason.Chen

import led.led as PowerLed
import tcp.tcp_client as tcp 
import sys
import command
import time


SERVER_ADDRESS='139.196.211.136'
SERVER_PORT=9003

DEVICE_ID='5ad64be6acff213d6d32000489dc74ad'

PACKAGEID='cb6d4e29eaef5580da3d' ;

RETURNOK='OK'
BUFFER_SIZE=255 

def CheckReturn(ReturnStr):
	print 'strRecv:' + ReturnStr ;
	if(len(ReturnStr) <= 0):
		print 'Recv error:' + ReturnStr ;
		return False ;
	if( ReturnStr.find(RETURNOK) == -1):
		print 'Recv error'
		return False

	return True 


if __name__ == "__main__":
	print 'command:',command.DEVICE_ONLINE ;
	print 'main'
	if False == tcp.tcpConnect(SERVER_ADDRESS,SERVER_PORT) :
		print 'connnect err'
		sys.exit(0)
	print 'connect success'

#
#	Process of upload data
#		1. 	send command of device on line
#		2.  send beat in 30s,or device will offline
#		3.  send data 
#
#	generate command of on_line

	strCommandOnline = command.GenerateDevice_OnLine(DEVICE_ID) ;
	tcp.send(strCommandOnline) ;
	print strCommandOnline 
	while(1):
		#print 'command'
		szCommand = tcp.tcpSelect(BUFFER_SIZE) ;
		if szCommand.strip():
			if(szCommand.find(command.DEVICE_CONTROL_BLINK) > 0) :
				PowerLed.InitLed() ;
				PowerLed.LedBlink(1) ;
			elif (szCommand.find(command.DEVICE_CONTROL_PAY_BLINK) > 0) :
				PowerLed.InitLed() ;
				PowerLed.LedBlink(2) ;

		print szCommand ;
		strCommandOnline = command.GenerateDevice_Bead() ;
		tcp.send(strCommandOnline) ;
		szCommand = tcp.tcpSelect(BUFFER_SIZE) ;
		print szCommand ;

	tcp.tcpClose() ;


