import serial
from collections import deque
import threading
import time

ser = None

buf = deque()


def scan_and_print():
    pos = 0
    v = []

    while True:
        while len(buf) > 0:

            b = buf.pop()

            if b == 255:
                if len(v) == 3:
                    print("{0}\t{1}".format(v[0],v[1]))
                v.clear()
            else:
                v.append(b)



        time.sleep(0.1);


threading.Thread(target=scan_and_print).start()


try:
    ser = serial.Serial('/dev/tty.usbmodem14101',115200,timeout=0.1)

    while True:
        try:
            count = ser.inWaiting()
            if count == 0:
                continue
            bs = ser.read(count)
            buf.extendleft(bs)
                
        except Exception as e:
            print(e)
            pass
except Exception as e:
    print(e)
    pass    
finally:
    if ser:
        ser.close()
        


