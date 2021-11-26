DOCS_DOXYFILE = build/docs/Doxyfile

build_message_docs:
	@echo "$(SECTION_PREFIX) Documentation $(ACTION_PREFIX)"

docs: build_message_docs clean_docs $(DOCS_DOXYFILE)
	-mkdir $@
	doxygen $(DOCS_DOXYFILE)

	@echo "$(ACTION_SUFFIX)"

# Builtin modules don't have auto clean targets
MODULE_CLEAN_TARGETS += clean_docs
clean_docs:
	-rm -rf $(wildcard docs/*)
