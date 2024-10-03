# Simulating Your Embedded Project on Your Computer

This is the companion project for the [two-part blog series](https://www.embeddedrelated.com/showarticle/1695/) I wrote on Embedded Related of the same name.

## What is it?

In [hardware\x86](https://github.com/nathancharlesjones/simulator-examples/tree/main/hardware/x86) you'll find 8 different ways to simulate a simple embedded system on your computer. The embedded system were simulating uses an accelerometer to control the speed of a motor and the brightness of an LED.
```
┌─────────────┐  ┌──────────┐  ┌─────┐         
│Accelerometer├─►│          ├─►│Motor│         
└─────────────┘  │          │  └─────┘         
  ┌─ ── ── ── ┐  │   MCU    │  ┌───┐           
  │Serial link│─►│          ├─►│LED│           
  └ ── ── ── ─┘  │          │  └───┘           
                 └──────────┘                  
                                               
\*Serial link included in the "advanced" version
```
The motor's speed is set using one of the directions reported by the accelerometer (x, y, or z) or the total acceleration (the sum of the squares of x, y, and z). Double-tapping the accelerometer cycles which variable sets the speed (x -> y -> z -> total -> x). The LED brightness is permanently set by the total acceleration.
 
There are two versions of the application code: one [basic](https://github.com/nathancharlesjones/simulator-examples/tree/main/application/basic) and one [advanced](https://github.com/nathancharlesjones/simulator-examples/tree/main/application/advanced). The basic version tries to use the minimal amount of code to demonstrate the techniques below, to keep the clutter small. The advanced version tries to add some features to show the full capability of each technique. In our sample system, the advanced version adds an [exponentially-weighted moving average](https://blog.stratifylabs.dev/device/2022-03-02-Simple-Moving-Average-and-Exponential-Moving-Average-in-embedded-Cpp/) on the three values reported by the accelerometer and also implements a small command-line interface to allow a developer to change the max acceleration value (used for scaling the inputs to the motor and the LED), the period at which the system runs, and the weighting of the EWMA filter.

The simulation techniques shown 

## How do I build it?

Requirements:
- Configured for Linux

Clone this repository
If you have ninja installed, run ninja. Otherwise run build.sh

## How do I use it? / What are the simulations?

1. Run

<iframe src="https://giphy.com/embed/TsrNLIKucmc6IdQZ5U" width="480" height="269" style="" frameBorder="0" class="giphy-embed" allowFullScreen></iframe><p><a href="https://giphy.com/gifs/TsrNLIKucmc6IdQZ5U">via GIPHY</a></p>