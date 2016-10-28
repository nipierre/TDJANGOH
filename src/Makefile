Objs            = test.o
CC              = gcc
CXX             = g++
CCFlags         = -g -O1 -Wall -fPIC
ROOTFLAGS 	= `root-config --cflags`
INCFLAGS	= -I../Djangoh_Interface/
LDFLAGS		= `root-config --libs` -L/afs/cern.ch/user/n/nipierre/workspace/TGEANT_DJANGOH/lib -lDjangoh
#DevFlags	=

%.o: %.cc
	$(CXX) $(CCFlags) $(ROOTFLAGS) $(INCFLAGS) -c -o $@ $< $(LDFLAGS)


test: $(Objs)
	$(CXX) $(CCFlags) $(ROOTFLAGS) $(INCFLAGS) -o $@ $< $(LDFLAGS)
	mv test ../bin

clean:
	rm -f *.o
