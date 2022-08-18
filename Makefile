include build/common.mk

MODULES = $(wildcard genstone/*.mk)
MODULE_NAMES = $(subst genstone/,,$(subst .mk,,$(MODULES)))
CLEAN_TARGETS = $(addprefix clean_,$(MODULE_NAMES))
TEST_TARGETS = $(addprefix test_,$(MODULE_NAMES))
include $(MODULES)

.PHONY: all
.DEFAULT_GOAL := all
all: $(MODULE_NAMES)
	@$(ECHO) "$(MODULE_NAMES)"
	@$(ECHO) "$(CLEAN_TARGETS)"
	@$(ECHO) "$(TEST_TARGETS)"

.PHONY: clean
clean: $(CLEAN_TARGETS)

lib:
	-mkdir $@
