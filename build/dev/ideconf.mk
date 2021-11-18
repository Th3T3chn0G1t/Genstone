# This Makefile is pretty much entirely a convenience provider
# So long as your generator block works and doesn't break another block pretty much everything goes
# Try to keep everything contained in the Makefile
# If platform specific tools make sense contextually (using Windows-specific tools to generate VS configs) then you can use them

IDECONF_INCLUDE_DIRS = genstone/gencore/include genstone/genuine/include genstone/gencalc/include genstone/genfx/gfx/include genstone/genfx/sfx/include 
ifeq ($(C11_COMPAT_UCHAR),1)
IDECONF_INCLUDE_DIRS += genstone/vendor/c11compat/musl/include
endif
ifeq ($(C11_COMPAT_THREADS),1)
IDECONF_INCLUDE_DIRS += genstone/vendor/c11compat/mesa/include
endif
ifeq ($(C11_COMPAT_ANNEXK),1)
IDECONF_INCLUDE_DIRS += genstone/vendor/c11compat/safeclib/include
endif
ifeq ($(PLATFORM),WIN)
IDECONF_INCLUDE_DIRS += genstone/vendor/dirent/include
endif
IDECONF_INCLUDE_DIRS += genstone/vendor/c11compat/mesa/include genstone/vendor/c11compat/musl/include genstone/vendor/mimalloc/include
IDECONF_DEFINES = DEBUG=1 RELEASE=0 ENABLED=1 DISABLED=0 PLATFORM=$(PLATFORM) WIN=1 DWN=2 LNX=3 BSD=4 MODE=DEBUG

IDECONF_TAG = This config was generated by Genstone

# If this isn't like this `make` doesn't allow a comma in the function call
IDECONF_VSC_ARRAY_ITEM_SUFFIX = \",
ideconf: ### @Default Generates an IDE configuration
	@echo "$(SECTION_PREFIX) IDE Configuration $(ACTION_PREFIX)"

ifeq ($(IDE),VSC)
	-mkdir .vscode
	touch .vscode/settings.json

	echo "// $(IDECONF_TAG)" > .vscode/settings.json

	echo "{" >> .vscode/settings.json

	echo "\"C_Cpp.default.includePath\": [" >> .vscode/settings.json
	echo "$(addprefix \",$(addsuffix $(IDECONF_VSC_ARRAY_ITEM_SUFFIX),$(IDECONF_INCLUDE_DIRS)))" >> .vscode/settings.json
	echo "]," >> .vscode/settings.json

	echo "\"C_Cpp.default.defines\": [" >> .vscode/settings.json
	echo "$(addprefix \",$(addsuffix $(IDECONF_VSC_ARRAY_ITEM_SUFFIX),$(IDECONF_DEFINES)))" >> .vscode/settings.json
	echo "]," >> .vscode/settings.json

	echo "}" >> .vscode/settings.json
endif

ifeq ($(IDE),)
	@echo "$(ERROR_PREFIX) \`IDE\` not set"
else
	@echo "$(ACTION_SUFFIX)"
endif
