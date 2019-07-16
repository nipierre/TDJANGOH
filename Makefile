all: src gen utils

src::
	@echo 'Building libDjangoh.so..'
	@$(MAKE) --no-print-directory -C $@
	@echo 'libDjangoh.so built !'

gen::
	@echo 'Building test programs..'
	@$(MAKE) --no-print-directory -C $@
	@echo 'Test programs built !'

utils::
	@echo 'Building utilitary programs..'
	@$(MAKE) --no-print-directory -C $@
	@echo 'Utilitary programs built !'

setup::
	@mkdir lib
	@mkdir bin
	@echo 'Directories lib/ and bin/ created !'

clean:
	@$(MAKE) --no-print-directory -C src clean
	@$(MAKE) --no-print-directory -C test clean
	@$(MAKE) --no-print-directory -C utils clean
	@(rm -f lib/* bin/*)

clfile:
	@rm TDjangoh_*.dat
