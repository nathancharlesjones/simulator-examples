from PyQt5 import QtWidgets, QtCore, QtGui
from pyqtgraph import PlotWidget, plot
import pyqtgraph as pg
import traceback
import sys
import os
import serial

class MainWindow(QtWidgets.QMainWindow):
    def __init__(self, *args, **kwargs):
        super(MainWindow, self).__init__(*args, **kwargs)

        tapBtn = QtWidgets.QPushButton("Double tap")
        tapBtn.clicked.connect(self.tapBtnPressed)

        self.portLabel = QtWidgets.QLabel("Serial port")
        #portLabel.setText("Serial port")

        self.port = QtWidgets.QLineEdit()
        self.port.editingFinished.connect(self.newPort)

        #self.output = QtWidgets.QLabel("Waiting for first message...")

        #self.button = QtWidgets.QPushButton("Button", self)
        #self.button.clicked.connect(self.pressed)

        self.xSlider = QtWidgets.QSlider(QtCore.Qt.Horizontal)
        self.xSlider.setMinimum(-10)
        self.xSlider.setMaximum(10)
        self.xSlider.setValue(0)
        self.xSlider.valueChanged.connect(lambda:self.newSliderVal(self.xSlider, self.xValue))
        self.xLabel = QtWidgets.QLabel("X")
        self.xValue = QtWidgets.QLineEdit(str(self.xSlider.value()))
        self.xValue.editingFinished.connect(lambda:self.newLineEditVal(self.xSlider, self.xValue))

        self.ySlider = QtWidgets.QSlider(QtCore.Qt.Horizontal)
        self.ySlider.setMinimum(-10)
        self.ySlider.setMaximum(10)
        self.ySlider.setValue(0)
        self.ySlider.valueChanged.connect(lambda:self.newSliderVal(self.ySlider, self.yValue))
        self.yLabel = QtWidgets.QLabel("Y")
        self.yValue = QtWidgets.QLineEdit(str(self.ySlider.value()))
        self.yValue.editingFinished.connect(lambda:self.newLineEditVal(self.ySlider, self.yValue))
        
        self.zSlider = QtWidgets.QSlider(QtCore.Qt.Horizontal)
        self.zSlider.setMinimum(-10)
        self.zSlider.setMaximum(10)
        self.zSlider.setValue(0)
        self.zSlider.valueChanged.connect(lambda:self.newSliderVal(self.zSlider, self.zValue))
        self.zLabel = QtWidgets.QLabel("Z")
        self.zValue = QtWidgets.QLineEdit(str(self.ySlider.value()))
        self.zValue.editingFinished.connect(lambda:self.newLineEditVal(self.zSlider, self.zValue))

        graph = pg.PlotWidget()
        graph.setLabel('left', '% Maximum')
        graph.setLabel('bottom', 'Time (relative sec)')
        graph.setBackground('w')
        graph.addLegend()

        max_time = 0
        min_time = -5
        period_ms = 10
        buffer_size = int((max_time - min_time)*(1000/period_ms))
        increment = (max_time - min_time)/buffer_size
        self.time_vals = [0] * buffer_size
        for idx in range(len(self.time_vals)):
            self.time_vals[idx] = -5 + idx * increment
        self.motorSpeed = [0] * buffer_size
        self.ledBrightness = [0] * buffer_size
        self.motorSpeedLine = graph.plot(self.time_vals, self.motorSpeed, name="Motor speed", pen=pg.mkPen(color=(255, 0, 0)))
        self.ledBrightnessLine = graph.plot(self.time_vals, self.ledBrightness, name="LED brightness", pen=pg.mkPen(color=(0, 0, 255)))

        layout = QtWidgets.QGridLayout()
        layout.addWidget(self.portLabel,    1, 1, 1, 2)
        layout.addWidget(self.port,         2, 1, 1, 2)
        layout.addWidget(tapBtn,            3, 1, 1, 2)
        layout.addWidget(self.xLabel,       4, 1)
        layout.addWidget(self.xValue,       4, 2)
        layout.addWidget(self.xSlider,      5, 1, 1, 2)
        layout.addWidget(self.yLabel,       6, 1)
        layout.addWidget(self.yValue,       6, 2)
        layout.addWidget(self.ySlider,      7, 1, 1, 2)
        layout.addWidget(self.zLabel,       8, 1)
        layout.addWidget(self.zValue,       8, 2)
        layout.addWidget(self.zSlider,      9, 1, 1, 2)
        layout.addWidget(graph,             1, 3, 8, 8)

        self.widget = QtWidgets.QWidget()            
        self.widget.setLayout(layout)
        self.setCentralWidget(self.widget)

        self.serial = serial.Serial()

        # A more elegant solution would use threads
        #   - https://stackoverflow.com/questions/6783194/background-thread-with-qthread-in-pyqt
        #   - https://realpython.com/python-pyqt-qthread/
        #   - https://stackoverflow.com/questions/847924/how-can-i-find-out-when-a-pyqt-application-is-idle
        self.timer = QtCore.QTimer()
        self.timer.setInterval(10)
        self.timer.timeout.connect(self.checkSerial)
        self.timer.start()

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
        #print("Pressed!")
        if self.serial.isOpen():
            self.serial.write("t\n".encode('utf-8'))

    def checkSerial(self):
        if self.serial.isOpen():
            while self.serial.inWaiting() > 0:
                text = ""
                c = self.serial.read().decode('utf-8')
                while(c != '\n'):
                    text += c
                    c = self.serial.read().decode('utf-8')
                text = text.split(" ")
                #print("Received: '{0}'".format(text))
                match text[0]:
                    case "r":
                        msg = "a {0} {1} {2}\n".format(self.xValue.text(), self.yValue.text(), self.zValue.text())
                        #print("Received 'r'. Sending '{0}'".format(msg))
                        self.serial.write(msg.encode('utf-8'))
                    case "m":
                        #print("Received 'm {0}'".format(float(text[1])))
                        self.motorSpeed = self.motorSpeed[1:]
                        self.motorSpeed.append(float(text[1]))
                        self.motorSpeedLine.setData(self.time_vals, self.motorSpeed)
                    case "l":
                        #print("Received 'l {0}'".format(float(text[1])))
                        self.ledBrightness = self.ledBrightness[1:]
                        self.ledBrightness.append(float(text[1]))
                        self.ledBrightnessLine.setData(self.time_vals, self.ledBrightness)

            '''
            text = ""
            received = self.serial.inWaiting()
            if received > 0:
                while received > 0:
                    text += self.serial.read(received).decode('utf-8')
                    received = self.serial.inWaiting()
                if text[-1] == '\n':
                    text = text[:-1]
                #print(text)
                self.output.setText(text)
                '''
 
try:
    app = QtWidgets.QApplication(sys.argv)
    w = MainWindow()
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