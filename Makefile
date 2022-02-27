include build/common.mk

VENDOR_MODULES = $(wildcard genstone/vendor/*.mk)
include $(VENDOR_MODULES)
BUILD_PREREQS += $(notdir $(subst .mk,,$(VENDOR_MODULES)))
MODULE_CLEAN_TARGETS += $(addprefix clean_,$(notdir $(subst .mk,,$(VENDOR_MODULES))))

GENSTONE_MODULES = $(wildcard genstone/*.mk)
include $(GENSTONE_MODULES)
BUILD_PREREQS += $(notdir $(subst .mk,,$(GENSTONE_MODULES)))
MODULE_CLEAN_TARGETS += $(addprefix clean_,$(notdir $(subst .mk,,$(GENSTONE_MODULES))))

# VULKAN_MODULES = $(wildcard vulkan/*.mk)
# include $(VULKAN_MODULES)
# BUILD_PREREQS += $(notdir $(subst .mk,,$(VULKAN_MODULES)))
# MODULE_CLEAN_TARGETS += $(addprefix clean_,$(notdir $(subst .mk,,$(VULKAN_MODULES))))

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
	@$(ECHO) "$(SECTION_PREFIX) Pre-Build"
ifneq ($(DISABLED_MODULES),)
	@$(ECHO) "$(INFO_PREFIX) Excluding disabled targets: $(addprefix \n   - ,$(DISABLED_MODULES))"
endif
ifneq ($(ERROR),)
	@$(ECHO) $(ERROR)
	@exit 1
endif

ifeq ($(DISABLED_MODULES),)
ifeq ($(ERROR),)
	@$(ECHO) "$(INFO_PREFIX) Nothing to do!"
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
		dependency_list \
	)
	
dependency_list: dependency_list.depfile ### @Default generates a list of source header dependencies and outputs it to a file

dependency_list.depfile:
	@$(ECHO) "$(ACTION_PREFIX)cat `find . -name "*.depfile"` > dependency_list.depfile$(ACTION_SUFFIX)"
	@cat `find . -name "*.depfile"` > dependency_list.depfile

all: $(BUILD_TARGETS) ### @Default Builds all modules specified in `config.mk`
	@$(ECHO) "$(INFO_PREFIX) All built!"

list: ### @Default Lists targets to be built for `all`
	@$(ECHO) "$(INFO_PREFIX) Targets to be built for \`all\`: $(addprefix \n - $(TARGET_PREFIX),$(addsuffix $(TARGET_SUFFIX),$(BUILD_TARGETS)))"
	@$(ECHO) "$(INFO_PREFIX) Excluded targets: $(addprefix \n - $(TARGET_PREFIX),$(addsuffix $(TARGET_SUFFIX),$(DISABLED_MODULES)))"
	@$(ECHO) "$(NOTE_PREFIX) For a list of available targets and their descriptions, see \`make help\`"

help: ### @Default Generates this message
	@$(ECHO) "$(INFO_PREFIX) Makefile help\n$(NOTE_PREFIX) This list only includes descriptions for user-desireable targets$(patsubst @%,$(NOTABLE_PREFIX)%$(NOTABLE_SUFFIX),$(patsubst %:,\n - $(TARGET_PREFIX)%$(TARGET_SUFFIX):,$(subst `,\`,$(shell grep -Eh '^\w+:.*\#\#\#.*' $(MAKEFILE_LIST) | sed 's|:.*\#\#\#|:|g'))))"

build_message_clean:
	@$(ECHO) "$(SECTION_PREFIX) Clean"
	@$(ECHO) "$(INFO_PREFIX) Cleaning up build artifacts from all modules"
	@$(ECHO) "$(NOTE_PREFIX) To clean an individual module, use \`make clean_MODULE_NAME\`$(ACTION_PREFIX)"

clean: build_message_clean $(MODULE_CLEAN_TARGETS) clean_clang_tooling_artifacts ### @Default Cleans the repository of most build artifacts
	@$(ECHO) "$(ACTION_SUFFIX)$(INFO_PREFIX) All clean!"

lib:
	-mkdir $@
