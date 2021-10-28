# Import required libraries
import serial
import time
import struct
import csv
from datetime import datetime
import os.path

# Settings
Nodes = [0xFF]   # Node adresses to request data 
MeasPeriod = 60  # Measurement Period (s)
NumSensors = 4    # Number of sensors
Header = ["Address","Time","Temp (C)","Humidity (%)", "Pressure (hPa)", "Soil (su)"]

# Create connection to LoRa32 master node
ser = serial.Serial('/dev/ttyS0', 115200)
ser.timeout = 1

# Change address from OldAdr -> NewAdr
def Pair(OldAdr, NewAdr):
    cmd = [0xBE,0xEF,OldAdr,0x00,NewAdr];
    cmd = bytes(cmd)
    ser.write(cmd)

# Get sensor data from device at Adr
# NumDoubles is number of sensors
def GetData(Adr, NumDoubles):
    cmd = [0xBE,0xEF,Adr,0x01];
    cmd = bytes(cmd)
    ser.write(cmd)
    time.sleep(1)
    if (ser.in_waiting != NumDoubles*8):
        ser.flush()
        return GetData(Adr,NumDoubles)
    else:
        return struct.unpack(str(NumDoubles) + 'd',ser.read_all())
    
def Sleep(Adr, Duration):
    cmd = [0xBE, 0xEF, Adr, 0x02, (Duration>>8)&0xFF, Duration&0xFF]
    cmd = bytes(cmd)
    ser.write(cmd)

# Log file preperation
if not os.path.exists("./LogData.csv"):
    with open("LogData.csv", 'a', newline='') as csvfile:
        csvwriter = csv.writer(csvfile,delimiter=',')
        csvwriter.writerow(Header)
        
# Program loop
print("Gateway begin")
while True:
    for Node in Nodes:
        Data = [Node]
        Data.append(datetime.now().strftime("%m/%d/%Y, %H:%M:%S"))
        SensorData = list(GetData(Node, NumSensors))
        time.sleep(0.25)
        Sleep(Node, int(MeasPeriod*.9)) # 10% tolerance
        Data += SensorData
        with open("LogData.csv", 'a', newline='') as csvfile:
            csvwriter = csv.writer(csvfile,delimiter=',')
            csvwriter.writerow(Data)    
    time.sleep(MeasPeriod)
