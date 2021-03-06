uSherpa firmware core library test
==================================
17.07.2012 Stefan Wendler
sw@kaltpost.de

This firmware is for testing "libsherpacore". Test results are printed on the serial line. The firmware depends on "libserial" and "libconio" from "libemb".

This README describes, how to compile and flash the firmware under Linux.    


Project Directory Layout
------------------------

* see toplevel README


Prerequisites
-------------

* see toplevel README

Also make sure "libsherpacore", "libserial" and "libconio" libraries are already successfully compiled. To perform all of the tests, the following needs to be wired up on the Launchpad:

* add additional LED to P2.5
* add additional button btw. P2.3 and VDD
* add additional button btw. P2.4 and GND 
* add additional 10k poti between P1.5 and Vcc
* add signal line (PWM) of servo to P2.2

For complete schematic see "msp430g2553_launchpad_testsetup.png" under "doc/schematic" in the top-level directory.


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

For the TI Launchpad, connect the USB-port to your PC. Then connect to the MCU through serial line by using the following parameters: 

* serial port (e.g. /dev/ttyACM0 on Linux)
* 9600 bauds
* 8 databits
* 1 stopbits

