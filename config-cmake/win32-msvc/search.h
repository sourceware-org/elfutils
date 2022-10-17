#include <../ucrt/search.h>

#pragma once

typedef enum { preorder, postorder, endorder, leaf } VISIT;

void *tsearch(const void *key, void **rootp,
                int (*compar)(const void *, const void *));
void *tfind(const void *key, void *const *rootp,
                int (*compar)(const void *, const void *));
void *tdelete(const void *restrict key, void **restrict rootp,
                int (*compar)(const void *, const void *));
void twalk(const void *root,
                void (*action)(const void *nodep, VISIT which,
                                int depth));

void twalk_r(const void *root,
                void (*action)(const void *nodep, VISIT which,
                                void *closure),
                void *closure);
void tdestroy(void *root, void (*free_node)(void *nodep));
