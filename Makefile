all: Djangoh_Interface src

Djangoh_Interface::
	$(MAKE) -C $@
src::
	$(MAKE) -C $@

clean:
	(cd Djangoh_Interface; make clean)
	(rm -f lib/* bin/*)
