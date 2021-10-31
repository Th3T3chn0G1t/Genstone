DOCS_DOXYFILE = build/docs/Doxyfile

.ONESHELL:
build_message_docs:
	@echo "$(SECTION_PREFIX) Documentation $(ACTION_PREFIX)"

.ONESHELL:
docs: build_message_docs clean_docs $(DOCS_DOXYFILE)
	-mkdir $@
	doxygen $(DOCS_DOXYFILE)

	@echo "$(ACTION_SUFFIX)"

# Builtin modules don't have auto clean targets
MODULE_CLEAN_TARGETS += clean_docs
.ONESHELL:
clean_docs:
	-$(RMDIR) $(subst /,$(SEP),$(wildcard docs/*))
