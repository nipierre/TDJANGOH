all: src

src::
	$(MAKE) -C $@

test::
	$(MAKE) -C $@

utils::
	$(MAKE) -C $@

setup::
	mkdir lib
	mkdir bin

clean:
	(cd utils; make clean)
	(cd test; make clean)
	(cd src; make clean)
	(rm -f lib/* bin/*)

clfile:
	rm TDjangoh_*.dat
