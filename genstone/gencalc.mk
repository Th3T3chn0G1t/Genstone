GEN_CALC_CFLAGS = -Igenstone/gencalc/include -fenable-matrix $(GEN_CORE_CFLAGS) -mavx2
GEN_CALC_LFLAGS = $(GEN_CORE_LFLAGS)

gencalc: ### @Genstone Builds Genstone mathematics utilities
clean_gencalc:
