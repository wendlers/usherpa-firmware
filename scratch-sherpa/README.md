uSherpa firmware pretending beeing a Scratch Sensor Board (Picoboard) 
=====================================================================

26.10.2012 Stefan Wendler
sw@kaltpost.de

"scratch-sherpa" implements the protocol of the [Sensor Board] (http://info.scratch.mit.edu/Sensor_Board) 
(also knwon as Picoboard), as supported by the educational [Scratch] (http://scratch.mit.edu/) 
programming environment. The original sensor board, e. g. as offered by 
[SparcFun] (https://www.sparkfun.com/products/10311), provides 8 sensors:
a slider, a light sensor, a sound sensor, a push-button and 4 general purpose ADC inputs available 
to the user. 

The protocol used is very simple (much simpler then the uSherpa protocol). Scratch polls the the board
regularly by sending one byte containing 0x01. The board then sends back 9 words (18 bytes). The first
word is the header, the next 8 represnt the sensor readings for each channel (including the channel ID 
from 0..7). Scratch eexpects the values to be 10bit long (since the original board, as well as the MSP430
do have 10bit ADCs). Scratch then converts the raw 10bit values (ranging from 0 to 1023) to a range
value between 0 and 100. 

The baudrate used in Scratch to communicate with the board is hard coded to 38400 Bauds within Scratch. 
Since the build in serial to usb converter of a Launchpad is only able to provide a maximum of 9600 Bauds, 
an external serial to usb converter needs to be connected to P1.1/P1.2 of the Launchpad. Every 
[FTDI] (https://www.sparkfun.com/products/718) or
[Prolific] (https://www.adafruit.com/products/954) will do the job. 

Currently not all of the eight sensors are supported by this firmware. The current mapping is:


	#define BUTTON					PIN_1_3
	#define RESA					PIN_1_4
	#define RESB					PIN_1_5
	#define RESC					PIN_1_7


This README describes, how to compile and flash the firmware under Linux.    


Project Directory Layout
------------------------

* see toplevel README


Prerequisites
-------------

* see toplevel README

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

* see toplevel README


Applying Code Formating Guidelines
----------------------------------

* see toplevel README


Performing Static Code Checks
-----------------------------

* see toplevel README


Tailoring the Build Settings
----------------------------

* see toplevel README


Basic Usage
----------------------------

Connect the Launchpad with the uploaded firmware to the USB port by using an external USB to serial
converter. To acquire the sensor values from within Scratch, see the 
[Picoboard getting started guide] (http://www.picocricket.com/pdfs/Getting_Started_With_PicoBoards.pdf)
 
