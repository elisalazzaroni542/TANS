.PHONY: list clean compile sim reco analysis all show

SEED = 987
EVENTS = 1000000
MS = true
NOISE = true
DISTR = custom
PHICUT = 0.01
WSIZE = 0.5


MAKEFLAGS += --no-print-directory

clean:
	@echo "Cleaning files..."
	@find . -type f -name '*.o' -delete -print
	@find . -type f -name '*.tmp' -delete -print
	@find . -type f -name '*.d' -delete -print
	@find . -type f -name '*.so' -delete -print
	@find . -type f -name '*.pcm' -delete -print
	@find . -type f -name '*_ACLiC_*' -delete -print
	@echo "Done."


list:
	@echo "Available commands:"
	@grep -E '^[a-z]+:' Makefile | sed 's/:.*//' | sort


compile:
	@echo "Compiling files..."
	@bash ./bash/compile.sh 
	@echo 'Done.'

sim: 
	@bash -c 'cd macros/ && root -b -l -q "sim.C+($(SEED), $(EVENTS), $(MS), $(NOISE), \"$(DISTR)\")"'


reco:
	@bash -c 'cd macros/ && root -b -l -q "reco.C+($(EVENTS), $(PHICUT), $(WSIZE), $(NOISE))"'


analysis:
	@bash -c 'cd macros/ && root -b -l -q "analysis.C+($(EVENTS), \"$(DISTR)\")"'
	@make show

	
all:
	@make clean
	@make compile
	@make sim
	@make reco
	@make analysis


run:
	@make sim
	@make reco
	@make analysis



show:
	@open -a preview ./plots/* || xdg-open  $(shell find ./plots -type f -name '*.png' | sort | head -n 1)

