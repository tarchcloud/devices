#copyright@tarchcloud.com reserved, 2016
#history:
#2016.08.26, created by Jason.Chen

DEVICE_ONLINE='1001'
DEVICE_BEAT = '1004'
DEVICE_UPLOAD='1003'
DEVICE_CONTROL='1002'

DEVICE_CONTROL_BLINK='blink'
DEVICE_CONTROL_PAY_BLINK='pay' 

def GenerateDevice_OnLine(deviceId):
	return '#'+DEVICE_ONLINE + '#'+deviceId+'#'

def GenerateDevice_Bead():
	return '#'+DEVICE_BEAT + '#' ;


#	function: upload data 
#	params:
#		PackageId: packageid
#		Params:  array of upload info
#	format of command:
#		#1003#PackageId#data1#data2#data3#...#
def GenerateDevice_Upload(PackageId,Params):
	if(len(Params) <= 0):
		return False ;
	print '#'+DEVICE_UPLOAD + '#' + PackageId + '#' + '#'.join(Params) + '#'
