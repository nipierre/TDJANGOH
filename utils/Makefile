CX=g++

CCFLAGS = -g -O1 -W -Wall #-pedantic -fPIC
ROOTFLAGS = `root-config --cflags --glibs`
ROOTVERSION = -D ROOT5

all: pdist rc_calc lowQ2

pdist:: pdist.cc
	$(CX) $(CCFLAGS) $(ROOTFLAGS) $(ROOTVERSION) -o $@ $< 
	mv pdist ../bin

rc_calc:: rc_calc.cc
	$(CX) $(CCFLAGS) $(ROOTFLAGS) $(ROOTVERSION) -o $@ $<
	mv rc_calc ../bin

lowQ2:: lowQ2.cc
	$(CX) $(CCFLAGS) $(ROOTFLAGS) $(ROOTVERSION) -o $@ $<
	mv lowQ2 ../bin

clean:
	rm -rf *.o

