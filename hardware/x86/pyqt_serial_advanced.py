from PyQt5 import QtWidgets, QtCore, QtGui
from pyqtgraph import PlotWidget, plot
import pyqtgraph as pg
import traceback
import sys
import os
import serial
import random
import time

class MainWindow(QtWidgets.QMainWindow):
    def __init__(self, *args, **kwargs):
        super(MainWindow, self).__init__()

        self.serial = serial.Serial(args[0][1])
        self.portLabel = QtWidgets.QLabel("Serial port: {0}".format(args[0][1]))

        random.seed()

        self.port = QtWidgets.QLineEdit()
        self.port.editingFinished.connect(self.newPort)

        tapBtn = QtWidgets.QPushButton("Double tap")
        tapBtn.clicked.connect(self.tapBtnPressed)

        periodLabel = QtWidgets.QLabel("Period (ms)")
        self.periodValue = QtWidgets.QLineEdit("10")
        self.serial.write("p {0}".format(int(self.periodValue.text())).encode('utf-8'))
        self.periodValue.editingFinished.connect(self.newPeriod)

        self.randomBtn = QtWidgets.QPushButton("Random values")
        self.randomBtn.setCheckable(True)

        volatilityLabel = QtWidgets.QLabel("Volatility")
        self.volatilityValue = QtWidgets.QLineEdit("0.1")

        self.xSlider = QtWidgets.QSlider(QtCore.Qt.Horizontal)
        self.xSlider.setMinimum(-10)
        self.xSlider.setMaximum(10)
        self.xSlider.setValue(0)
        self.xSlider.sliderMoved.connect(lambda:self.newSliderVal(self.xSlider, self.xValue))
        self.xLabel = QtWidgets.QLabel("X")
        self.xValue = QtWidgets.QLineEdit(str(self.xSlider.value()))
        self.xValue.editingFinished.connect(lambda:self.newLineEditVal(self.xSlider, self.xValue))

        self.ySlider = QtWidgets.QSlider(QtCore.Qt.Horizontal)
        self.ySlider.setMinimum(-10)
        self.ySlider.setMaximum(10)
        self.ySlider.setValue(0)
        self.ySlider.sliderMoved.connect(lambda:self.newSliderVal(self.ySlider, self.yValue))
        self.yLabel = QtWidgets.QLabel("Y")
        self.yValue = QtWidgets.QLineEdit(str(self.ySlider.value()))
        self.yValue.editingFinished.connect(lambda:self.newLineEditVal(self.ySlider, self.yValue))
        
        self.zSlider = QtWidgets.QSlider(QtCore.Qt.Horizontal)
        self.zSlider.setMinimum(-10)
        self.zSlider.setMaximum(10)
        self.zSlider.setValue(0)
        self.zSlider.sliderMoved.connect(lambda:self.newSliderVal(self.zSlider, self.zValue))
        self.zLabel = QtWidgets.QLabel("Z")
        self.zValue = QtWidgets.QLineEdit(str(self.ySlider.value()))
        self.zValue.editingFinished.connect(lambda:self.newLineEditVal(self.zSlider, self.zValue))

        weightingLabel = QtWidgets.QLabel("EWMA weighting")
        self.weightingValue = QtWidgets.QLineEdit("0.1")
        self.serial.write("w {0}".format(float(self.weightingValue.text())).encode('utf-8'))
        self.weightingValue.editingFinished.connect(self.newWeighting)

        maxAccelLabel = QtWidgets.QLabel("Max accel")
        self.maxAccelValue = QtWidgets.QLineEdit("10")
        self.serial.write("m {0}".format(int(self.maxAccelValue.text())).encode('utf-8'))
        self.maxAccelValue.editingFinished.connect(self.newMaxAccel)

        graph = pg.PlotWidget()
        graph.setLabel('left', '% Maximum')
        graph.setLabel('bottom', 'Clock time (sec)')
        graph.setBackground('w')
        graph.addLegend()

        self.motorSpeed = []
        self.motorSpeedTimeVals = []
        self.ledBrightness = []
        self.ledBrightnessTimeVals = []
        self.motorSpeedLine = graph.plot(self.motorSpeedTimeVals, self.motorSpeed, name="Motor speed", pen=pg.mkPen(color=(255, 0, 0)))
        self.ledBrightnessLine = graph.plot(self.ledBrightnessTimeVals, self.ledBrightness, name="LED brightness", pen=pg.mkPen(color=(0, 0, 255)))

        # Desired layout:
        #    +----------------+--------------------+------------------------------------------------+
        #    |Serial label    |Serial port text box|                                                |
        #    +-------------------------------------|                                                |
        #    |          Double tap btn             |                                                |
        #    +----------------+--------------------|                                                |
        #    |Period label    |Period text box     |                                                |
        #    +-------------------------------------|                                                |
        #    |     Random values toggle btn        |                                                |
        #    +----------------+--------------------|                                                |
        #    |Volatility label|Volatility text box |                                                |
        #    +----------------+--------------------|                  Graph of                      |
        #    |X label         |X text box          |                                                |
        #    +-------------------------------------|                 motor speed                    |
        #    |             X slider                |                                                |
        #    +----------------+--------------------|                    and                         |
        #    |Y label         |Y text box          |                                                |
        #    +-------------------------------------|                LED brightness                  |
        #    |             Y slider                |                                                |
        #    +----------------+--------------------|                                                |
        #    |Z label         |Z text box          |                                                |
        #    +-------------------------------------|                                                |
        #    |             Z slider                |                                                |
        #    +----------------+--------------------|                                                |
        #    |EWMA label      |EWMA text box       |                                                |
        #    +----------------+--------------------|                                                |
        #    |Max accel label |Max accel text box  |                                                |
        #    +----------------+--------------------+------------------------------------------------+
        layout = QtWidgets.QGridLayout()
        layout.addWidget(self.portLabel,        1, 1)
        layout.addWidget(self.port,             1, 2)
        layout.addWidget(tapBtn,                2, 1, 1, 2)
        layout.addWidget(periodLabel,           3, 1)
        layout.addWidget(self.periodValue,      3, 2)
        layout.addWidget(self.randomBtn,        4, 1, 1, 2)
        layout.addWidget(volatilityLabel,       5, 1)
        layout.addWidget(self.volatilityValue,  5, 2)
        layout.addWidget(self.xLabel,           6, 1)
        layout.addWidget(self.xValue,           6, 2)
        layout.addWidget(self.xSlider,          7, 1, 1, 2)
        layout.addWidget(self.yLabel,           8, 1)
        layout.addWidget(self.yValue,           8, 2)
        layout.addWidget(self.ySlider,          9, 1, 1, 2)
        layout.addWidget(self.zLabel,           10, 1)
        layout.addWidget(self.zValue,           10, 2)
        layout.addWidget(self.zSlider,          11, 1, 1, 2)
        layout.addWidget(weightingLabel,        12, 1)
        layout.addWidget(self.weightingValue,   12, 2)
        layout.addWidget(maxAccelLabel,         13, 1)
        layout.addWidget(self.maxAccelValue,    13, 2)
        layout.addWidget(graph,                 1, 3, 13, 8)

        self.widget = QtWidgets.QWidget()            
        self.widget.setLayout(layout)
        self.setCentralWidget(self.widget)

        # Check for characters on the serial port at 100 Hz. A more elegant solution would use threads:
        #   - https://stackoverflow.com/questions/6783194/background-thread-with-qthread-in-pyqt
        #   - https://realpython.com/python-pyqt-qthread/
        #   - https://stackoverflow.com/questions/847924/how-can-i-find-out-when-a-pyqt-application-is-idle
        self.timer = QtCore.QTimer()
        self.timer.setInterval(10)
        self.timer.timeout.connect(self.checkSerial)
        self.timer.start()

    def newPeriod(self):
        self.periodValue.setText(str(int(float(self.periodValue.text()))))
        self.serial.write("p {0}\n".format(int(self.periodValue.text())).encode('utf-8'))

    def newWeighting(self):
        self.serial.write("w {0}\n".format(float(self.weightingValue.text())).encode('utf-8'))

    def newMaxAccel(self):
        self.serial.write("m {0}\n".format(float(self.maxAccelValue.text())).encode('utf-8'))

    # Each acceleration value can be set with both a slider or a text box. These functions
    # (newSliderVal and newLineEditVal) ensure both widgets stay in sync with each other
    # any time either one is updated.
    def newSliderVal(self, slider, value):
        value.setText(str(float(slider.value())))

    def newLineEditVal(self, slider, value):
        slider.setValue(int(float(value.text())))

    def newPort(self):
        self.serial.port = self.port.text()
        self.portLabel.setText("Serial port: {0}".format(self.serial.port))
        if not self.serial.isOpen():
            self.serial.open()

    def tapBtnPressed(self):
        self.serial.write("t\n".encode('utf-8'))

    def checkSerial(self):
        while self.serial.inWaiting() > 0:
            # Build string until a newline is reached
            text = ""
            c = self.serial.read().decode('utf-8')
            while(c != '\n'):
                text += c
                c = self.serial.read().decode('utf-8')

            # Create argv out of received string
            argv = text.split(" ")

            # Perform action based on first argument given
            match argv[0]:
                case "r":
                    if self.randomBtn.isChecked():
                        self.xValue.setText(str(float(self.xValue.text()) + float(self.volatilityValue.text())*((random.random()*2)-1))[:15])
                        self.newLineEditVal(self.xSlider, self.xValue)
                        self.yValue.setText(str(float(self.yValue.text()) + float(self.volatilityValue.text())*((random.random()*2)-1))[:15])
                        self.newLineEditVal(self.ySlider, self.yValue)
                        self.zValue.setText(str(float(self.zValue.text()) + float(self.volatilityValue.text())*((random.random()*2)-1))[:15])
                        self.newLineEditVal(self.zSlider, self.zValue)
                    msg = "a {0} {1} {2}\n".format(self.xValue.text(), self.yValue.text(), self.zValue.text())
                    self.serial.write(msg.encode('utf-8'))
                case "m":
                    # Append new speed and time values
                    self.motorSpeedTimeVals.append(time.time())
                    self.motorSpeed.append(float(argv[1]))

                    # Remove any time values older than 5 sec
                    self.motorSpeedTimeVals = list(filter(lambda time: time > self.motorSpeedTimeVals[-1] - 5, self.motorSpeedTimeVals))
                    
                    # Remove the appropriate number of speed values
                    self.motorSpeed = self.motorSpeed[-len(self.motorSpeedTimeVals):]

                    # Redraw the motor speed line
                    self.motorSpeedLine.setData(self.motorSpeedTimeVals, self.motorSpeed)

                case "l":
                    # Append new speed and time values
                    self.ledBrightnessTimeVals.append(time.time())
                    self.ledBrightness.append(float(argv[1]))

                    # Remove any time values older than 5 sec
                    self.ledBrightnessTimeVals = list(filter(lambda time: time > self.ledBrightnessTimeVals[-1] - 5, self.ledBrightnessTimeVals))
                    
                    # Remove the appropriate number of brightness values
                    self.ledBrightness = self.ledBrightness[-len(self.ledBrightnessTimeVals):]

                    # Redraw the LED brightness line
                    self.ledBrightnessLine.setData(self.ledBrightnessTimeVals, self.ledBrightness)

if __name__ == "__main__": 
    try:
        app = QtWidgets.QApplication(sys.argv)
        w = MainWindow(sys.argv)
        w.show()
        sys.exit(app.exec_())
    except Exception as e:
        exc_type, exc_obj, exc_tb = sys.exc_info()
        fname = os.path.split(exc_tb.tb_frame.f_code.co_filename)[1]
        print(e)
        print(exc_type, fname, exc_tb.tb_lineno)
        print(traceback.format_exc())
        while(input("Type 'q' to quit: ")!="q"):
            pass