GEN_CALC_CFLAGS = -Igenesis/gencalc/include -fenable-matrix $(GEN_CORE_CFLAGS)
GEN_CALC_LFLAGS = $(GEN_CORE_LFLAGS)

gencalc: ### @Genesis Builds Genesis mathematics utilities
clean_gencalc:
