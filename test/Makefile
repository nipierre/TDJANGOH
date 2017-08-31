CC              = gcc
CXX             = g++
CCFlags         = -g -O1 -Wall -fPIC
ROOTFLAGS 	= `root-config --cflags`
INCFLAGS	= -I../include/
TDJANGOH_LIB=$(TDJANGOH)/lib
LDFLAGS		= `root-config --libs` -L$(TDJANGOH_LIB) -lDjangoh
#DevFlags	=

all: test xsgen

%.o: %.cc
	$(CXX) $(CCFlags) $(ROOTFLAGS) $(INCFLAGS) -c -o $@ $< $(LDFLAGS)

test: test.o
	$(CXX) $(CCFlags) $(ROOTFLAGS) $(INCFLAGS) -o $@ $< $(LDFLAGS)
	mv test ../bin

xsgen: xsgen.o
	$(CXX) $(CCFlags) $(ROOTFLAGS) $(INCFLAGS) -o $@ $< $(LDFLAGS)
	mv xsgen ../bin

clean:
	rm -f *.o
