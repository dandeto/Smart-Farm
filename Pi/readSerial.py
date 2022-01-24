import serial
from datetime import datetime as dt

# Steve PC - Bus 01 - USB 005 and 007 --- /dev/bus/usb/001/005 ---- /dev/ttyUSB5"
# Change to proper USB port as needed
serialPort = serial.Serial(port="/dev/ttyUSB1", baudrate=115200, timeout=1, stopbits=serial.STOPBITS_ONE)

while(1):
    with open("recieved.csv","a") as file:
        serialInput = serialPort.readline().decode(errors='ignore')
        if serialInput:
            data = serialInput.strip()
            senderId, temp, pressure, altitude, humidity, capacitive = data.split(',')
            time = dt.now()
            file.write(f"{str(time)}, {senderId}, {temp}, {pressure}, {altitude}, {humidity}, {capacitive}\n")

