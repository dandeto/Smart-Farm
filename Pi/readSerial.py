import serial

with serial.Serial('COM4', 115200, timeout=1) as ser:
	line = ser.readline()   # read a '\n' terminated line
	print(line)
