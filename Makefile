all: src

src::
	$(MAKE) -C $@
test::
	$(MAKE) -C $@
setup::
	mkdir lib
	mkdir bin

clean:
	(cd test; make clean)
	(cd src; make clean)
	(rm -f lib/* bin/*)

clfile:
	rm TDjangoh_*.dat
