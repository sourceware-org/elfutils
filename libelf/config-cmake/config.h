#ifndef _CONFIG_H_
#define _GNU_SOURCE
#include "eu-config.h"

#if defined(_WIN32)
#define HAVE_DECL_MEMPCPY 0
#define HAVE_DECL_MEMRCHR 0
#define HAVE_DECL_POWEROF2 0
#define HAVE_DECL_MMAP 0
#else
#define HAVE_DECL_MEMPCPY 1
#define HAVE_DECL_MEMRCHR 1
#define HAVE_DECL_POWEROF2 1
#define HAVE_DECL_MMAP 1
#endif
#define HAVE_DECL_RAWMEMCHR 0
#define HAVE_DECL_REALLOCARRAY 1
#define HAVE_VISIBILITY 1

#undef HAVE_GCC_STRUCT
#undef USE_LOCKS

#if !defined(_MSC_VER)
#define HAVE_ERROR_H
#endif

#endif /* _CONFIG_H_ */