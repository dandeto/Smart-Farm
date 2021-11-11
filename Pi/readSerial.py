import serial

with serial.Serial('/dev/ttyUSB0', 115200, timeout=1) as ser:
	while(1):
		line = ser.readline() # read a '\n' terminated line
		if(line):
			print(line) # print to terminal
