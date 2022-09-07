GEN_CORE_COMMON_CFLAGS = -fenable-matrix
GEN_CORE_COMMON_LFLAGS =

GEN_CORE_DIAGNOSTIC_CFLAGS = -Werror -Weverything
GEN_CORE_DIAGNOSTIC_CFLAGS += -Wno-poison-system-directories -Wno-declaration-after-statement
GEN_CORE_DIAGNOSTIC_CFLAGS += -Wno-padded -Wno-c++98-compat -Wno-pointer-arith -Wno-cast-align
GEN_CORE_DIAGNOSTIC_CFLAGS += -Wno-missing-field-initializers # TODO: This might be desirable for genfx/vk

GEN_CORE_INTERNAL_CFLAGS = $(GEN_CORE_DIAGNOSTIC_CFLAGS) $(GEN_CORE_COMMON_CFLAGS)
GEN_CORE_INTERNAL_LFLAGS = $(GEN_CORE_COMMON_LFLAGS)

ifeq ($(SANITIZERS),ENABLED)
	GEN_CORE_INTERNAL_CFLAGS = -fsanitize=undefined,address,cfi
	GEN_CORE_INTERNAL_LFLAGS = -fsanitize=undefined,address,cfi
endif

ifneq ($(PLATFORM),WINDOWS)
	GEN_CORE_COMMON_LFLAGS += -lpthread
endif

ifeq ($(PLATFORM),LINUX)
	GEN_CORE_COMMON_LFLAGS += -ldl
endif

GEN_CORE_CFLAGS = -I$(GENSTONE_DIR)/genstone/gencore/include $(GEN_CORE_COMMON_CFLAGS)
GEN_CORE_LFLAGS = -lgencore $(GEN_CORE_COMMON_LFLAGS)
GEN_CORE_LIBDIRS = $(GENSTONE_DIR)/lib

GEN_CORE_SOURCES = $(wildcard $(GENSTONE_DIR)/genstone/gencore/*.c)
GEN_CORE_OBJECTS = $(GEN_CORE_SOURCES:.c=$(OBJECT_SUFFIX))

GEN_CORE_LIB = $(GENSTONE_DIR)/lib/$(LIB_PREFIX)gencore$(DYNAMIC_LIB_SUFFIX)

$(GEN_CORE_LIB): CFLAGS = $(GEN_CORE_INTERNAL_CFLAGS)
$(GEN_CORE_LIB): LFLAGS = $(GEN_CORE_INTERNAL_LFLAGS)
$(GEN_CORE_LIB): LIBDIRS =
$(GEN_CORE_LIB): $(GEN_CORE_OBJECTS) | $(GENSTONE_DIR)/lib

.PHONY: gencore
gencore: $(GEN_CORE_LIB)

.PHONY: gencore
test_gencore: gencore # TODO: Write tests

.PHONY: clean_gencore
clean_gencore:
	@$(ECHO) "$(ACTION_PREFIX)"
	-$(RM) $(GEN_CORE_OBJECTS)
	-$(RM) $(GEN_CORE_LIB)
	@$(ECHO) "$(ACTION_SUFFIX)"
