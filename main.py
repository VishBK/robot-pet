#!/usr/bin/env python3
import serial
import time
import atexit

SERIAL_PATH = '/dev/arduino'
BAUD_RATE = 9600
DELAY = 50/1000


def exit_handler(ser):
    ser.flush()
    SendData(ser, '<S,000>')


def RecieveData(ser):
    while True:
        if ser.in_waiting > 0:
            data = ser.readline()
            return data


def SendData(ser, data):
    ser.write(data.encode())


def SpeedTest(ser):
    # Move forward
    for i in range(255):
        data = '<F,' + str(i) + '>'  
        SendData(ser, data)
        time.sleep(DELAY)
    for i in range(255,0,-1):
        data = '<F,' + str(i) + '>'
        SendData(ser, data)
        time.sleep(DELAY)
    
    # Move backward
    for i in range(255):
        data = '<B,' + str(i) + '>'
        SendData(ser, data)
        time.sleep(DELAY)
    for i in range(255,0,-1):
        data = '<B,' + str(i) + '>'
        SendData(ser, data)
        time.sleep(DELAY)


def MovementTest(ser):
    # Move forward
    for i in range(255):
        data = '<F,' + str(i) + '>'  
        SendData(ser, data)
        time.sleep(DELAY)
    for i in range(255,0,-1):
        data = '<F,' + str(i) + '>'
        SendData(ser, data)
        time.sleep(DELAY)

    # Turn 90° right
    

if __name__ == '__main__':
    ser = serial.Serial(SERIAL_PATH, BAUD_RATE, timeout=1)
    atexit.register(exit_handler,ser)
    ser.flush()
    SendData(ser, '<S,000>')    # format: '<D,spd>'

    while True:
        # print(RecieveData(ser).decode().rstrip())
        SpeedTest(ser)
        # MovementTest(ser)
