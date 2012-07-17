##
# Toplevel Makefile ROCKETuC firmware 
#
# 07.04.2012, Stefan Wendler, sw@kaltpost.de 
##

export TARCH=MSP430

all: target test-target 

clean: clean-lib clean-test 

lib: 
	make -C libemb
	make -C libsherpacore

target: lib
	make -C serial-sherpa 

test-target: lib
	make -C test/packet 
	make -C test/packet-serial
	make -C test/sherpacore-pin

clean-lib: 
	make -C libemb clean
	make -C libsherpacore clean
	make -C serial-sherpa clean 

clean-test: 
	make -C test/packet clean 
	make -C test/packet-serial clean
	make -C test/sherpacore-pin clean

gen-docs: lib
	make -C libemb gen-docs 
	make -C libsherpacore gen-docs 
	make -C serial-sherpa gen-docs 
