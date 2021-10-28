# This is for Austin's CurrentRanger
# We can use it for power measurements

import serial
import time

CurrentRanger = serial.Serial("COM4", 115200, timeout=1)

def GetCurrent():
	CurrentRanger.reset_input_buffer()	# Clear input buffer
	CurrentRanger.readline()			# Remove (possibly) corrupted data from clear
	i = CurrentRanger.readline()		# Read latest good data
	if (len(i) == 0):
		return "e"
	i = float(i[:-2].decode('utf-8'))
	return i

def ToggleLogging():
	CurrentRanger.write(b'u')
	CurrentRanger.readline()
	CurrentRanger.reset_input_buffer()
	
# ToggleLogging()
# while True:
	# i = GetCurrent()
	# print(i)
	# time.sleep(1)
