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
	make -C scratch-sherpa 

test-target: lib
	make -C test/packet 
	make -C test/packet-serial
	make -C test/sherpacore-pin
	make -C test/fliwatuet

clean-lib: 
	make -C libemb clean
	make -C libsherpacore clean
	make -C serial-sherpa clean 
	make -C scratch-sherpa clean 

clean-test: 
	make -C test/packet clean 
	make -C test/packet-serial clean
	make -C test/sherpacore-pin clean
	make -C test/fliwatuet clean

gen-docs: lib
	make -C libemb gen-docs 
	make -C libsherpacore gen-docs 
	make -C serial-sherpa gen-docs 
	make -C scratch-sherpa gen-docs 

bin-dist: lib 
	test -d bin || mkdir bin
	make -C serial-sherpa clean
	BAUDRATE=9600 make -C serial-sherpa
	cp serial-sherpa/bin/firmware.bin bin/uSherpa-fw_serial-sherpa_9600b_${TARCH}.bin 
	cp serial-sherpa/bin/firmware.elf bin/uSherpa-fw_serial-sherpa_9600b_${TARCH}.elf 
	cp serial-sherpa/bin/firmware.hex bin/uSherpa-fw_serial-sherpa_9600b_${TARCH}.hex
	make -C serial-sherpa clean
	BAUDRATE=19200 make -C serial-sherpa
	cp serial-sherpa/bin/firmware.bin bin/uSherpa-fw_serial-sherpa_19200b_${TARCH}.bin 
	cp serial-sherpa/bin/firmware.elf bin/uSherpa-fw_serial-sherpa_19200b_${TARCH}.elf 
	cp serial-sherpa/bin/firmware.hex bin/uSherpa-fw_serial-sherpa_19200b_${TARCH}.hex
	make -C scratch-sherpa
	cp scratch-sherpa/bin/firmware.bin bin/uSherpa-fw_scratch-sherpa_38400b_${TARCH}.bin 
	cp scratch-sherpa/bin/firmware.elf bin/uSherpa-fw_scratch-sherpa_38400b_${TARCH}.elf 
	cp scratch-sherpa/bin/firmware.hex bin/uSherpa-fw_scratch-sherpa_38400b_${TARCH}.hex
