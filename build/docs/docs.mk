DOCS_DOXYFILE = build/docs/Doxyfile

docs: clean_docs $(DOCS_DOXYFILE)
	-mkdir $@
	doxygen $(DOCS_DOXYFILE)

# Builtin modules don't have auto clean targets
MODULE_CLEAN_TARGETS += clean_docs
clean_docs:
	-rm -rf $(wildcard docs/*)
