#copyright@tarchcloud.com reserved, 2016
#history:
#2016.08.26, created by Jason.Chen

import RPi.GPIO as GPIO
import time

POWERLED = 11
SLEEPTIME=1

#init GPIO mode
def InitLed(nLed=POWERLED):
	print('InitLed') ;
	GPIO.setmode(GPIO.BOARD) ;
	GPIO.setup(nLed,GPIO.OUT) ;

def LedBlink(Count,Led=POWERLED,SleepTime=SLEEPTIME):
	print('LedBlind') 
	LenNumber = Led ;
	InitLed(LenNumber)
	i = 0 ;
	while i < Count:
		print 'i:',i
 		GPIO.output(LenNumber,GPIO.HIGH)
 		time.sleep(SleepTime)
 		GPIO.output(LenNumber,GPIO.LOW) 
 		time.sleep(SleepTime)
 		i = i+1

 	print 'led blink done'
 	return 