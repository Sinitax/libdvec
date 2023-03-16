PREFIX ?= /usr/local
LIBDIR ?= /lib
INCLDIR ?= /include

CFLAGS = -I include -Wno-prototype -Wunused-function -Wunused-variable

ifeq "$(DEBUG)" "1"
CFLAGS += -g -DLIBDVEC_CHECK_ENABLE=1
endif

all: build/libdvec.so build/libdvec.a build/test

clean:
	rm -rf build

build:
	mkdir build

build/libdvec.a: src/dvec.c include/dvec.h libdvec.api | build
	$(CC) -o build/tmp.o src/dvec.c $(CFLAGS) -r
	objcopy --keep-global-symbols=libdvec.api build/tmp.o build/fixed.o
	ar rcs $@ build/fixed.o

build/libdvec.so: src/dvec.c include/dvec.h libdvec.lds | build
	$(CC) -o $@ src/dvec.c -fPIC $(CFLAGS) \
		-shared -Wl,-version-script libdvec.lds

build/test: src/test.c build/libdvec.a
	$(CC) -o $@ $^ $(CFLAGS)

install:
	install -m644 include/dvec.h -t "$(DESTDIR)$(PREFIX)$(INCLDIR)"
	install -m755 build/libdvec.so -t "$(DESTDIR)$(PREFIX)$(LIBDIR)"
	install -m755 build/libdvec.a -t "$(DESTDIR)$(PREFIX)$(LIBDIR)"

uninstall:
	rm -f "$(DESTDIR)$(PREFIX)$(INCLDIR)/dvec.h"
	rm -f "$(DESTDIR)$(PREFIX)$(LIBDIR)/libdvec.so"
	rm -f "$(DESTDIR)$(PREFIX)$(LIBDIR)/libdvec.a"

.PHONY: all clean install uninstall
