import serial
from datetime import datetime as dt

with open("recieved.txt","a") as file:
	with serial.Serial('/dev/ttyUSB0', 115200, timeout=1) as ser:
		while(1):
			line = ser.readline() # read a '\n' terminated line
			if(line):
				line = line[0:-2]
				print(line) # print to terminal
				time = dt.now()
				# file.write(str(time) + "," + str(line))
