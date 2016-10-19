Objs            = test.o
CC              = gcc
CXX             = g++
CCFlags         = -g -O1 -Wall -fPIC
ROOTFLAGS 			= `root-config --cflags`
INCFLAGS				= -I../Djangoh_Interface/
LIBFLAGS				= -L../lib/
#DevFlags		=

%.o: %.cc %.h
	$(CXX) $(INCFLAGS) $(CCFlags) $(ROOTFLAGS) -c -o $@ $<


test: $(Objs)
	$(CXX) $(INCFLAGS) $(CCFlags) $(ROOTFLAGS) -o $@ $<

clean:
	rm -f *.o
