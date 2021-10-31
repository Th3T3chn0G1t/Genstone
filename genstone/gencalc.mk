GEN_CALC_CFLAGS = -Igenstone/gencalc/include -fenable-matrix $(GEN_CORE_CFLAGS)
GEN_CALC_LFLAGS = $(GEN_CORE_LFLAGS)

.ONESHELL:
gencalc: ### @Genstone Builds Genstone mathematics utilities
.ONESHELL:
clean_gencalc:
