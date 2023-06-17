ifeq ($(HOST),)
	ifeq ($(shell uname -s), Linux)
		HOST = linux
	endif
	ifeq ($(shell uname -s), Darwin)
		HOST = darwin
	endif
endif

ifeq ($(PLATFORM),)
	PLATFORM = $(HOST)
endif

ifeq ($(HOST_DIR),)
	HOST_DIR = $(GENSTONE_DIR)/build/host
endif

ifeq ($(PLATFORM_DIR),)
	PLATFORM_DIR = $(GENSTONE_DIR)/build/platform
endif

include $(HOST_DIR)/$(HOST).mk
include $(PLATFORM_DIR)/$(PLATFORM).mk
