GENSTONE_DIR = .

include build/common.mk

.PHONY: all
.DEFAULT_GOAL := all
all: $(MODULE_NAMES)

.PHONY: clean
clean: $(CLEAN_TARGETS)

.PHONY: test
test: all $(TEST_TARGETS)

# TODO: Custom CSS on Doxygen
# TODO: Generate and publish documentation automatically with GHCI
documentation: all
	doxygen build/documentation/Doxyfile
