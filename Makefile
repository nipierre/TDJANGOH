all: src test utils

src::
	@echo 'Building libDjangoh.so..'
	@$(MAKE) -C $@
	@echo 'libDjangoh.so built !'

test::
	@echo 'Building test programs..'
	@$(MAKE) -C $@
	@echo 'Test programs built !'

utils::
	@echo 'Building utilitary programs..'
	@$(MAKE) -C $@
	@echo 'Utilitary programs built !'

setup::
	@mkdir lib
	@mkdir bin
	@echo 'Directories lib/ and bin/ created !'

clean:
	(@cd utils; @make clean)
	(@cd test; @make clean)
	(@cd src; @make clean)
	(@rm -f lib/* bin/*)

clfile:
	@rm TDjangoh_*.dat
