# Simulating Your Embedded Project on Your Computer

This is the companion project for the [two-part blog series](https://www.embeddedrelated.com/showarticle/1695/) of the same name that I wrote for  Embedded Related.

All ASCII diagrams were made using [ASCIIflow.com](https://asciiflow.com/#/).

## What is it?

In [hardware\x86](https://github.com/nathancharlesjones/simulator-examples/tree/main/hardware/x86) you'll find several different ways to simulate a simple embedded system on your computer. The embedded system we're simulating uses an accelerometer to control both the speed of a motor and the brightness of an LED.
```
+-------------+  +----------+  +-----+         
|Accelerometer+->|          +->|Motor|         
+-------------+  |          |  +-----+         
  +- - - - - -+  |   MCU    |  +---+           
  |Serial link|->|          +->|LED|           
  + - - - - - +  |          |  +---+           
                 +----------+                  
                                               
*Serial link included in the "advanced" version
```
The motor's speed is set using one of the directions reported by the accelerometer (x, y, or z) or the total acceleration (the sum of the squares of x, y, and z). Double-tapping the accelerometer cycles which variable sets the speed (x -> y -> z -> total -> x). The LED brightness is permanently set by the total acceleration.
 
There are two versions of the application code: one [basic](https://github.com/nathancharlesjones/simulator-examples/tree/main/application/basic) and one [advanced](https://github.com/nathancharlesjones/simulator-examples/tree/main/application/advanced). The basic version tries to use the minimal amount of code to demonstrate the techniques below, to keep the clutter small. The advanced version tries to add some features to show the full capability of each technique. In our sample system, the advanced version adds an [exponentially-weighted moving average](https://blog.stratifylabs.dev/device/2022-03-02-Simple-Moving-Average-and-Exponential-Moving-Average-in-embedded-Cpp/) on the three values reported by the accelerometer and also implements a small command-line interface to allow a developer to change the max acceleration value (used for scaling the inputs to the motor and the LED), the period at which the system runs, and the weighting of the EWMA filter.

## How do I build it?

Requirements:
- Configured for Linux
- pthreads required for all simulations

Steps to build:
1. Clone this repository: `git clone --recurse-submodules -j8 https://github.com/nathancharlesjones/simulator-examples`
2. If you have ninja installed, navigate to the project folder and run `ninja`. Or execute `ninja -t targets` to see all of the possible targets and then `ninja <TARGET>` to build just one. Otherwise run `build.sh`.

## How do I use it? / What are the simulations?

### Simple Printf (Basic, Advanced)

Steps to run:
1. Run `./build/simple-printf_basic.elf` or `./build/simple-printf_advanced.elf` (the Basic version is depicted in the GIF below; you can also watch this video on YouTube [here](https://www.youtube.com/watch?v=nlTBdMFNMJs))

![](https://github.com/nathancharlesjones/simulator-examples/blob/main/media/simple-printf_basic.gif)

This first set of simulations use printf in the simplest way possible to achieve simulation: by printing a message or value to the terminal any time any output is changed. For example:
```
void setMotorSpeed(double speed)
{
    ...
    printf("Setting motor speed: %lf\n", speed);
    ...
}
```
This keeps the simulation code simple, with the downside being that reading all of the messages that are printed to the terminal can be a chore. To help, the implementations for `setMotorSpeed()` and `setLED()` only print out values at 1 Hz, despite the application task actually running at 100 Hz.

Allowable commands for the Basic version:
- `x`, `y`, or `z` + a floating-point value + Enter (e.g. `x 1.1` or `z -0.34`): Mimic a reading from the accelerometer in the direction given.
- `t` + Enter: Mimic double-tapping the accelerometer, which changes the value that is used to set the motor speed.

Allowable commands for the Advanced version:
- All of the above, plus
- `r` + Enter: Turn on/off generating random values for x, y, and z.
- `v` + a floating-point value + Enter: Set the "volatility" for random number generation (new random values are "±v" of the last value).
- `p` + a positive integer + Enter: Set the period of the main application task (in ms).
- `m` + a positive integer + Enter: Set the value of "maximum acceleration", the value against which x/y/z/total are scaled to set the motor speed and LED brightness.
- `w` + a floating-point value from 0 to 1 + Enter: Set the weighting value for the EWMA (lower values result in a stronger filter).

### Pretty Printf (Basic, Advanced_v1, Advanced_v2)

Steps to run:
1. Run `./build/pretty-printf_basic.elf`, `./build/pretty-printf_advanced_v1.elf`, or `./build/pretty-printf_advanced_v2.elf` (Advanced_v2 is shown in the GIF below; you can also watch this video on YouTube [here](https://www.youtube.com/watch?v=erZ2x05wkJA))

![](https://github.com/nathancharlesjones/simulator-examples/blob/main/media/pretty-printf_advanced_v2.gif)

This second set of applications utilize printf in a more elegant way: by defining a "background" at the top of the file with format strings in each location where we want a value printed.

- Basic:
```
char background[] = "+-----------------------------------------------------+\n\r"
                    "|Motor speed:    %12lf"     "                         |\n\r"
                    "|LED brightness: %12lf"     "                         |\n\r"
                    "+-----------------------------------------------------+\n\r";
```
- Advanced, v1:
```
char background[] = "+-----------------------------------------------------+\n\r"
                    "|X:              %12lf"     "                         |\n\r"
                    "|Y:              %12lf"     "                         |\n\r"
                    "|Z:              %12lf"     "                         |\n\r"
                    "|Values generated: %10s"    " | Volatility: %8lf"  "  |\n\r"
                    "+-----------------------------------------------------+\n\r"
                    "|Max accel:      %12lf"     "                         |\n\r"
                    "|Motor speed:    %12lf"     "                         |\n\r"
                    "|LED brightness: %12lf"     "                         |\n\r"
                    "+-----------------------------------------------------+\n\r";
```
- Advanced, v2:
```
char background[] = "+-----------------------------------------------------------------------------------------------+\n\r"
                    "|    ____  _                 _       _               _____                           _          |\n\r"
                    "|   / ___|(_)_ __ ___  _   _| | __ _| |_ ___  _ __  | ____|_  ____ _ _ __ ___  _ __ | | ___     |\n\r"
                    "|   \___ \| | '_ ` _ \| | | | |/ _` | __/ _ \| '__| |  _| \ \/ / _` | '_ ` _ \| '_ \| |/ _ \    |\n\r"
                    "|    ___) | | | | | | | |_| | | (_| | || (_) | |    | |___ >  < (_| | | | | | | |_) | |  __/    |\n\r"
                    "|   |____/|_|_| |_| |_|\__,_|_|\__,_|\__\___/|_|    |_____/_/\_\__,_|_| |_| |_| .__/|_|\___|    |\n\r"
                    "|                                                                             |_|               |\n\r"
                    "+-----------------------------------------------------------------------------------------------+\n\r"
                    "|+--------------------------+                             +----------------Total---+            |\n\r"
                    "||Values generated: %.8s"  "|                             |          +------------+v-----------+|\n\r"
                    "||Task period:  %8d"   " ms |                             +-%.7s" "->|Motor speed |LED brightns||\n\r"
                    "||Volatility:       %8lf"  "|                             |      +---+------------+------------+|\n\r"
                    "|+--------------------------+    +------------------------+---+  |Val|%12lf"     "|%12lf"     "||\n\r"
                    "||X:            %12lf"     "+--->|Exp-weighted moving average:|  +---+------------+------------+|\n\r"
                    "|+--------------------------+    | x_avg = w*x + (1-w)*x_avg  |  |Max|%12lf"     "|%12lf"     "||\n\r"
                    "||Y:            %12lf"     "+--->+----------------------------+  +---+------------+------------+|\n\r"
                    "|+--------------------------+    |w:                  %8lf"  "|  |Avg|%12lf"     "|%12lf"     "||\n\r"
                    "||Z:            %12lf"     "+--->+----------------------------+  +---+------------+------------+|\n\r"
                    "|+--------------------------+                                    |Min|%12lf"     "|%12lf"     "||\n\r"
                    "|                                                                +---+------------+------------+|\n\r"
                    "|                                  +--------------------------+            ^             ^      |\n\r"
                    "|                                  |Max accel:    %12lf"     "+------------+-------------+      |\n\r"
                    "|                                  +--------------------------+                                 |\n\r"
                    "+-----------------------------------------------------------------------------------------------+\n\r"
                    "|Application commands:                          |Simulator commands:                            |\n\r"
                    "| 't': Toggle x/y/z/total -> Motor speed        | 'r': Toggle random accel values               |\n\r"
                    "| 'm <double>': Set max accel value             | 'x <double>': Set x accelerometer value       |\n\r"
                    "| 'p <int>': Set main task period               | 'y <double>': Set y accelerometer value       |\n\r"
                    "| 'w <double>': Set EWMA coefficient            | 'z <double>': Set z accelerometer value       |\n\r"
                    "|                                               | 'v <double>': Set the 'volatility' of rand    |\n\r"
                    "+-----------------------------------------------------------------------------------------------+\n\r";
```
(I went a little crazy on that last one, but I was excited to see what I could do with this technique! I used [this website](https://www.asciiart.eu/text-to-ascii-art) to make the banner for it.)

Anytime a value is updated, we simply "redraw" the screen. For Advanced, v1:
```
void updateDisplay(void)
{
    ...
    printf(background, curr_x, curr_y, curr_z, rand_accels ? "Randomly" : "Manually", \
        volatility, getMaxAccel(), curr_motor_speed, curr_led_brightness);
}
```
Local variables hold copies of all of the values that will be printed (except for values like "maximum acceleration" that don't necessarily pass through the simulator, so "getter" functions [e.g. `getMaxAccel()`] have to be provided.)

Allowable commands are the same as for the Simple Printf simulations.

<!--
### Wokwi

Steps to run:
1. Navigate to https://wokwi.com/projects/367244067477216257 and press the "play" button

![](https://github.com/nathancharlesjones/simulator-examples/blob/main/media/wokwi.gif)

This simulation uses

### RayGUI

Steps to run:
1. Run `./build/raygui_advanced.elf`

https://community.gamedev.tv/t/raylib-on-linux/212199/2

![](https://github.com/nathancharlesjones/simulator-examples/blob/main/media/raygui.gif)

This simulation
-->

### PyQT + Virtual Serial Ports

Steps to run:
1. Create a virtual serial port.
    - Linux: Run `socat -d -d pty,rawer,echo=0 pty,rawer,echo=0`.
    - (On Windows you can use the [com2com](https://com0com.sourceforge.net/) tool, but there will be other changes you'll need to make to the code to get it to work on Windows.)
2. In a second terminal, run `./build/serial_advanced.elf <FIRST SERIAL PORT>`
3. In a third terminal, run `python3 hardware/x86/pyqt_serial.py <SECOND SERIAL PORT>`

Alternately, run all of the above commands in a single terminal window by starting them in the background (postfix each command with `&` and take note of the PID that is displayed, so that you can end the process later).

![](https://github.com/nathancharlesjones/simulator-examples/blob/main/media/pyqt_serial_advanced.gif)

This simulation is, quite possibly, the most unique of the bunch and it demonstrates that we don't even need to have our simulator running in the same executable as our application! The application program calls the hardware-dependent functions, as before, but this time the hardware-dependent functions then send and receive serial data to/from a different program in order to fulfill the requirements of each function.

![](https://github.com/nathancharlesjones/simulator-examples/blob/main/media/pyqt_serial_advanced.png)

For instance, previously our application would call `setMotorSpeed()` and this function would print something to the display:
```
// pretty-printf_advanced_v1.c
void setMotorSpeed(double speed)
{
    ...
    curr_motor_speed = speed;
    updateDisplay();
    ...
}
```
_Now_, however, this function will send that data out the serial port:
```
// serial_advanced.c
void setMotorSpeed(double speed)
{
    char buffer[32] = {0};
    sprintf(buffer, "m %lf\n", speed);
    write(serial_port, buffer, strlen(buffer));
}
```
Notice that we're constructing a "motor speed" message by writing `'m' + space + speed value + '\n'` to the serial port.

A different program (pyqt_serial_advanced.py) is written that accepts and responds to these serial commands.
```
def checkSerial(self):
    while self.serial.inWaiting() > 0:
        # Read one full message
        text = ""
        c = self.serial.read().decode('utf-8')
        while(c != '\n'):
            text += c
            c = self.serial.read().decode('utf-8')
        text = text.split(" ")
        match text[0]:
            ...
            case "m":
                # Append new value to pyqtgraph plot and adjust x/y data
                self.motorSpeedTimeVals.append(time.time())
                self.motorSpeed.append(float(text[1]))
                self.motorSpeedTimeVals = list(filter(lambda time: time > self.motorSpeedTimeVals[-1] - 5, self.motorSpeedTimeVals))
                self.motorSpeed = self.motorSpeed[-len(self.motorSpeedTimeVals):]
                self.motorSpeedLine.setData(self.motorSpeedTimeVals, self.motorSpeed)
```
This means, as can be seen here, that our simulators or GUIs don't need to be written and executed in the same programming language as our application code; we can pick any GUI we want! Furthermore, we don't even need the simulator to be running on the same machine as our application code, provided that our serial link can communicate over a network. Think about it: you could have direct control over your MCU from a remote location!