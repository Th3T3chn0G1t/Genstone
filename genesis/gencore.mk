GEN_CORE_CFLAGS = -Igenesis/gencore/include $(C11_COMPAT_CFLAGS)
ifeq ($(PLATFORM),WIN)
GEN_CORE_CFLAGS += -Igenesis/vendor/dirent/include
endif
GEN_CORE_LFLAGS += -lgencore $(C11_COMPAT_LFLAGS)

GEN_CORE_SOURCES = $(wildcard genesis/gencore/*.c)
GEN_CORE_OBJECTS = $(GEN_CORE_SOURCES:.c=.o)

GEN_CORE_LIB = lib/$(LIB_PREFIX)gencore$(DYNAMIC_LIB_SUFFIX)

gencore: $(GEN_CORE_LIB)

$(GEN_CORE_LIB): CFLAGS = $(C11_COMPAT_CFLAGS)
$(GEN_CORE_LIB): LFLAGS = -Llib $(C11_COMPAT_LFLAGS)
$(GEN_CORE_LIB): $(GEN_CORE_OBJECTS) $(C11_COMPAT_LIB) | lib

clean_gencore:
	-rm $(GEN_CORE_OBJECTS)
	-rm $(GEN_CORE_LIB)
