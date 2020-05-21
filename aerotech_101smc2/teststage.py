import serial
port = serial.Serial("COM3", baudrate=57600)
port.write(b"H\n")
port.write(b"Q\n")
while port.readline().strip()=='B':
    port.write(b"Q\n")
port.write(b"M 100\n")