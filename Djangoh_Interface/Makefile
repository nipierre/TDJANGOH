OBJSF						= TDjangoh.o TMCParticle.o djangoh_h.o djangoh_l.o djangoh_u.o djangoh_t.o \
	sophia.o pythia-6.4.24.o jetset7409.o polpdf.o gmc_random.o lhaglue.o
OBJSC						= TDjangoh.o TMCParticle.o
CC              = gcc
CXX             = g++
GFF							= gfortran
LHAPDF=/afs/cern.ch/user/n/nipierre/workspace/lhapdf5
LHAPDF_LIB=$(LHAPDF)/lib
CCFlags         = -g -O1 -fPIC -w
FFFlags					= -fno-automatic
ROOTFLAGS 	= `root-config --cflags`
INCDIR		= -I`root-config --incdir`
LDFLAGS		= `root-config --libs --evelibs --glibs` -L$(LHAPDF_LIB) -lLHAPDF -L/usr/lib64/cernlib/2006/lib -Wl,-lmathlib -lpacklib -lkernlib -Wl,-dy -llapack -lm -lnsl -lcrypt -ldl
#DevFlags	=

all: libDjangoh #libTDjangoh

libDjangoh: $(OBJSF)
	$(CC) $(CCFlags) -shared $(ROOTFLAGS) $(INCDIR) -o libDjangoh.so $(OBJSF) $(LDFLAGS)
	mv libDjangoh.so ../lib

libTDjangoh: $(OBJSC)
	$(CC) $(CCFlags) -shared $(ROOTFLAGS) $(INCDIR) -o libTDjangoh.so $(OBJSC) $(LDFLAGS) -L../lib -lDjangoh
	mv libTDjangoh.so ../lib

TDjangoh.o: TDjangoh.h TDjangohCalls.h TMCParticle.h TDjangoh_inputfile.h
	$(CC) $(CCFlags) $(ROOTFLAGS) $(INCDIR) -o TDjangoh.o -c TDjangoh.cc $(LDFLAGS)

TMCParticle.o: TMCParticle.h
	$(CC) $(CCFlags) $(ROOTFLAGS) $(INCDIR) -o TMCParticle.o -c TMCParticle.cc $(LDFLAGS)

lhaglue.o: lhaglue.f
	$(GFF) $(CCFlags) $(FFFlags) -ffree-form -c lhaglue.f -o lhaglue.o

gmc_random.o: gmc_random.f
	$(GFF) $(CCFlags) $(FFFlags) -o $@ -c $<

polpdf.o: polpdf.f
	$(GFF) $(CCFlags) $(FFFlags) -o $@ -c $<

jetset7409.o: jetset7409.f
	$(GFF) $(CCFlags) $(FFFlags) -o $@ -c $<

pythia-6.4.24.o: pythia-6.4.24.f
	$(GFF) $(CCFlags) $(FFFlags) -o $@ -c $<

sophia.o: sophia.f
	$(GFF) $(CCFlags) $(FFFlags) -o $@ -c $<

djangoh_t.o: djangoh_t.f
	$(GFF) $(CCFlags) $(FFFlags) -o $@ -c $<

djangoh_u.o: djangoh_u.f
	$(GFF) $(CCFlags) $(FFFlags) -o $@ -c $<

djangoh_l.o: djangoh_l.f
	$(GFF) $(CCFlags) $(FFFlags) -o $@ -c $<

djangoh_h.o: djangoh_h.f
	$(GFF) $(CCFlags) $(FFFlags) -o $@ -c $<

clean:
	rm -f *.o
