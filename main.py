#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import serial
import time
import atexit
import getch


SERIAL_PATH = '/dev/arduino'
BAUD_RATE = 9600
DELAY = 50/1000
STOP_COMMAND = '<S,000>'
HANDSHAKE = '@COM'
HANDSHAKE_LEN = len(HANDSHAKE)
IS_KEYBOARD = True


def exit_handler(ser):
    SendData(ser, STOP_COMMAND)    
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
    turnDelay = 2.8
    forwardDelay = 2

    # Move forward
    data = '<F,200>'
    SendData(ser, data)
    time.sleep(forwardDelay)

    # Turn 90° left
    data = '<L,255>'
    SendData(ser, data)
    time.sleep(turnDelay)

    # Move forward
    data = '<F,200>'
    SendData(ser, data)
    time.sleep(forwardDelay)

    # Turn 90° right
    data = '<R,255>'
    SendData(ser, data)
    time.sleep(turnDelay)


def keyboard_control(key):
    fast_spd = '225'
    slow_spd = '100'

    # w = Forward
    if key == 'w' :
        data = f'<F,{fast_spd}>'
    elif key == 'W' :
        data = f'<F,{slow_spd}>'
    # s = Backward
    elif key == 's' :
        data = f'<B,{fast_spd}>'
    elif key == 'S' :
        data = f'<B,{slow_spd}>'
    # d = Turn right
    elif key == 'd' :
        data = f'<R,{fast_spd}>'
    elif key == 'D' :
        data = f'<R,{slow_spd}>'
    # a = Turn left
    elif key == 'a' :
        data = f'<L,{fast_spd}>'
    elif key == 'A' :
        data = f'<L,{slow_spd}>'
    # x = quit
    elif key == 'x' :
        exit()
    # anything else = Stop
    else:
        data = STOP_COMMAND
    
    SendData(ser, data)
	

if __name__ == '__main__':
    ser = serial.Serial(SERIAL_PATH, BAUD_RATE, timeout=1)
    atexit.register(exit_handler,ser)
    ser.flush()
    PerformHandshake(ser)

    SendData(ser, STOP_COMMAND)    # format: '<D,spd>'

    while True:
        # print(ReceiveData(ser).decode().rstrip())
        if IS_KEYBOARD:
            key = getch.getche()
            keyboard_control(key)
        # SpeedTest(ser)
        # TurnTest(ser)
