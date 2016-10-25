OBJS		= TDjangoh.o TMCParticle.o
CC              = gcc
CXX             = g++
CCFlags         = -g -O1 -Wall -fPIC
ROOTFLAGS 	= `root-config --cflags`
INCDIR		= -I`root-config --incdir`
LDFLAGS		= `root-config --libs --evelibs --glibs`
#DevFlags	=

all: $(OBJS)
	$(CC) -shared $(CCFlags) $(ROOTFLAGS) $(INCDIR) -o libTDjangoh.so $(OBJS) $(LDFLAGS)
	mv libTDjangoh.so ../lib

TDjangoh.o: TDjangoh.h TDjangohCalls.h TMCParticle.h TDjangoh_inputfile.h 
	$(CC) $(CCFlags) $(ROOTFLAGS) $(INCDIR) -o TDjangoh.o -c TDjangoh.cc $(LDFLAGS)

TMCParticle.o: TMCParticle.h
	$(CC) $(CCFlags) $(ROOTFLAGS) $(INCDIR) -o TMCParticle.o -c TMCParticle.cc $(LDFLAGS)

clean:
	rm -f *.o
