Objs            = test.o
CC              = gcc
CXX             = g++
CCFlags         = -g -O1 -Wall -fPIC
ROOTFLAGS 	= `root-config --cflags`
INCFLAGS	= -I../include/
LDFLAGS		= `root-config --libs` -L/sps/compass/npierre/TDJANGOH/lib -lDjangoh
#DevFlags	=

%.o: %.cc
	$(CXX) $(CCFlags) $(ROOTFLAGS) $(INCFLAGS) -c -o $@ $< $(LDFLAGS)


test: $(Objs)
	$(CXX) $(CCFlags) $(ROOTFLAGS) $(INCFLAGS) -o $@ $< $(LDFLAGS)
	mv test ../bin

clean:
	rm -f *.o
