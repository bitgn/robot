# Import libraries
from numpy import *
import numpy as np
from pyqtgraph.Qt import QtGui, QtCore
import pyqtgraph as pg
import serial
from collections import deque
import threading
import time

buf = deque()

def receive():
    ser = None
    try:
        ser = serial.Serial('/dev/tty.usbmodem14101',115200,timeout=0.1)

        v = []

        while True:
            try:
                count = ser.inWaiting()
                if count == 0:
                    time.sleep(0.001)
                    continue
                bs = ser.read(count)

                for b in bs:
                    if b == 255:
                        if len(v) == 3:
                            buf.appendleft(v[0:2])

                            while len(buf) > 10000:
                                buf.pop()

                        v.clear()
                    else:
                        v.append(b)
            except Exception as e:
                print(e)
                pass
    except Exception as e:
        print(e)
        pass
    finally:
        if ser:
            ser.close()

s = threading.Thread(target=receive)
s.setDaemon(True) # make sure we quit
s.start()


### START QtApp #####
app = QtGui.QApplication([])            # you MUST do this once (initialize things)
####################

win = pg.GraphicsWindow(title="Signal from serial port") # creates a window
p = win.addPlot(title="Realtime plot")  # creates empty space for the plot in the window
curve = p.plot()

# Realtime data plot. Each time this function is called, the data display is updated
def update():
    global curve

    x = np.array(buf)
    curve.setData(x)                     # set the curve with this data
    QtGui.QApplication.processEvents()    # you MUST process the plot now
    time.sleep(0.01)

### MAIN PROGRAM #####    
# this is a brutal infinite loop calling your realtime data plot
while True: update()

### END QtApp ####
pg.QtGui.QApplication.exec_() # you MUST put this at the end
##################
