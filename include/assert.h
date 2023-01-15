#pragma once

#ifdef LIBVEC_ASSERT_ENABLE

#include <stdio.h>
#include <stdlib.h>

#define LIBVEC_ASSERT(x) libvec_assert((x), __FILE__, __LINE__, #x)

static inline void libvec_assert(int cond,
	const char *file, int line, const char *condstr)
{
	if (cond) return;

	fprintf(stderr, "libvec: Assertion failed at %s:%i (%s)\n",
		file, line, condstr);
	abort();
}

#else
#define LIBVEC_ASSERT(x)
#endif


#ifdef LIBVEC_HANDLE_ERRS

#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define LIBVEC_HANDLE_ERR(x) libvec_err(__FILE__, __LINE__, x)

static inline void libvec_err(const char *file, int line, const char *info)
{
	fprintf(stderr, "libvec: %s at %s:%i: %s\n",
		info, file, line, strerror(errno));
	exit(1);
}

#else
#define LIBVEC_HANDLE_ERR(x)
#endif
