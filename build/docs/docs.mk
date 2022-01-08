DOCS_DOXYFILE = build/docs/Doxyfile

build_message_docs:
	@$(ECHO) "$(SECTION_PREFIX) Documentation $(ACTION_PREFIX)"

docs: build_message_docs clean_docs $(DOCS_DOXYFILE)
	-mkdir $@
	doxygen $(DOCS_DOXYFILE)

	@$(ECHO) "$(ACTION_SUFFIX)"

# Builtin modules don't have auto clean targets
MODULE_CLEAN_TARGETS += clean_docs
clean_docs:
	@$(ECHO) "$(ACTION_PREFIX)"
	-rm -rf $(wildcard docs/*)
	@$(ECHO) "$(ACTION_SUFFIX)"
