GEN_CORE_CFLAGS += -Igenesis/gencore/include $(C11COMPAT_CFLAGS)
GEN_CORE_LFLAGS += -lgencore $(C11COMPAT_LFLAGS)

GEN_CORE_SOURCES = $(wildcard genesis/gencore/*.c)
GEN_CORE_OBJECTS = $(GEN_CORE_SOURCES:.c=.o)

GEN_CORE_LIB = lib/$(LIB_PREFIX)gencore$(DYNAMIC_LIB_SUFFIX)

gencore: $(GEN_CORE_LIB)

$(GEN_CORE_LIB): CFLAGS =
$(GEN_CORE_LIB): LFLAGS =
$(GEN_CORE_LIB): $(GEN_CORE_OBJECTS) c11compat lib

clean_gencore:
	-rm $(GEN_CORE_OBJECTS)
	-rm $(GEN_CORE_LIB)
