CFLAGS = -g -I include -Wno-prototype -Wunused-function -Wunused-variable

SRC = src/vec.c src/bitvec.c
HDR = include/vec.h include/bitvec.h

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

build/libvec.a: $(SRC) $(HDR) | build
	$(CC) -o build/tmp.o $(SRC) $(CFLAGS) -r
	objcopy --keep-global-symbols=libvec.abi build/tmp.o build/fixed.o
	ar rcs $@ build/fixed.o

build/libvec.so: $(SRC) $(HDR) | build
	$(CC) -o $@ $(SRC) -fPIC $(CFLAGS) -shared -Wl,-version-script libvec.lds

build/test: src/test.c build/libvec.a
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: all clean
