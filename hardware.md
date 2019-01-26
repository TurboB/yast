# YAST - Hardware Collection

## General
 
Be carefully, the Raspberry Pi is a **3,3V** System.  
Don't connect any 5V systems direct to the I/O pins.

## Input Track Pulse Shaping

![Raspberry Pi Type B](pictures/P1080109S.JPG" ALT="Raspberry Pi Type B" )

I use a Raspberry Pi Type B, B+ is also tested. But also Type A, A+, B2 and later are usable (I think so).
On the picture it has an additional breakout PCB mounted. So is development soldering more easy.

To prevent confusion on the GPIO pin enumeration I try to disentangle these informations
about RaspberryPi hardware and WiringPi software.
Inside the yast I use the wiringPi numbering.
At the 26 Pin GPIO port of the Raspberry Pi you will find the
IO Pins on a Model B, Rev. 2, in the following way:


 
| Schematic Name | Pinheader Number | WiringPi Number | Track Number |
|:---:|:---:|:---:|:---:|
| GPIO 17 | PIN 11 | 0 | 1 |
| GPIO 18 |PIN 12 | 1 | 2 |
| GPIO 27 | PIN 13 | 2 | 3 |
| GPIO 22 | PIN 15 | 3 | 4 |
| GND  | PIN 6 | - | - |


For further information see the wiringPi project.
I use these WiringPi Number 0,1,2,3 for Track 1,2,3,4.
The Pi and the other hardware is powered over a 78S05 (5V/2A) linear voltage regulator.
For a newer Pi you need a more powerful regulator, because there are more than 2A required.
Also there is a HEF 4528 monostable multi vibrator for a defined pulse shaping used.

![Hardware principle of one input path](pictures/hardwareschematic_input.gif "Hardware principle of one input path" )

Just to show the principle of my input hardware here you see one input path from track to Pi.

![Input hardware with supply for Raspberry Pi](pictures/P1080115S.JPG "Input hardware with supply for Raspberry Pi" )
 
First prototype of pulse shaping hardware and supply connected to a Raspberry Pi.

![Emitter line and Sensor line for two tracks](pictures/P1080120S.JPG "Emitter line and Sensor line for two tracks" )

First prototypes of emitter pcb and track detector sensor pcb.
As track detector is used an IR-LED on top of the track and an IR-Photo transistor under the track.
The picture shows two prototype PCB. On top the IR-detector and down the IR-emitter.
Both were designed to cascade them as a strip over eight tracks. Additional there are
some visible LED's mounted to illuminate the point of interest for time detection.

Also all other slotcar track detector hardware, which works in the same way,
can be connected by usage of a 3.3V adaptation.

Be carefully, the Raspberry Pi is a **3,3V** System.

I use a 1 KOhm / 2 KOhm  resistor combination  to
bring down the 5V at the used input lines.

![Input Level Shifter](pictures/5V_TO_33V.PNG "Input Level Shifter" )

Level decrease with two resistors to fulfill the **3.3 V** input limitation.  
A clean schematic of my hardware will follow after more testing and optimization.......

## Input Panic Button
    
There are only future plans to do this with an external buzzer button connected to an GPIO port.
Currently panic can be used only with a keyboard.

## I2C bus - Installation and usage

For output an I2C MCP23017 or I2C SN3218 output for Traffic Lights are supported.

The I2C interface of the IO expander is connected to I2C Nr. 1 of my Raspberry Pi B (Rev. 2).

I2C installation:
<cite>gpio load i2c</cite><br>
<cite>sudo apt-get install i2c-tools</cite><br>
To check the installation and list the i2c-busses just type:  

<cite>#> i2cdetect -l</cite><br>  
and also

```markdown
#> i2cdetect -y 1
     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f
00:          -- -- -- -- -- -- -- -- -- -- -- -- --
10: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
20: -- 21 22 -- -- -- -- -- -- -- -- -- -- -- -- --
30: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
40: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
50: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
60: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
70: -- -- -- -- -- -- -- --
```

Any IO Extender based on MCP23017 with two IC's can be used. I use the IO addresses 0x21 and 0x22. The
addresses can be changed in yast_define.c.
The MCP23017, MCP23008 or the SN3218 can configured in Makefile.
There is no Traffic Light hardware urgently required to run yast.

| Schematic Name | Pinheader Number |
|:---:|:---:|
| I2C SDA | PIN 3 |
| I2C SDL | PIN 5 |
| GND | PIN 6 |

There are 20 output lines used for the starting lights, so each lamp is driven by an own line.
Five green, five yellow and ten for red. In case of a SN3218 there are five lines for red, one for
yellow and one for green.

##Output Traffic Light Hardware

![MCP23017 based TrafficLight](pictures/P1080508S.JPG "MCP23017 based TrafficLight" )

![Schematic TrafficLight](pictures/MCP23017_Ampel.GIF "Schematic TrafficLight" )

On the picture you can see my own hardware, but also these "Ready to Run" solutions from many
RPi hardware toolkits should run also. For the I2C setup please ask a search engine.

## Output and usage of Track Power Switching

The track power switching is done by switching a GPIO pin.
The pin can be chosen by config file.
With respect to single track supply and braking you have to use one relay for each track.
The relays for all tracks can be switched from one GPIO line.

For each track I use such schematic for power switching with an automotive relays.

![Track Power Switching Hardware](pictures/TPS.PNG "Track Power Switching Hardware" )

A changer relays is used to change between power and brake.  
This is used seperate for each tracks.

![four canal 3,3V to relays board"] (pictures/IMG_2740_SMALL.PNG "four canal 3,3V to relays board"")

To switch all track relays together witc one gpio chanel i use such a standard relay bank.

![30A automotive relays] (pictures/IMG_2743_SMALL.PNG" "30A automotive relays")

The power switching itself is done by a 30A automotive changer relay.

## Overview on the used GPIO pins

Last in this chapter a summary of the possible hardware connections to the Pi with YAST.

![Hardware connecting summary] (pictures/PINBELEGUNG.GIF "Hardware connecting summary")

