#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import serial
import time
import atexit

SERIAL_PATH = '/dev/arduino'
BAUD_RATE = 9600
DELAY = 50/1000
HANDSHAKE = '@COM'
HANDSHAKE_LEN = len(HANDSHAKE)


def exit_handler(ser):
    SendData(ser, '<S,000>')    
    ser.flush()


def ReceiveData(ser):
    while True:
        time.sleep(DELAY)
        if ser.in_waiting > 0:
            data = ser.readline().decode().rstrip()
            # print("Arduino:", data)
            return data


def SendData(ser, data):
    while True:
        # print("Waiting...")
        ackSignal = ReceiveData(ser)
        if ackSignal == '!':
            # print("Sending:", data)
            ser.write(data.encode())
            return


def PerformHandshake(ser):
    handshakeComplete = False
    while (not handshakeComplete):
        ser.write(HANDSHAKE.encode())
        print('Handshake sent')
        inData = ReceiveData(ser)
        # print("Data in:", inData)
        if (inData == HANDSHAKE):
            handshakeComplete = True
            print("Pi handshake complete")
            return
        time.sleep(1)


def SpeedTest(ser):
    # print("Moving forward")
    # Move forward
    for i in range(255):
        data = '<F,' + str(i).zfill(3) + '>'  
        SendData(ser, data)
        # time.sleep(DELAY)
    time.sleep(1)
    for i in range(255,0,-1):
        data = '<F,' + str(i).zfill(3) + '>'
        SendData(ser, data)
        # time.sleep(DELAY)
    
    # print("Moving backward")
    # Move backward
    for i in range(255):
        data = '<B,' + str(i).zfill(3) + '>'
        SendData(ser, data)
        # time.sleep(DELAY)
    time.sleep(1)
    for i in range(255,0,-1):
        data = '<B,' + str(i).zfill(3) + '>'
        SendData(ser, data)
        # time.sleep(DELAY)


def TurnTest(ser):
    # Turn 90° left
    data = '<L,255>'
    SendData(ser, data)
    time.sleep(3)

    # Move forward
    data = '<F,128>'
    SendData(ser, data)
    time.sleep(2)

    # Turn 90° right
    data = '<R,255>'
    SendData(ser, data)
    time.sleep(3)

    # Move forward
    data = '<F,128>'
    SendData(ser, data)
    time.sleep(2)


if __name__ == '__main__':
    ser = serial.Serial(SERIAL_PATH, BAUD_RATE, timeout=1)
    atexit.register(exit_handler,ser)
    ser.flush()
    PerformHandshake(ser)

    SendData(ser, '<S,000>')    # format: '<D,spd>'

    while True:
        # print(ReceiveData(ser).decode().rstrip())
        # SendData(ser, '<F,240>')
        # SpeedTest(ser)
        TurnTest(ser)
