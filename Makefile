NO_COLOR=\x1b[0m
OK_COLOR=\x1b[32;01m
OK_STRING=$(OK_COLOR)[OK]$(NO_COLOR)

all: src test utils

src::
	@echo 'Building libDjangoh.so..'
	@$(MAKE) --no-print-directory -C $@
	@echo "$(OK_STRING)"

test::
	@echo 'Building test programs..'
	@$(MAKE) --no-print-directory -C $@
	@echo "$(OK_STRING)"

utils::
	@echo 'Building utilitary programs..'
	@$(MAKE) --no-print-directory -C $@
	@echo "$(OK_STRING)"

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
