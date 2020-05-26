import serial
import time

port = serial.Serial("COM12", baudrate=57600)
port.write(b"H\n")
#port.write(b"Q\n")

#port.write(b"M 100\n")


while True:
    port.write(b"Q\n")
    time.sleep(0.1)
    strng=port.readline().strip()
    print(strng)

'''
while port.readline().strip()=='B':
    port.write(b"Q\n")
    port.write(b"M 100\n")
'''

#end
