PREFIX ?= /usr/local
LIBDIR ?= /lib
INCLUDEDIR ?= /include

CFLAGS = -I include -Wno-prototype -Wunused-function -Wunused-variable

ifeq "$(LIBVEC_DEBUG)" "1"
CFLAGS += -g
endif

ifeq "$(LIBVEC_ASSERT)" "1"
CFLAGS += -DLIBVEC_ASSERT_ENABLE=1
endif

ifeq "$(LIBVEC_HANDLE_ERR)" "1"
CFLAGS += -DLIBVEC_HANDLE_ERRS=1
endif

all: build/libvec.so build/libvec.a build/test

clean:
	rm -rf build

build:
	mkdir build

build/libvec.a: src/vec.c include/vec.h | build
	$(CC) -o build/tmp.o src/vec.c $(CFLAGS) -r
	objcopy --keep-global-symbols=libvec.abi build/tmp.o build/fixed.o
	ar rcs $@ build/fixed.o

build/libvec.so: src/vec.c include/vec.h | build
	$(CC) -o $@ src/vec.c -fPIC $(CFLAGS) -shared -Wl,-version-script libvec.lds

build/test: src/test.c build/libvec.a
	$(CC) -o $@ $^ $(CFLAGS)

install:
	install -m644 include/vec.h -t "$(DESTDIR)$(PREFIX)$(INCLUDEDIR)"
	install -m755 build/libvec.so -t "$(DESTDIR)$(PREFIX)$(LIBDIR)"
	install -m755 build/libvec.a -t "$(DESTDIR)$(PREFIX)$(LIBDIR)"

uninstall:
	rm -f "$(DESTDIR)$(PREFIX)$(INCLUDEDIR)/vec.h"
	rm -f "$(DESTDIR)$(PREFIX)$(LIBDIR)/libvec.so"
	rm -f "$(DESTDIR)$(PREFIX)$(LIBDIR)/libvec.a"

.PHONY: all clean install uninstall
