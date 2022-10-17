/* Definitions for system functions.
   Copyright (C) 2006-2011 Red Hat, Inc.
   Copyright (C) 2022 Mark J. Wielaard <mark@klomp.org>
   Copyright (C) 2022 Yonggang Luo <luoyonggang@gmail.com>
   This file is part of elfutils.

   This file is free software; you can redistribute it and/or modify
   it under the terms of either

     * the GNU Lesser General Public License as published by the Free
       Software Foundation; either version 3 of the License, or (at
       your option) any later version

   or

     * the GNU General Public License as published by the Free
       Software Foundation; either version 2 of the License, or (at
       your option) any later version

   or both in parallel, as here.

   elfutils is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received copies of the GNU General Public License and
   the GNU Lesser General Public License along with this program.  If
   not, see <http://www.gnu.org/licenses/>.  */

/* mingw */
#if defined(_WIN32) && !defined(_MSC_VER)
#define _SEARCH_PRIVATE
#endif

#include <errno.h>
#include <search.h>
#include <stdbool.h>

#include "system.h"
#if defined(_WIN32)
#include <windows.h>
#endif

#if defined(_MSC_VER)
static const char letters[] =
"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
int mkstemp(char *tmpl)
{
  int len;
  char *XXXXXX;
  static unsigned long long value;
  unsigned long long random_time_bits;
  unsigned int count;
  int fd = -1;
  int save_errno = errno;

  /* A lower bound on the number of temporary files to attempt to
     generate.  The maximum total number of temporary file names that
     can exist for a given template is 62**6.  It should never be
     necessary to try all these combinations.  Instead if a reasonable
     number of names is tried (we define reasonable as 62**3) fail to
     give the system administrator the chance to remove the problems.  */
#define ATTEMPTS_MIN (62 * 62 * 62)

  /* The number of times to attempt to generate a temporary file.  To
     conform to POSIX, this must be no smaller than TMP_MAX.  */
#if ATTEMPTS_MIN < TMP_MAX
  unsigned int attempts = TMP_MAX;
#else
  unsigned int attempts = ATTEMPTS_MIN;
#endif

  len = strlen (tmpl);
  if (len < 6 || strcmp (&tmpl[len - 6], "XXXXXX"))
    {
      errno = EINVAL;
      return -1;
    }

/* This is where the Xs start.  */
  XXXXXX = &tmpl[len - 6];

  /* Get some more or less random data.  */
  {
    SYSTEMTIME      stNow;
    FILETIME ftNow;

    // get system time
    GetSystemTime(&stNow);
    stNow.wMilliseconds = 500;
    if (!SystemTimeToFileTime(&stNow, &ftNow))
    {
        errno = -1;
        return -1;
    }

    random_time_bits = (((unsigned long long)ftNow.dwHighDateTime << 32)
                        | (unsigned long long)ftNow.dwLowDateTime);
  }
  value += random_time_bits ^ (unsigned long long)GetCurrentThreadId ();

  for (count = 0; count < attempts; value += 7777, ++count)
    {
      unsigned long long v = value;

      /* Fill in the random bits.  */
      XXXXXX[0] = letters[v % 62];
      v /= 62;
      XXXXXX[1] = letters[v % 62];
      v /= 62;
      XXXXXX[2] = letters[v % 62];
      v /= 62;
      XXXXXX[3] = letters[v % 62];
      v /= 62;
      XXXXXX[4] = letters[v % 62];
      v /= 62;
      XXXXXX[5] = letters[v % 62];

      fd = open (tmpl, O_RDWR | O_CREAT | O_EXCL, S_IREAD | S_IWRITE);
      if (fd >= 0)
    {
      errno = save_errno;
      return fd;
    }
      else if (errno != EEXIST)
    return -1;
    }

  /* We got out of the loop because we ran out of combinations to try.  */
  errno = EEXIST;
  return -1;
}

static inline bool is_path_separator(char c) {
  return c == '/' || c == '\\';
}

char *basename(char *s)
{
  size_t i;
  if (!s || !*s)
    return ".";
  i = strlen(s) - 1;
  for (; i && is_path_separator(s[i]); i--)
    s[i] = 0;
  for (; i && !is_path_separator(s[i - 1]); i--)
    ;
  return s + i;
}

int ftruncate(int fd, off_t length)
{
     return _chsize_s(fd, length);
}

int vasprintf(char **strp, const char *format, va_list ap)
{
    int len = _vscprintf(format, ap);
    if (len == -1)
        return -1;
    char *str = (char*)malloc((size_t) len + 1);
    if (!str)
        return -1;
    int retval = vsnprintf(str, len + 1, format, ap);
    if (retval == -1) {
        free(str);
        return -1;
    }
    *strp = str;
    return retval;
}

struct tree {
    // datum must be the first field in struct tree
    const void *datum;
    struct tree *left, *right;
};

static struct tree *null_tree_p = NULL;

typedef int  cmp(const void *key1, const void *key2);
typedef void act(const void *node, VISIT order, int level);
typedef void freer(void *node);

// guaranteed to return a non-NULL pointer (might be a pointer to NULL)
static struct tree **traverse(const void *key, struct tree **rootp, cmp *compar,
                              int create, struct tree **parent)
{
    if (*rootp == NULL) {
        if (create) {
            struct tree *q = *rootp = malloc(sizeof *q);
            q->left = q->right = NULL;
            q->datum = key;
            return rootp;
        } else {
            return &null_tree_p;
        }
    }

    struct tree *p = *rootp;
    int result = compar(key, p->datum);
    if (parent) *parent = p;
    // we could easily implement this iteratively as well but let's do it
    // recursively and depend on a smart compiler to use tail recursion
    if (result == 0) {
        return rootp;
    } else if (result < 0) {
        return traverse(key, &p->left , compar, create, parent);
    } else {
        return traverse(key, &p->right, compar, create, parent);
    }
}

void *tfind(const void *key, void *const *rootp, cmp *compar)
{
    return *traverse(key, (void*)rootp, compar, 0, NULL);
}

void *tsearch(const void *key, void **rootp, cmp *compar)
{
    return *traverse(key, (void*)rootp, compar, 1, NULL);
}

void *tdelete(const void *restrict key, void **restrict rootp, cmp *compar)
{
    struct tree *parent = NULL;
    struct tree **q = traverse(key, (void*)rootp, compar, 0, &parent);
    if (!*q || !(*q)->datum)
        return NULL;

    struct tree *t = *q;
    do {
        if (!t->right) {
            *q = t->left;
            break;
        }

        struct tree *r = t->right;
        if (!r->left) {
            r->left = t->left;
            *q = r;
            break;
        }

        struct tree *s = r->left;
        while (s->left) {
            r = s;
            s = r->left;
        }
        s->left = t->left;
        r->left = s->right;
        s->right = t->right;
        *q = s;
    } while (0);

    free(t);

    return parent;
}

// walk(), unlike traverse(), cannot tail-recurse, and so we might want an
// iterative implementation for large trees
static void walk(const void *root, act *action, int level)
{
    const struct tree *p = root;
    if (!p) return;

    if (!p->left && !p->right) {
        action(p, leaf, level);
    } else {
        action(p, preorder , level);
        walk(p->left , action, level + 1);
        action(p, postorder, level);
        walk(p->right, action, level + 1);
        action(p, endorder , level);
    }
}

void twalk(const void *root, act *action)
{
    walk(root, action, 0);
}

void tdestroy(void *root, void (*free_node)(void *nodep))
{
    struct tree *p = root;
    if (!p)
        return;

    tdestroy(p->left , free_node);
    tdestroy(p->right, free_node);
    free_node((void*)p->datum);
    free(p);
}

#endif

#if defined(_WIN32)

#if !defined(_MSC_VER)
/* destroy tree recursively and call free_node on each node key */
void tdestroy(void *root, void (*free_node)(void *))
{
  node_t *p = (node_t *)root;
  if (!p)
    return;

  tdestroy(p->llink , free_node);
  tdestroy(p->rlink, free_node);
  free_node((void*)p->key);
  free(p);
}
#endif

static const struct
{
  DWORD winerr;
  int doserr;
}
doserrors[] =
{
  {ERROR_INVALID_FUNCTION, EINVAL},
  {ERROR_FILE_NOT_FOUND, ENOENT},
  {ERROR_PATH_NOT_FOUND, ENOENT},
  {ERROR_TOO_MANY_OPEN_FILES, EMFILE},
  {ERROR_ACCESS_DENIED, EACCES},
  {ERROR_INVALID_HANDLE, EBADF},
  {ERROR_ARENA_TRASHED, ENOMEM},
  {ERROR_NOT_ENOUGH_MEMORY, ENOMEM},
  {ERROR_INVALID_BLOCK, ENOMEM},
  {ERROR_BAD_ENVIRONMENT, E2BIG},
  {ERROR_BAD_FORMAT, ENOEXEC},
  {ERROR_INVALID_ACCESS, EINVAL},
  {ERROR_INVALID_DATA, EINVAL},
  {ERROR_INVALID_DRIVE, ENOENT},
  {ERROR_CURRENT_DIRECTORY, EACCES},
  {ERROR_NOT_SAME_DEVICE, EXDEV},
  {ERROR_NO_MORE_FILES, ENOENT},
  {ERROR_LOCK_VIOLATION, EACCES},
  {ERROR_SHARING_VIOLATION, EACCES},
  {ERROR_BAD_NETPATH, ENOENT},
  {ERROR_NETWORK_ACCESS_DENIED, EACCES},
  {ERROR_BAD_NET_NAME, ENOENT},
  {ERROR_FILE_EXISTS, EEXIST},
  {ERROR_CANNOT_MAKE, EACCES},
  {ERROR_FAIL_I24, EACCES},
  {ERROR_INVALID_PARAMETER, EINVAL},
  {ERROR_NO_PROC_SLOTS, EAGAIN},
  {ERROR_DRIVE_LOCKED, EACCES},
  {ERROR_BROKEN_PIPE, EPIPE},
  {ERROR_DISK_FULL, ENOSPC},
  {ERROR_INVALID_TARGET_HANDLE, EBADF},
  {ERROR_INVALID_HANDLE, EINVAL},
  {ERROR_WAIT_NO_CHILDREN, ECHILD},
  {ERROR_CHILD_NOT_COMPLETE, ECHILD},
  {ERROR_DIRECT_ACCESS_HANDLE, EBADF},
  {ERROR_NEGATIVE_SEEK, EINVAL},
  {ERROR_SEEK_ON_DEVICE, EACCES},
  {ERROR_DIR_NOT_EMPTY, ENOTEMPTY},
  {ERROR_NOT_LOCKED, EACCES},
  {ERROR_BAD_PATHNAME, ENOENT},
  {ERROR_MAX_THRDS_REACHED, EAGAIN},
  {ERROR_LOCK_FAILED, EACCES},
  {ERROR_ALREADY_EXISTS, EEXIST},
  {ERROR_FILENAME_EXCED_RANGE, ENOENT},
  {ERROR_NESTING_NOT_ALLOWED, EAGAIN},
  {ERROR_NOT_ENOUGH_QUOTA, ENOMEM},
  {ERROR_DELETE_PENDING, ENOENT}
};

static void
_dosmaperr(unsigned long e)
{
  int i;

  if (e == 0)
  {
    errno = 0;
    return;
  }

  for (i = 0; i < sizeof(doserrors) / sizeof(doserrors[0]); i++)
  {
    if (doserrors[i].winerr == e)
    {
      int doserr = doserrors[i].doserr;

      errno = doserr;
      return;
    }
  }

  errno = EINVAL;
}

ssize_t
pread(int fd, void *buf, size_t count, off_t offset)
{
  OVERLAPPED overlapped = {0};
  HANDLE h;
  DWORD ret;

  h = (HANDLE)_get_osfhandle(fd);
  if (h == INVALID_HANDLE_VALUE)
  {
    errno = EBADF;
    return -1;
  }

  overlapped.Offset = offset;
  if (!ReadFile(h, buf, count, &ret, &overlapped))
  {
    if (GetLastError() == ERROR_HANDLE_EOF)
      return 0;

    _dosmaperr(GetLastError());
    return -1;
  }

  return ret;
}

ssize_t
pwrite(int fd, const void *buf, size_t count, off_t offset)
{
  OVERLAPPED overlapped = {0};
  HANDLE h;
  DWORD ret;

  h = (HANDLE)_get_osfhandle(fd);
  if (h == INVALID_HANDLE_VALUE)
  {
    errno = EBADF;
    return -1;
  }

  overlapped.Offset = offset;
  if (!WriteFile(h, buf, count, &ret, &overlapped))
  {
    _dosmaperr(GetLastError());
    return -1;
  }

  return ret;
}

#endif /* defined(_WIN32) */

size_t
sys_get_page_size(void)
{
#ifdef _WIN32
  SYSTEM_INFO info;
  GetSystemInfo(&info);
  return info.dwPageSize;
#else
  return sysconf (_SC_PAGESIZE);
#endif
}
