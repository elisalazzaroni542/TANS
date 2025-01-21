.PHONY: list clean compile sim reco analysis all show

SEED = 123
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
	@bash -c 'cd macros/ && root -b -l -q "analysis.C+($(EVENTS))"'

all:
	@make clean
	@make compile
	@make sim
	@make reco
	@make analysis
	@make show

run:
	@make sim
	@make reco
	@make analysis
	@make show

show:
	@xdg-open $(shell find plots -type f -name '*.png' | sort | head -n 1) || open -a preview $(shell find plots -type f -name '*.png' | sort | head -n 1)
