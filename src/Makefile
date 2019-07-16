OBJSF						= TDjangoh.o TMCParticle.o djangoh_h.o djangoh_l.o djangoh_u.o djangoh_t.o \
	sophia.o pythia-6.4.24.o jetset7409.o polpdf.o gmc_random.o lhaglue.o pugixml.o
OBJSC						= TDjangoh.o TMCParticle.o
CC              = gcc
CXX             = g++
GFF							= gfortran
LHAPDF5_LIB=$(LHAPDF5)/lib
CCFlags         = -g -O1 -fPIC -std=c++11 -w
FFFlags					= -fno-automatic
ROOTFLAGS 	= `root-config --cflags`
INCDIR		= -I`root-config --incdir` -I../include/
ifeq ($(SITE),CCIN2P3)
LDFLAGS		= `root-config --libs --evelibs --glibs` -L$(LHAPDF5_LIB) -lLHAPDF -L/usr/lib/gcc/x86_64-redhat-linux/3.4.6/ -L/afs/in2p3.fr/cernlib/@sys/pro/lib/ -Wl,-lmathlib -lpacklib -lkernlib -Wl,-dy -lm -lnsl -lcrypt -ldl
else
ifeq ($(SITE),BW)
LDFLAGS		= `root-config --libs --evelibs --glibs` -L$(LHAPDF5_LIB) -lLHAPDF -L/projects/sciteam/balh/opt/x86_64-suse-linux-gcc49/cernlib/2005/lib/ -Wl,-lmathlib -lpacklib -lkernlib -Wl,-dy -lm -lnsl -lcrypt -ldl
else
LDFLAGS		= `root-config --libs --evelibs --glibs` -L$(LHAPDF5_LIB) -lLHAPDF -L/usr/lib/gcc/x86_64-redhat-linux/3.4.6/ -L/cvmfs/sft.cern.ch/lcg/external/cernlib/2006a/x86_64-slc6-gcc47-opt/lib -Wl,-lmathlib -lpacklib -lkernlib -Wl,-dy -lm -lnsl -lcrypt -ldl
endif
endif
#DevFlags	=

all: modules libDjangoh

libDjangoh: $(OBJSF)
	@$(CC) $(CCFlags) -shared $(ROOTFLAGS) $(INCDIR) -o libDjangoh.so $(OBJSF) $(LDFLAGS)
	@mv libDjangoh.so ../lib

modules: xSectionModule.f90
	@$(GFF) $(CCFlags) -c xSectionModule.f90

pugixml.o:
	@$(CC) $(CCFlags) $(ROOTFLAGS) $(INCDIR) -o pugixml.o -c pugixml.cpp $(LDFLAGS)

TDjangoh.o: ../include/TDjangoh.h ../include/TDjangohCalls.h ../include/TMCParticle.h
	@$(CC) $(CCFlags) $(ROOTFLAGS) $(INCDIR) -o TDjangoh.o -c TDjangoh.cc $(LDFLAGS)

TMCParticle.o: ../include/TMCParticle.h
	@$(CC) $(CCFlags) $(ROOTFLAGS) $(INCDIR) -o TMCParticle.o -c TMCParticle.cc $(LDFLAGS)

lhaglue.o: lhaglue.f
	@$(GFF) $(CCFlags) $(FFFlags) -ffree-form -c lhaglue.f -o lhaglue.o

gmc_random.o: gmc_random.f
	@$(GFF) $(CCFlags) $(FFFlags) -o $@ -c $<

polpdf.o: polpdf.f
	@$(GFF) $(CCFlags) $(FFFlags) -o $@ -c $<

jetset7409.o: jetset7409.f
	@$(GFF) $(CCFlags) $(FFFlags) -o $@ -c $<

pythia-6.4.24.o: pythia-6.4.24.f
	@$(GFF) $(CCFlags) $(FFFlags) -o $@ -c $<

sophia.o: sophia.f
	@$(GFF) $(CCFlags) $(FFFlags) -o $@ -c $<

djangoh_t.o: djangoh_t.f
	@$(GFF) $(CCFlags) $(FFFlags) -o $@ -c $<

djangoh_u.o: djangoh_u.f
	@$(GFF) $(CCFlags) $(FFFlags) -o $@ -c $<

djangoh_l.o: djangoh_l.f
	@$(GFF) $(CCFlags) $(FFFlags) -o $@ -c $<

djangoh_h.o: djangoh_h.f
	@$(GFF) $(CCFlags) $(FFFlags) -o $@ -c $<

clean:
	@rm -f *.o
