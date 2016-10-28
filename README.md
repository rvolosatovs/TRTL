# Description
TRTL is a model of a rescue mission robot designed during the Engineering Design(4WBB0) 2016-2017 course at TU/e. 

# Design
_(excerpt from the report)_

As using a keyboard to control the robot is not comfortable, we use a Playstation 3 joystick for that. Keypresses on joystick are translated into commands written to the serial port on the Arduino over a tcp connection.

All the functionality needed(grabbing the victim, driving etc.) is implemented on the Arduino. It continuously checks the serial port for input and executes the action requested, depending on the character received(for example: `h` to drive left, `j` to drive back, etc.).

The robot is controlled from a Linux machine(any Unix-like operating system would work). `socat` program is used to emulate a serial terminal connected to the machine, such that the tcp connection appears as `$HOME/dev/ttyACM0` on the machine and everything written to this virtual device gets written directly to the Arduino.

A program written in Go is used to translate the joystick keypresses into commands written to this virtual device. Using the `evdev` library we are able to capture the keypresses as events, process them and pass appropriate character(byte) to the virtual device(which in turn passes it to the Arduino itself).
