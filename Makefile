all: Djangoh_Interface src

Djangoh_Interface::
	$(MAKE) -C $@
src::
	$(MAKE) -C $@

clean:
	(cd Djangoh_Interface; make clean)
	(cd src; make clean)
	(rm -f lib/* bin/*)

clfile:
	rm luevents.dat fort.6 TDjangoh_*.dat tdjangoh_his.paw
