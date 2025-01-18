.PHONY: list clean compile sim reco analysis

SEED = 123
EVENTS = 1000000
MS = true
NOISE = true
PHICUT = 0.01
WSIZE = 0.5

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
	@echo "Compiling simulation files..."
	@bash ./bash/compile.sh 
	@echo 'Done.'

sim:
	@bash -c 'cd macros/ && root -b -l -q "sim.C+($(SEED), $(EVENTS), $(MS), $(NOISE))"'



reco:
	@bash -c 'cd macros/ && root -b -l -q "reco.C+($(EVENTS), $(PHICUT), $(WSIZE))"'


analysis:
	@bash -c 'cd macros/ && root -b -l -q "analysis.C+($(EVENTS))"'


#Explanation of Each Part:
#
#    grep -E '^[a-zA-Z_-]+:' Makefile:
#        grep: A command-line utility used to search text using patterns.
#        -E: This flag tells grep to use extended regular expressions (ERE), which allows more complex pattern matching.
#        '^[a-zA-Z_-]+:':
#            ^ anchors the pattern to the beginning of the line.
#            [a-zA-Z_-]+ matches one or more characters that can be uppercase (A-Z), lowercase (a-z), underscores (_), or hyphens (-). This pattern matches valid Makefile target names.
#            : matches the colon that follows a target name in a Makefile.
#        Purpose: This grep command searches for lines in the Makefile that start with a valid target name followed by a colon. For example, it would match clean: or all:.
#
#    | (Pipe Operator):
#        The pipe (|) takes the output of the command on the left (grep) and passes it as input to the command on the right (sed).
#
#    sed 's/:.*//':
#        sed: A stream editor for filtering and transforming text.
#        s/:.*//:
#            s stands for "substitute."
#            /:.*// is the pattern and replacement:
#                : matches the colon after the target name.
#                .* matches everything after the colon (0 or more of any character).
#                // means to replace everything matched by :.* with nothing (i.e., remove it).
#        Purpose: This sed command removes everything from the colon to the end of the line, leaving just the target names. For example, clean: becomes clean.
#
#    sort:
#        Purpose: Sorts the resulting target names alphabetically.
