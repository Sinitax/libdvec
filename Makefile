PREFIX ?= /usr/local
LIBDIR ?= /lib
INCLDIR ?= /include

CFLAGS = -I include -Wno-prototype -Wunused-function -Wunused-variable

ifeq "$(DEBUG)" "1"
CFLAGS += -g -DLIBVEC_CHECK_ENABLE=1
endif

all: build/libvec.so build/libvec.a build/test

clean:
	rm -rf build

build:
	mkdir build

build/libvec.a: src/vec.c include/vec.h libvec.abi | build
	$(CC) -o build/tmp.o src/vec.c $(CFLAGS) -r
	objcopy --keep-global-symbols=libvec.abi build/tmp.o build/fixed.o
	ar rcs $@ build/fixed.o

build/libvec.so: src/vec.c include/vec.h libvec.lds | build
	$(CC) -o $@ src/vec.c -fPIC $(CFLAGS) \
		-shared -Wl,-version-script libvec.lds

build/test: src/test.c build/libvec.a
	$(CC) -o $@ $^ $(CFLAGS)

install:
	install -m644 include/vec.h -t "$(DESTDIR)$(PREFIX)$(INCLDIR)"
	install -m755 build/libvec.so -t "$(DESTDIR)$(PREFIX)$(LIBDIR)"
	install -m755 build/libvec.a -t "$(DESTDIR)$(PREFIX)$(LIBDIR)"

uninstall:
	rm -f "$(DESTDIR)$(PREFIX)$(INCLDIR)/vec.h"
	rm -f "$(DESTDIR)$(PREFIX)$(LIBDIR)/libvec.so"
	rm -f "$(DESTDIR)$(PREFIX)$(LIBDIR)/libvec.a"

.PHONY: all clean install uninstall
