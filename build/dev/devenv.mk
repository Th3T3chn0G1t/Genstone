DEVENV_OUT = out

_DOCS_CONTENT = $(wildcard docs/*)

devenv: all docs $(IDECONF_INCLUDE_DIRS) | $(DEVENV_OUT) $(DEVENV_OUT)/include $(DEVENV_OUT)/lib $(DEVENV_OUT)/docs ### @Default Generates a development environment for the current configuration
	cp -r $(wildcard $(addsuffix /*,$(IDECONF_INCLUDE_DIRS))) $(DEVENV_OUT)/include
	cp -r $(filter-out $(wildcard lib/*test*),$(wildcard lib/*)) $(DEVENV_OUT)/lib
	cp -r $(_DOCS_CONTENT) $(DEVENV_OUT)/docs

$(DEVENV_OUT)/%: $(DEVENV_OUT)
	-mkdir $@

$(DEVENV_OUT):
	-mkdir $@
