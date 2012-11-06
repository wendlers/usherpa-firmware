uSherpa firmware pretending being a Scratch Sensor Board (Picoboard) 
=====================================================================

26.10.2012 Stefan Wendler
sw@kaltpost.de

"scratch-sherpa" implements the protocol of the 
[Scratch Sensor Board] (http://info.scratch.mit.edu/Sensor_Board) 
(also known as Picoboard), as supported by the educational [Scratch] (http://scratch.mit.edu/) 
programming environment. The original sensor board, e. g. as offered by 
[SparcFun] (https://www.sparkfun.com/products/10311), provides 8 sensors:
a slider, a light sensor, a sound sensor, a push-button and 4 general purpose ADC inputs available 
to the user. 

The [protocol] (http://info.scratch.mit.edu/sites/infoscratch.media.mit.edu/files/file/ScratchBoard_Tech_InfoR2.pdf) 
used is very simple (much simpler then the uSherpa protocol). Scratch polls the board
regularly by sending one byte containing _0x01_. The board then sends back 9 words (18 bytes). The first
word is the header, the next 8 represent the sensor readings for each channel (including the channel ID 
from 0..7). Scratch expects the values to be 10bits wide (since the original board, as well as the MSP430
do have 10bit ADCs). Scratch then converts the raw 10bit values (ranging from 0 to 1023) to a range
between 0 and 100 for readings from an ADC, or to TRUE/FALSE for readings from an digital input. 

The baud-rate used in Scratch to communicate with the board is hard coded to 38400 Bauds within Scratch. 
Since the build in serial to USB converter of a Launchpad is only able to provide a maximum of 9600 Bauds, 
an external serial to USB converter needs to be connected to P1.1/P1.2 of the Launchpad. Every 
[FTDI] (https://www.sparkfun.com/products/718) or
[Prolific] (https://www.adafruit.com/products/954) will do the job. If you use the Raspberry Pi 
(which comes with a pre-installed Scratch version), you could directly wire the RX/TX pins from 
the Launchpad (P1.1 and P1.2) to the Pis 
[expansion header] (http://elinux.org/Rpi_Low-level_peripherals#General_Purpose_Input.2FOutput_.28GPIO.29). 

The sensor mapping chosen on the Launchpad differs slightly from what you get on the original board. 
This is mostly because the ADC channels of the Launchpad are located on port 1 (P1.0 to P1.7). Also 
the RX/TX (P1.1 and P1.2) and the build in user button (P1.3) as well as the build in LEDs (P1.0 and
P1.6) are located on port 1. Thus, when using P1.3 as the sensor board button, P1.0 as status LED 
and using P1.1/P1.2 for serial communication, only four ADC channels are left. This channels
(P1.4..7) are mapped to the user ADCs named RESA..D. As mentioned before, the build in button
on P1.3 is mapped to button. The slider is emulated through two digital buttons on P2.0 and P2.1. 
One button increases the current slider value, the other decreases it. Since I did not have light
nor sound sensors at hand, I mapped the LIGHT input to an user definable digital input (with internal
PULL-UP enabled), and the SOUND input to an ultrasonic range finder which could be read out digitally.
The complete mapping as done in "main.c" is like so: 

	#define BUTTON					PIN_1_3		// Launchpad build in button
	#define RESA					PIN_1_4		// MSP430 ADC CH4
	#define RESB					PIN_1_5		// MSP430 ADC CH5
	#define RESC					PIN_1_6     // MSP430 ADC CH6
	#define RESD					PIN_1_7     // MSP430 ADC CH7

	// simulate slider with two buttons (soft-slider)
	#define SLIDER_UP				PIN_2_0		// DIGITAL in w. internal PULL-UP
	#define SLIDER_DWN				PIN_2_1		// DIGITAL in w. internal PULL-UP

	#define LIGHT					PIN_2_2		// DIGITAL in w. internal PULL-DOWN
	#define SOUND					PIN_2_3		// DIGITAL in, floating, 
												// drive high before read (ultrasonnic sensor SRF05)


Note: Two more ADC channels could be easily freed, by not using P1.3 for the button (but e. g. 2.4), 
and not using P1.0 for the status LED (but e.g. 2.5). The pin assignments and functions could be 
easily changed in the file "src/main.c".


Hardware
---------

For an example schematic of the board I use see ./doc/msp430g2553_launchpad_sensorboard.png.

The parts used are:

* 1   x MSP430 [Launchpad] (http://www.ti.com/tool/msp-exp430g2) with MSP430G2553
* 4   x [Potentiometers] (https://www.sparkfun.com/products/9939) (e.g. 10K, I used some from an old RC-radio)
* 2-4 x Input [Buttons] (https://www.sparkfun.com/products/97)
* 1   x Ultrasonic range finder (digital, e.g. [Devantech SRF05] (http://www.robotshop.com/devantech-ultrasonic-range-finder-srf05-2.html)) 
* 1   x [Bumper sensor] (http://www.watterott.com/de/Pololu-Carrier-with-Sharp-GP2Y0D810Z0F-Digital-Distance-Sensor-10cm) (I sue for LIGHT input, could also be replaced by button)


Project Directory Layout
------------------------

* see top level README


Prerequisites
-------------

* see top level README

Also make sure "libserial" and "libsherpacore" libraries are already successfully compiled.


Compilation
------------

To compile this firmware issue:

	make

This will produce the firmware "firmware.*" in various formats under the "bin" sub-directory. 


Flashing
--------

	make flash-target

For the MSP430 target on a Launchpad, the "mspdebug" tool is used for flashing. 


Generating API Documents
------------------------

* see top level README


Applying Code Formatting Guidelines
----------------------------------

* see top level README


Performing Static Code Checks
-----------------------------

* see top level README


Tailoring the Build Settings
----------------------------

* see top level README


Basic Usage
----------------------------

Connect the Launchpad with the uploaded firmware to the USB port by using an external USB to serial
converter. To acquire the sensor values from within Scratch, see the 
[Picoboard getting started guide] (http://www.picocricket.com/pdfs/Getting_Started_With_PicoBoards.pdf).
 
