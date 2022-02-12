CFLAGS = -I include
LDLIBS =
DEPFLAGS = -MT $@ -MMD -MP -MF build/$*.d

_SRCS = vec.c
SRCS = $(_SRCS:%.c=src/%.c)
OBJS = $(_SRCS:%.c=build/%.o)
DEPS = $(_SRCS:%.c=build/%.d)
PI_OBJS = $(_SRCS:%.c=build/%.pi.o)

.PHONY: all clean

all: build/libvec.so build/libvec.a build/test

clean:
	rm -rf build

build:
	mkdir build

build/%.o: src/%.c build/%.d | build
	$(CC) -c -o $@ $< $(DEPFLAGS) $(CFLAGS)

build/%.pi.o: src/%.c build/%.d | build
	$(CC) -c -o $@ $< $(DEPFLAGS) $(CFLAGS) -fPIC

build/%.d: | build;

include $(DEPS)

build/libvec.a: $(OBJS) | build
	$(CC) -o build/tmp.o $^ $(CFLAGS) -r
	objcopy --keep-global-symbols=libvec.abi build/tmp.o build/fixed.o
	ar rcs $@ build/fixed.o

build/libvec.so: $(PI_OBJS) | build
	$(CC) -o build/tmp.so $(PI_OBJS) $(CFLAGS) -shared
	objcopy --keep-global-symbols=libvec.abi build/tmp.so $@

build/test: src/test.c build/libvec.a | build
	$(CC) -o $@ $^ -I include

