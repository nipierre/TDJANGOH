Objs            = TDjangoh.o TMCParticle.o
CC              = gcc
CXX             = g++
CCFlags         = -g -O1 -Wall -fPIC
ROOTFLAGS 			= `root-config --cflags`
LIBFLAGS				= -L/usr/local/lib/root/
#DevFlags		=

%.o: %.cc %.h
	$(CXX) $(CCFlags) $(ROOTFLAGS) -c -o $@ $<


all: $(Objs) TDjangohCalls.h TDjangoh_inputfile.h
	$(CC) -shared -fPIC -o libDjangoh.so $(Objs) $(LIBFLAGS)
	mv libDjangoh.so ../lib

clean:
	rm -f *.o
