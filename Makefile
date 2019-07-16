all: src gen utils

src::
	@echo 'Building libDjangoh.so..'
	@$(MAKE) --no-print-directory -C $@
	@echo 'libDjangoh.so built !'

gen::
	@echo 'Building generation applets..'
	@$(MAKE) --no-print-directory -C $@
	@echo 'Generation applets built !'

utils::
	@echo 'Building utilitary applets..'
	@$(MAKE) --no-print-directory -C $@
	@echo 'Utilitary applets built !'

setup::
	@mkdir lib
	@mkdir bin
	@echo 'Directories lib/ and bin/ created !'

clean:
	@$(MAKE) --no-print-directory -C src clean
	@$(MAKE) --no-print-directory -C gen clean
	@$(MAKE) --no-print-directory -C utils clean
	@(rm -f lib/* bin/*)

clfile:
	@rm TDjangoh_*.dat
