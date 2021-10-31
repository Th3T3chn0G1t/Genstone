DEVENV_OUT = out

_DOCS_CONTENT = $(wildcard docs/*)

.ONESHELL:
devenv: all docs $(IDECONF_INCLUDE_DIRS) | $(DEVENV_OUT) $(DEVENV_OUT)/include $(DEVENV_OUT)/lib $(DEVENV_OUT)/docs ### @Default Generates a development environment for the current configuration
	$(CP) $(subst /,$(SEP),$(wildcard $(addsuffix /*,$(IDECONF_INCLUDE_DIRS))) $(DEVENV_OUT)/include)
	$(CP) $(subst /,$(SEP),$(filter-out $(wildcard lib/*test*),$(wildcard lib/*)) $(DEVENV_OUT)/lib)
	$(CP) $(subst /,$(SEP),$(_DOCS_CONTENT) $(DEVENV_OUT)/docs)

$(DEVENV_OUT)/%: $(DEVENV_OUT)
	-mkdir $@

$(DEVENV_OUT):
	-mkdir $@
