import serial
import time

port = serial.Serial("COM13", baudrate=57600)
#port.write(b"H\n")
#port.write(b"Q\n")
port.write(b"M 50\n")
time.sleep(0.1)
port.close()


'''
while True:
    port.write(b"Q\n")
    time.sleep(0.1)
    strng=port.readline().strip()
    


while port.readline().strip()=='B':
    port.write(b"Q\n")
    port.write(b"M 100\n")
'''

#end
