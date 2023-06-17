GENSTONE_DIR = .

include build/common.mk

# We've got to specify these manually to get the order right
MODULES = $(GENSTONE_DIR)/genstone/gentests.mk \
			$(GENSTONE_DIR)/genstone/gencore.mk \
			$(GENSTONE_DIR)/genstone/genbackends.mk
MODULE_NAMES = $(subst $(GENSTONE_DIR)/genstone/,,$(subst .mk,,$(MODULES)))
CLEAN_TARGETS = $(addprefix clean_,$(MODULE_NAMES)) clean_common
TEST_TARGETS = $(addprefix test_,$(MODULE_NAMES))

include $(MODULES)

.PHONY: all
.DEFAULT_GOAL := all
all: $(MODULE_NAMES)

.PHONY: clean
clean: $(CLEAN_TARGETS)

.PHONY: test
test: all $(TEST_TARGETS)
