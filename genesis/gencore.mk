GEN_CORE_DIAGNOSTIC_FLAGS = -Werror -Weverything -Wno-redundant-parens -Wno-atomic-implicit-seq-cst -Wno-padded -Wno-vla -Wno-poison-system-directories
GEN_CORE_CFLAGS = -Igenesis/gencore/include $(C11_COMPAT_CFLAGS) $(GEN_CORE_DIAGNOSTIC_FLAGS)
ifeq ($(PLATFORM),WIN)
GEN_CORE_CFLAGS += -Igenesis/vendor/dirent/include
endif
GEN_CORE_LFLAGS = -lgencore $(C11_COMPAT_LFLAGS) -ldl

GEN_CORE_SOURCES = $(wildcard genesis/gencore/*.c)
GEN_CORE_OBJECTS = $(GEN_CORE_SOURCES:.c=$(OBJECT_SUFFIX))

GEN_CORE_LIB = lib/$(LIB_PREFIX)gencore$(DYNAMIC_LIB_SUFFIX)

build_message_gencore:
	@echo "$(SECTION_PREFIX) Gencore"
	@echo "$(INFO_PREFIX) Core Genesis utilities"

gencore: build_message_gencore $(GEN_CORE_LIB)

_GEN_CORE_CFLAGS = $(GEN_CORE_DIAGNOSTIC_FLAGS)
ifeq ($(PLATFORM),WIN)
_GEN_CORE_CFLAGS += -Igenesis/vendor/dirent/include # This is kind of annoying but whatever
endif

ifeq ($(PLATFORM),LNX)
_GEN_CORE_LFLAGS = -ldl
endif
ifeq ($(PLATFORM),BSD)
_GEN_CORE_LFLAGS = -ldl
endif

$(GEN_CORE_LIB): CFLAGS = $(C11_COMPAT_CFLAGS) $(_GEN_CORE_CFLAGS)
$(GEN_CORE_LIB): LFLAGS = -Llib $(C11_COMPAT_LFLAGS) $(_GEN_CORE_LFLAGS)
$(GEN_CORE_LIB): $(GEN_CORE_OBJECTS) $(C11_COMPAT_LIB) | lib

$(GEN_CORE_OBJECTS): $(wildcard genesis/gencore/include/*.h)

clean_gencore:
	-$(RM) $(GEN_CORE_OBJECTS)
	-$(RM) $(GEN_CORE_LIB)
