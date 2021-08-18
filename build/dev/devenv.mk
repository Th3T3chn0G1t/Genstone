DEVENV_OUT = out

_DOCS_CONTENT = $(wildcard docs/*)

devenv: all docs $(IDECONF_INCLUDE_DIRS) | $(DEVENV_OUT) $(DEVENV_OUT)/include $(DEVENV_OUT)/lib $(DEVENV_OUT)/docs
	$(CP) $(subst /,$(SEP),$(wildcard $(addsuffix /*,$(IDECONF_INCLUDE_DIRS))) $(DEVENV_OUT)/include)
	$(CP) $(subst /,$(SEP),$(filter-out $(wildcard lib/*test*),$(wildcard lib/*)) $(DEVENV_OUT)/lib)
	$(CP) $(subst /,$(SEP),$(_DOCS_CONTENT) $(DEVENV_OUT)/docs)

$(DEVENV_OUT)/%: $(DEVENV_OUT)
	-mkdir $@

$(DEVENV_OUT):
	-mkdir $@

# Builtin modules don't have auto clean targets
MODULE_CLEAN_TARGETS += clean_devenv
clean_devenv:
	-$(RMDIR) $(subst /,$(SEP),$(wildcard $(DEVENV_OUT)/*))
