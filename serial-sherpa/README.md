uSherpa firmware usable through serial port 
===========================================
17.07.2012 Stefan Wendler
sw@kaltpost.de

"serial-sherpa" implements the functionality described by the uSherpa binary protocoll 
and provides this functionality through the serial line of the MCU (the build in 
usb-2-serial converter could be used on the TI Launchpad). The firmware depends on the 
"libsherpacore" from the uSherpa firmware project and "libserial" from the "libemb" 
project.  

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

For the TI Launchpad, connect the USB-port to your PC. Then connect to the MCU through one of the client implementations (e.g. processing library or Java library) by using the following parameters: 

* serial port (e.g. /dev/ttyACM0 on Linux)
* 9600 bauds
* 8 databits
* 1 stopbits

For examples on using one of the client libraries see the wiki [@TODO].
