uSherpa firmware
================
17.07.2012 Stefan Wendler
sw@kaltpost.de

This is the firmware project used by the uSherpa project (http://gpio.kaltpost.de). The firmware 
helps operate a MCU (currently the TI Launchpad with an MSP430G2553 installed) from a host 
connected through a serial line. For communication, the uSherpa binary format protocol.

This project heavily reuses the code code I initially wrote for the ROCKETuC project (which
was the idea of Alexander Reben). 

This README describes, how to compile the firmware binary and flash it to the target MCU under Linux.    


Project Directory Layout
------------------------

The top-level directory structure of the firmware project looks something like this:

* `bin` -              Pre-Build FW and test binaries
* `CHANGELOG` -        Firmware Change-Log
* `common_lib.mk` -    Common makefile used for library building
* `common.mk` -        Common makefile used for firmware building
* `doc` -              Common documentation
* `libemb` -           Git sub-project referencing to "libemb"
* `libsherpacore` -    uSherpa core (protocol and core functionality)
* `Makefile` -         Top-Level makefile
* `README` -           This README
* `serial-sherpa` -    Firmware which exposes the uSherpa on 1st UART
* `test` -             Various test firmwares
* `tools` -            Various tools, used mainly internally


Each library sub-project (names starting with "lib*") has the following layout:

* `doc` -               Library specific documentation
  * `gen` -         Source code documentation generated by doxygen (after "make gen-docs")
* `doxygen.conf` -      Configuration for doxygen
* `lib` -               Library in binary format (after "make")
* `Makefile` -          Library top-level makefile
* `README` -            Library specific README
* `src` -               Library sources 
  * `include` -     Library header files
  * `Makefile` -    Library makefile


Each firmware sub-project ("*-sherpa" and "test/*") have the following layout:

* `doc` -               Firmware specific documentation
  * `gen` -             Source code documentation generated by doxygen (after "make gen-docs")
* `doxygen.conf`        Configuration for doxygen
* `bin` -               Firmware in binary format (after "make")
* `Makefile` -          Firmware top-level makefile
* `README` -            Firmware specific README
* `src` -               Firmware sources 
  * `include` -         Firmware header files
  * `Makefile` -        Firmware makefile



Prerequisites
-------------

To compile all the libraries, firmware variants and test firmwares, the following mandatory tools need 
to be installed on your Linux build-host:

* msp430-gcc for compiling (I use 4.5.3 which is `available on Ubuntu as packet|TODO link to Ubuntu Wiki`)
* mspdebug for flashing (I use 0.16 which is `available on Ubuntu as packet|TODO link to Ubuntu Wiki`)
* GNU-make or make (I use GNU make 3.81 which is available also as a pecket on Ubuntu) 

To perform some of the extra tasks (like doxygen etc.), the following optional tools need to be installed on your build-host:

* doxygen for creating HTML documentation from the code annotations
* astyle for code formating
* cppcheck for static code checking


Compilation
------------

When building the firmware the first time, the git submodule for "libemb" has to be initialized (since the "libemb" subdirectory
is a stub only otherwise). To initialize the "libemb" submodule, do the following (while in the top-level directory):

	`git submodule init`

	`git submodule update`

To compile all the libraries, firmware variants and test firmware, change to the top-level directory "firmware" and issue:

	`make`

This will produce the firmware binares in various formats in the "bin" sub-directory of each variant. i
Currently the only firmware variant is the "serial-sherpa" firmware located in the directory "serial-sherpa". 
It will also produce a bunch of test firmwares located under the "test" directory. For more details on the 
firmware variants or the tests, see the corresponding README files in the sub-directories. 

NOTE: a pre-compiled version of the firmware and some of the tests could always be found in the top-level "bin" directory".


Flashing
--------

	`make flash-target`

For the MSP430 target on a Launchpad, the "mspdebug" tool is used for flashing. 


Generating API Documents
------------------------

If you installed doxygen on your system, you could generate the HTML-based API documentation by calling:

	`make gen-docs`

This will create the API documentation under "doc/gen/html" within each of the sub-directories of the libraries and 
firmwares. 


Applying Code Formating Guidelines
----------------------------------

To format all code located in "*.h" and "*.c" files, you could issu the following command in one of the sub-directories:

	`make style` 

This will use "astyle" to format the code in "stroustrup" style.  


Performing Static Code Checks
-----------------------------

To perform static code checks on all files ending with "*.h" or "*.c" , you could issue the following command while in one of the 
sub-directories:

	`make check` 

This will use "cppcheck" for static code checks and print out the results on the console.  


Tailoring the Build Settings
----------------------------

General build settings (compiler, linker, flags etc.), and settings regarding code formating and static code checks could be 
adjusted in two files on a global basis: 

`common.mk`: This is the common make file shared accross all firmware variants as well as by the test firmware. 
`common_lib.mk`: This is the commen make file shared accross all libraries. 

