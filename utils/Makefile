CX=g++

CCFLAGS = -g -O1 -W -Wall #-pedantic -fPIC
ROOTFLAGS = `root-config --cflags --glibs`
ROOTVERSION = -D ROOT5

all: pdist rc_calc TERAD_plot

pdist:: pdist.cc
	$(CX) $(CCFLAGS) $(ROOTFLAGS) $(ROOTVERSION) -o $@ $<
	mv pdist ../bin

rc_calc:: rc_calc.cc
	$(CX) $(CCFLAGS) $(ROOTFLAGS) $(ROOTVERSION) -o $@ $<
	mv rc_calc ../bin

TERAD_plot:: TERAD_plot.cc
	$(CX) $(CCFLAGS) $(ROOTFLAGS) $(ROOTVERSION) -o $@ $<
	mv TERAD_plot ../bin

clean:
	rm -rf *.o
