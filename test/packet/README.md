uSherpa firmware packet library test 
====================================
17.07.2012 Stefan Wendler
sw@kaltpost.de

This firmware is for testing packet assembling through "libsherpacore". Test results are printed on the serial line. The firmware depends on "libserial" and "libconio" from "libemb".

This README describes, how to compile and flash the firmware under Linux.    


Project Directory Layout
------------------------

* see toplevel README


Prerequisites
-------------

* see toplevel README

Also make sure "libpacket", "libserial" and "libconio" libraries are already successfully compiled.


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

