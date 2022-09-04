GENSTONE_DIR = .

include build/common.mk

.PHONY: all
.DEFAULT_GOAL := all
all: $(MODULE_NAMES)

.PHONY: clean
clean: $(CLEAN_TARGETS)

.PHONY: test
test: $(TEST_TARGETS)
