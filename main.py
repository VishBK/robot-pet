#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import serial
import time
import atexit
import getch


""" Data is sent to the Arduino in '<D,sss>' format where:
        D   = direction (F: forward, B: backward, S: release, L: left, R: right)
        sss = speed (000-255)
"""
SERIAL_PATH = '/dev/arduino'    # USB path to connected Arduino
BAUD_RATE = 9600
DELAY = 50/1000
STOP_COMMAND = '<S,000>'
HANDSHAKE = '@COM'
HANDSHAKE_LEN = len(HANDSHAKE)
ACK_SIGNAL = '!'
IS_KEYBOARD = True
IS_CONTROLLER = False


def exit_handler(ser):
    """ Stops robot when stopping the program

    Sends the STOP_COMMAND to the Arduino when program is manually terminated
    """
    SendData(ser, STOP_COMMAND)    
    ser.flush()


def ReceiveData(ser):
    """ Returns data from the serial buffer

    Checks if there are any incoming bytes in the given serial buffer every DELAY s
    then decodes and returns the data (terminates on newline)

    Args:
        ser: Serial object to read from

    Returns:
        data from the given serial buffer
    """
    while True:
        time.sleep(DELAY)
        if ser.in_waiting > 0:
            data = ser.readline().decode().rstrip()
            # print("Arduino:", data)
            return data


def SendData(ser, data):
    """ Sends data to the serial buffer

    Waits for the ACK_SIGNAL to arrive from the Arduino, 
    then writes the encoded data to the outgoing serial buffer

    Args:
        ser: Serial object to write to
        data: String to send to the outgoing serial buffer
    """
    while True:
        ackSignal = ReceiveData(ser)
        if ackSignal == ACK_SIGNAL:
            # print("Sending:", data)
            ser.write(data.encode())
            return


def PerformHandshake(ser):
    """ Sends and checks the handshake to/from the Arduino

    Writes HANDSHAKE to the serial buffer every 1s until
    received data from the Arduino matches HANDSHAKE

    Args:
        ser: Serial object to write to
    """
    handshakeComplete = False
    while (not handshakeComplete):
        ser.write(HANDSHAKE.encode())
        print('Handshake sent')
        inData = ReceiveData(ser)
        if (inData == HANDSHAKE):
            handshakeComplete = True
            print("Pi handshake complete")
            return
        time.sleep(1)


def SpeedTest(ser):
    """ Tests speed control

    Sends commands to:
    Ramp up to max speed (255) going forward, stay at max speed for 1s, then do the same backwards

    Args:
        ser: Serial object to write to
    """
    delay = 1
    print("Moving forward")
    for i in range(255):
        data = '<F,' + str(i).zfill(3) + '>'  
        SendData(ser, data)
        # time.sleep(DELAY)
    time.sleep(delay)
    for i in range(255,0,-1):
        data = '<F,' + str(i).zfill(3) + '>'
        SendData(ser, data)
        # time.sleep(DELAY)
    
    print("Moving backward")
    for i in range(255):
        data = '<B,' + str(i).zfill(3) + '>'
        SendData(ser, data)
        # time.sleep(DELAY)
    time.sleep(delay)
    for i in range(255,0,-1):
        data = '<B,' + str(i).zfill(3) + '>'
        SendData(ser, data)
        # time.sleep(DELAY)


def TurnTest(ser):
    """ Tests turn control

    Sends commands to:
    Move forward for forwardDelay seconds, then turn 90° left, 
    move forward again, then turn 90° right

    Args:
        ser: Serial object to write to
    """
    turnDelay = 2.8
    forwardDelay = 2

    # Move forward
    print("Moving forward")
    data = '<F,200>'
    SendData(ser, data)
    time.sleep(forwardDelay)

    # Turn 90° left
    print("Turning left")
    data = '<L,255>'
    SendData(ser, data)
    time.sleep(turnDelay)

    # Move forward
    print("Moving forward")
    data = '<F,200>'
    SendData(ser, data)
    time.sleep(forwardDelay)

    # Turn 90° right
    print("Turning right")
    data = '<R,255>'
    SendData(ser, data)
    time.sleep(turnDelay)


def keyboard_control(key):
    """ Handles manual control using the keyboard

    Robot continues moving in specified direction until another command is given
    SLow and fast speeds are constant
    Controls:
        w = Forwards            Hold shift to use faster speed
        s = Backwards           x = exit
        d = Turn Right          any other key = Stop
        a = Turn Left

    Args:
        key: Typed key
    """
    fast_spd = '225'
    slow_spd = '100'

    if key == 'w' :
        data = f'<F,{fast_spd}>'
    elif key == 'W' :
        data = f'<F,{slow_spd}>'

    elif key == 's' :
        data = f'<B,{fast_spd}>'
    elif key == 'S' :
        data = f'<B,{slow_spd}>'

    elif key == 'd' :
        data = f'<R,{fast_spd}>'
    elif key == 'D' :
        data = f'<R,{slow_spd}>'

    elif key == 'a' :
        data = f'<L,{fast_spd}>'
    elif key == 'A' :
        data = f'<L,{slow_spd}>'
    
    elif key == 'x' :
        exit()
    
    else:
        data = STOP_COMMAND
    
    SendData(ser, data)
	

if __name__ == '__main__':
    ser = serial.Serial(SERIAL_PATH, BAUD_RATE, timeout=1)
    atexit.register(exit_handler,ser)
    ser.flush()
    PerformHandshake(ser)

    SendData(ser, STOP_COMMAND)

    while True:
        # print(ReceiveData(ser).decode().rstrip())
        if IS_KEYBOARD:
            key = getch.getche()    # returns and prints typed key
            keyboard_control(key)
        else:
            SpeedTest(ser)
            # TurnTest(ser)
