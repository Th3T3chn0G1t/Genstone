GEN_CORE_DIAGNOSTIC_FLAGS = -Werror -Weverything -Wno-c++98-compat -Wno-redundant-parens -Wno-atomic-implicit-seq-cst -Wno-padded -Wno-vla -Wno-poison-system-directories
GEN_CORE_CFLAGS = -Igenstone/gencore/include $(C11_COMPAT_CFLAGS) $(MIMALLOC_CFLAGS) $(GEN_CORE_DIAGNOSTIC_FLAGS)
ifeq ($(PLATFORM),WIN)
GEN_CORE_CFLAGS += -Igenstone/vendor/dirent/include
endif
GEN_CORE_LFLAGS = -lgencore $(C11_COMPAT_LFLAGS) $(MIMALLOC_LFLAGS)
ifeq ($(PLATFORM),LNX)
GEN_CORE_LFLAGS += -ldl
endif
ifeq ($(PLATFORM),BSD)
GEN_CORE_LFLAGS += -ldl
endif

GEN_CORE_SOURCES = $(wildcard genstone/gencore/*.c)
GEN_CORE_OBJECTS = $(GEN_CORE_SOURCES:.c=$(OBJECT_SUFFIX))

GEN_CORE_LIB = lib/$(LIB_PREFIX)gencore$(DYNAMIC_LIB_SUFFIX)

.ONESHELL:
build_message_gencore:
	@echo "$(SECTION_PREFIX) Gencore"
	@echo "$(INFO_PREFIX) Core Genstone utilities"

.ONESHELL:
gencore: c11compat mimalloc build_message_gencore $(GEN_CORE_LIB) ### @Genstone Builds core Genstone utilities

_GEN_CORE_CFLAGS = $(GEN_CORE_DIAGNOSTIC_FLAGS)
ifeq ($(PLATFORM),WIN)
_GEN_CORE_CFLAGS += -Igenstone/vendor/dirent/include # This is kind of annoying but whatever
endif

ifeq ($(PLATFORM),LNX)
_GEN_CORE_LFLAGS = -ldl
endif
ifeq ($(PLATFORM),BSD)
_GEN_CORE_LFLAGS = -ldl
endif

$(GEN_CORE_LIB): CFLAGS = $(C11_COMPAT_CFLAGS) $(MIMALLOC_CFLAGS) $(_GEN_CORE_CFLAGS)
$(GEN_CORE_LIB): LFLAGS = -Llib $(C11_COMPAT_LFLAGS) $(MIMALLOC_LFLAGS) $(_GEN_CORE_LFLAGS)
$(GEN_CORE_LIB): $(GEN_CORE_OBJECTS) | lib

$(GEN_CORE_OBJECTS): $(wildcard genstone/gencore/include/*.h)

.ONESHELL:
clean_gencore:
	-$(RM) $(GEN_CORE_OBJECTS)
	-$(RM) $(GEN_CORE_LIB)
