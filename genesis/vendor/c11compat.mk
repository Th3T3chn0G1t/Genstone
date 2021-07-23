C11COMPAT = 0 # Windows & macOS always need, Linux never needs
C11COMPAT_UCHAR = 0 # BSD never needs
C11COMPAT_THREADS = 0 # FreeBSD never needs but other flavours do

# 		UCHAR		THREADS
# LNX	0			0
# WIN	1			1
# DWN	1			1
# BSD	0			FreeBSD 0, Other 1

ifeq ($(PLATFORM),LNX)
	C11COMPAT = 0
	C11COMPAT_UCHAR = 0
	C11COMPAT_THREADS = 0
endif
ifeq ($(PLATFORM),WIN)
	C11COMPAT = 1
	C11COMPAT_UCHAR = 1
	C11COMPAT_THREADS = 1
endif
ifeq ($(PLATFORM),DWN)
	C11COMPAT = 1
	C11COMPAT_UCHAR = 1
	C11COMPAT_THREADS = 1
endif
ifeq ($(PLATFORM),BSD)
	C11COMPAT_UCHAR = 0
	ifneq ($(shell uname -r),FreeBSD)
		C11COMPAT = 0
		C11COMPAT_THREADS = 0
	else
		C11COMPAT = 1
		C11COMPAT_THREADS = 1
	endif
endif

ifeq ($(C11COMPAT),1)
C11COMPAT_CFLAGS =
C11COMPAT_LFLAGS =
ifeq ($(C11COMPAT_UCHAR),1)
C11COMPAT_CFLAGS += -Igenesis/vendor/c11compat/musl/include
C11COMPAT_LFLAGS += -lc11compat

C11COMPAT_SOURCES = $(wildcard genesis/vendor/c11compat/musl/*.c)
C11COMPAT_OBJECTS = $(C11COMPAT_SOURCES:.c=.o)

C11COMPAT_LIB = genesis/vendor/lib/$(LIB_PREFIX)c11compat$(DYNAMIC_LIB_SUFFIX)

c11compat: $(C11COMPAT_LIB)

$(C11COMPAT_LIB): CFLAGS = -Igenesis/vendor/c11compat/musl/include
$(C11COMPAT_LIB): LFLAGS =
$(C11COMPAT_LIB): $(C11COMPAT_OBJECTS) genesis/vendor/lib

clean_c11compat:
	-rm $(C11COMPAT_OBJECTS)
	-rm $(C11COMPAT_LIB)
endif
ifeq ($(C11COMPAT_THREADS),1)
C11COMPAT_CFLAGS += -Igenesis/vendor/c11compat/mesa/include
endif
endif

ifneq ($(C11COMPAT_UCHAR),1)
c11compat:
clean_c11compat:
endif
