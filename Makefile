include build/common.mk

VENDOR_MODULES = $(wildcard genesis/vendor/*.mk)
include $(VENDOR_MODULES)
BUILD_PREREQS += $(notdir $(subst .mk,,$(VENDOR_MODULES)))
MODULE_CLEAN_TARGETS += $(addprefix clean_,$(notdir $(subst .mk,,$(VENDOR_MODULES))))

GENESIS_MODULES = $(wildcard genesis/*.mk)
include $(GENESIS_MODULES)
BUILD_PREREQS += $(notdir $(subst .mk,,$(GENESIS_MODULES)))
MODULE_CLEAN_TARGETS += $(addprefix clean_,$(notdir $(subst .mk,,$(GENESIS_MODULES))))

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

BUILTIN_MODULES = $(wildcard build/dev/*.mk) build/docs/docs.mk
include $(BUILTIN_MODULES)

include $(SANDBOX_PROJECT_MODULE)
MODULE_CLEAN_TARGETS += $(addprefix clean_,$(notdir $(subst .mk,,$(SANDBOX_PROJECT_MODULE))))

MODULE_CLEAN_TARGETS += clean_tmpfile

.DEFAULT_GOAL := all

build_message_pre_build:
	@echo "$(SECTION_PREFIX) Pre-Build"
ifneq ($(DISABLED_MODULES),)
	@echo "$(INFO_PREFIX) Excluding disabled targets: $(addprefix \n   - ,$(DISABLED_MODULES))"
endif
ifneq ($(ERROR),)
	@echo $(ERROR)
	@exit 1
endif

ifeq ($(DISABLED_MODULES),)
ifeq ($(ERROR),)
	@echo "$(INFO_PREFIX) Nothing to do!"
endif
endif

include $(ADDITIONAL_BUILD_MODULES)
MODULE_CLEAN_TARGETS += $(addprefix clean_,$(notdir $(subst .mk,,$(ADDITIONAL_BUILD_MODULES))))

BUILD_TARGETS = \
	$(filter-out $(DISABLED_MODULES),\
		build_message_pre_build \
		$(BUILD_PREREQS) \
		$(notdir $(subst .mk,,$(ADDITIONAL_BUILD_MODULES))) \
		$(notdir $(subst .mk,,$(SANDBOX_PROJECT_MODULE))) \
		$(BUILD_POST) \
		clean_tmpfile \
	)

all: $(BUILD_TARGETS) ### @Default Builds all modules specified in `config.mk`
	@echo "$(INFO_PREFIX) All built!"

list: ### @Default Lists targets to be built for `all`
	@echo "$(INFO_PREFIX) Targets to be built for \`all\`: $(addprefix \n - $(TARGET_PREFIX),$(addsuffix $(TARGET_SUFFIX),$(BUILD_TARGETS)))"
	@echo "$(INFO_PREFIX) Excluded targets: $(addprefix \n - $(TARGET_PREFIX),$(addsuffix $(TARGET_SUFFIX),$(DISABLED_MODULES)))"
	@echo "$(NOTE_PREFIX) For a list of available targets and their descriptions, see \`make help\`"

help: ### @Default Generates this message
ifeq ($(SHELL),cmd.exe)
	@echo "$(ERROR_PREFIX) The self-documenting Makefile target does not work on Windows hosts"
else
	@echo "$(INFO_PREFIX) Makefile help\n$(NOTE_PREFIX) This list only includes descriptions for user-desireable targets$(patsubst @%,$(NOTABLE_PREFIX)%$(NOTABLE_SUFFIX),$(patsubst %:,\n - $(TARGET_PREFIX)%$(TARGET_SUFFIX):,$(subst `,\`,$(shell grep -Eh '^\w+:.*\#\#\#.*' $(MAKEFILE_LIST) | sed 's|:.*\#\#\#|:|g'))))"
endif

build_message_clean:
	@echo "$(SECTION_PREFIX) Clean"
	@echo "$(INFO_PREFIX) Cleaning up build artifacts from all modules"
	@echo "$(NOTE_PREFIX) To clean an individual module, use \`make clean_MODULE_NAME\`$(ACTION_PREFIX)"

clean: build_message_clean $(MODULE_CLEAN_TARGETS) clean_clang_tooling_artifacts ### @Default Cleans the repository of most build artifacts
	@echo "$(ACTION_SUFFIX)$(INFO_PREFIX) All clean!"

lib:
	-mkdir $@
