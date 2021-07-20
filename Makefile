include build/common.mk

ifeq ($(TEST_BUILD),1)
	TEST_BUILD_MODULES = $(wildcard test/build/*.mk)
	include $(TEST_BUILD_MODULES)
	BUILD_PREREQS += $(notdir $(subst .mk,,$(TEST_BUILD_MODULES)))
	MODULE_CLEAN_TARGETS += $(addprefix clean_,$(notdir $(subst .mk,,$(TEST_BUILD_MODULES))))
endif
ifeq ($(TEST_UNITS),1)
	TEST_UNIT_MODULES = $(wildcard test/unit/*.mk)
	include $(TEST_UNIT_MODULES)
	BUILD_PREREQS += $(notdir $(subst .mk,,$(TEST_UNIT_MODULES)))
	MODULE_CLEAN_TARGETS += $(addprefix clean_,$(notdir $(subst .mk,,$(TEST_UNIT_MODULES))))
endif

include $(SANDBOX_PROJECT_MODULE)
MODULE_CLEAN_TARGETS += $(addprefix clean_,$(notdir $(subst .mk,,$(SANDBOX_PROJECT_MODULE))))

.DEFAULT_GOAL := all
all: $(BUILD_PREREQS) $(notdir $(subst .mk,,$(SANDBOX_PROJECT_MODULE))) $(BUILD_POST)

clean: $(MODULE_CLEAN_TARGETS)
