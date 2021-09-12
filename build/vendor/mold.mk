mold:
	LTO=1 make -Cbuild/vendor/mold

# Cleaning build tools automatically with `clean` doesn't really make sense
#MODULE_CLEAN_TARGETS += clean_mold
clean_mold:
	make -Cbuild/vendor/mold clean
