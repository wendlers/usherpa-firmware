BINARY	 = firmware
OBJS	+= main.o serial_transport_msp430.o 
INCDIR  += -I../../libemb/libserial/src/include 
INCDIR  += -I../../libsherpacore/src/include
LIBDIR  += -L../../libemb/libserial/lib 
LIBDIR  += -L../../libsherpacore/lib
LIBS	+= -lserial -lsherpacore 

ifndef BAUDRATE 
BAUDRATE=9600
endif

CFLAGS = -DBAUDRATE_${BAUDRATE}

include ../../common.mk

check: $(SRC)
	$(CHECKER) $(CHECKERFLAGS) $(SRC)

gen-docs: $(HDR) $(SRC) 
	$(DOXYGEN) $(DOXYGENFLAGS)
