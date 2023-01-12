CFLAGS = -g -I include -Wno-prototype -Wunused-function -Wunused-variable

ifeq "$(LIBVEC_ASSERT)" "1"
CFLAGS += -DLIBVEC_ASSERT=1
endif

all: build/libvec.so build/libvec.a build/test

clean:
	rm -rf build

build:
	mkdir build

build/libvec.a: src/vec.c | build
	$(CC) -o build/tmp.o $^ $(CFLAGS) -r
	objcopy --keep-global-symbols=libvec.abi build/tmp.o build/fixed.o
	ar rcs $@ build/fixed.o

build/libvec.so: src/vec.c | build
	$(CC) -o $@ $* -fPIC $(CFLAGS) -shared -Wl,-version-script libvec.lds

build/test: src/test.c build/libvec.a
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: all clean
