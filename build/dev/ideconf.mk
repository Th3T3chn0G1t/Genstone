# This Makefile is pretty much entirely a convenience provider
# So long as your generator block works and doesn't break another block pretty much everything goes
# Try to keep everything contained in the Makefile
# If platform specific tools make sense contextually (using Windows-specific tools to generate VS configs) then you can use them

IDECONF_INCLUDE_DIRS = genesis/gencore/include genesis/genuine/include genesis/genfx/gfx/include genesis/genfx/sfx/include 
ifeq ($(C11_COMPAT_UCHAR),1)
IDECONF_INCLUDE_DIRS += genesis/vendor/c11compat/musl/include
endif
ifeq ($(C11_COMPAT_THREADS),1)
IDECONF_INCLUDE_DIRS += genesis/vendor/c11compat/mesa/include
endif
ifeq ($(C11_COMPAT_KANNEX),1)
IDECONF_INCLUDE_DIRS += genesis/vendor/c11compat/safeclib/include
endif
ifeq ($(PLATFORM),WIN)
IDECONF_INCLUDE_DIRS += genesis/vendor/dirent/include
endif
IDECONF_INCLUDE_DIRS += genesis/vendor/c11compat/mesa/include genesis/vendor/c11compat/musl/include
IDECONF_DEFINES = DEBUG=1 RELEASE=0 ENABLED=1 DISABLED=0 PLATFORM=DWN WIN=1 DWN=2 LNX=3 BSD=4 WEB=5 MODE=DEBUG

IDECONF_TAG = This config was generated by Genesis

# If this isn't like this `make` doesn't allow a comma in the function call
IDECONF_VSC_ARRAY_ITEM_SUFFIX = \",\n
ideconf:
	@echo "$(SECTION_PREFIX) IDE Configuration $(ACTION_PREFIX)"

ifeq ($(IDE),VSC)
	-mkdir .vscode
	touch .vscode/settings.json

	echo "// $(IDECONF_TAG)" > .vscode/settings.json

	echo "{" >> .vscode/settings.json

	echo "\t\"C_Cpp.default.includePath\": [" >> .vscode/settings.json
	echo "$(addprefix \t\t\",$(addsuffix $(IDECONF_VSC_ARRAY_ITEM_SUFFIX),$(IDECONF_INCLUDE_DIRS)))" >> .vscode/settings.json
	echo "\t]," >> .vscode/settings.json

	echo "\t\"C_Cpp.default.defines\": [" >> .vscode/settings.json
	echo "$(addprefix \t\t\",$(addsuffix $(IDECONF_VSC_ARRAY_ITEM_SUFFIX),$(IDECONF_DEFINES)))" >> .vscode/settings.json
	echo "\t]," >> .vscode/settings.json

	echo "}" >> .vscode/settings.json
endif

ifeq ($(IDE),)
	@echo "$(ERROR_PREFIX) \`IDE\` not set"
else
	@echo "$(ACTION_SUFFIX)"
endif
