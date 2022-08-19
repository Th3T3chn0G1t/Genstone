GENSTONE_DIR = .

include build/common.mk

.PHONY: all
.DEFAULT_GOAL := all
all: $(MODULE_NAMES)
	@$(ECHO) "$(MODULE_NAMES)"
	@$(ECHO) "$(CLEAN_TARGETS)"
	@$(ECHO) "$(TEST_TARGETS)"

.PHONY: clean
clean: $(CLEAN_TARGETS)
