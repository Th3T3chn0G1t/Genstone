GEN_CORE_DIAGNOSTIC_FLAGS = -Werror -Weverything -Wno-gnu-statement-expression -Wno-c++98-compat -Wno-redundant-parens -Wno-atomic-implicit-seq-cst -Wno-padded -Wno-poison-system-directories -Wno-unknown-warning-option  -Wno-c++98-compat-pedantic -Wno-old-style-cast -Wno-register -Wno-overlength-strings -Wno-cast-qual
GEN_CORE_CFLAGS = -Igenstone/gencore/include $(SAFEC_CFLAGS) $(MIMALLOC_CFLAGS) $(GEN_CORE_DIAGNOSTIC_FLAGS)
ifeq ($(PLATFORM),DWN)
GEN_CORE_CFLAGS += -Igenstone/gencore/threademu/include
endif
GEN_CORE_LFLAGS = -lgencore $(SAFEC_LFLAGS) $(MIMALLOC_LFLAGS) -pthread
ifeq ($(PLATFORM),LNX)
GEN_CORE_LFLAGS += -ldl
endif

GEN_CORE_SOURCES = $(wildcard genstone/gencore/*.c)
ifeq ($(PLATFORM),DWN)
GEN_CORE_SOURCES += $(wildcard genstone/gencore/threademu/*.c)
endif
GEN_CORE_OBJECTS = $(GEN_CORE_SOURCES:.c=$(OBJECT_SUFFIX))

GEN_CORE_LIB = lib/$(LIB_PREFIX)gencore$(DYNAMIC_LIB_SUFFIX)

build_message_gencore:
	@$(ECHO) "$(SECTION_PREFIX) Gencore"
	@$(ECHO) "$(INFO_PREFIX) Core Genstone utilities"

gencore: build_message_gencore $(GEN_CORE_LIB) ### @Genstone Builds core Genstone utilities

_GEN_CORE_CFLAGS = $(GEN_CORE_DIAGNOSTIC_FLAGS)

ifeq ($(PLATFORM),LNX)
_GEN_CORE_LFLAGS = -ldl
endif
ifeq ($(PLATFORM),DWN)
_GEN_CORE_CFLAGS += -Igenstone/gencore/threademu/include
endif

$(GEN_CORE_LIB): CFLAGS = $(SAFEC_CFLAGS) $(MIMALLOC_CFLAGS) $(_GEN_CORE_CFLAGS)
$(GEN_CORE_LIB): LFLAGS = -Llib $(SAFEC_LFLAGS) $(MIMALLOC_LFLAGS) $(_GEN_CORE_LFLAGS)
$(GEN_CORE_LIB): $(GEN_CORE_OBJECTS) $(MIMALLOC_LIB) $(SAFEC_LIB) | lib

$(GEN_CORE_OBJECTS): $(wildcard genstone/gencore/include/*.h)
$(GEN_CORE_OBJECTS): $(SAFEC_LIB) $(MIMALLOC_LIB)

clean_gencore:
	@$(ECHO) "$(ACTION_PREFIX)"
	-rm $(GEN_CORE_OBJECTS)
	-rm $(GEN_CORE_LIB)
	@$(ECHO) "$(ACTION_SUFFIX)"
