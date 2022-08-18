GEN_CORE_COMMON_CFLAGS = -Werror -Weverything
GEN_CORE_COMMON_CFLAGS += -Wno-poison-system-directories
GEN_CORE_COMMON_LFLAGS =

ifneq ($(PLATFORM),WINDOWS)
GEN_CORE_COMMON_LFLAGS += -lpthread
endif

ifeq ($(PLATFORM),LINUX)
GEN_CORE_COMMON_LFLAGS += -ldl
endif

GEN_CORE_CFLAGS = -Igenstone/gencore/include $(GEN_CORE_COMMON_CFLAGS)
GEN_CORE_LFLAGS = -lgencore $(GEN_CORE_COMMON_LFLAGS)

GEN_CORE_SOURCES = $(wildcard genstone/gencore/*.c)
GEN_CORE_OBJECTS = $(GEN_CORE_SOURCES:.c=$(OBJECT_SUFFIX))

GEN_CORE_LIB = lib/$(LIB_PREFIX)gencore$(DYNAMIC_LIB_SUFFIX)

$(GEN_CORE_LIB): CFLAGS = $(GEN_CORE_COMMON_CFLAGS)
$(GEN_CORE_LIB): LFLAGS = $(GEN_CORE_COMMON_LFLAGS)
$(GEN_CORE_LIB): $(GEN_CORE_OBJECTS) | lib

.PHONY: gencore
gencore: $(GEN_CORE_LIB)

.PHONY: gencore
test_gencore: $(GEN_CORE_LIB)

.PHONY: clean_gencore
clean_gencore:
	@$(ECHO) "$(ACTION_PREFIX)"
	-$(RM) $(GEN_CORE_OBJECTS)
	-$(RM) $(GEN_CORE_LIB)
	@$(ECHO) "$(ACTION_SUFFIX)"
