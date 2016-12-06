all: src

src::
	$(MAKE) -C $@
test::
	$(MAKE) -C $@

clean:
	(cd test; make clean)
	(cd src; make clean)
	(rm -f lib/* bin/*)

clfile:
	rm TDjangoh_*.dat
