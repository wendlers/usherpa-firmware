BINARY	 = firmware
OBJS	+= main.o 
INCDIR  += -I../../libemb/libserial/src/include 
INCDIR  += -I../../libemb/libconio/src/include 
INCDIR  += -I../../libsherpacore/src/include 
LIBDIR  += -L../../libemb/libserial/lib
LIBDIR  += -L../../libemb/libconio/lib
LIBDIR  += -L../../libsherpacore/lib
LIBS	+= -lserial -lconio -lsherpacore

include ../../common.mk

check: $(SRC)
	$(CHECKER) $(CHECKERFLAGS) $(SRC)

gen-docs: $(HDR) $(SRC) 
	$(DOXYGEN) $(DOXYGENFLAGS)
