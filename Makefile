all: TDjangoh src

TDjangoh::
	$(MAKE) -C $@
src::
	$(MAKE) -C $@

clean:
	(cd TDjangoh; make clean)
	(cd src; make clean)
	(rm -f lib/* bin/*)

clfile:
	rm luevents.dat fort.6 TDjangoh_*.dat tdjangoh_his.paw
