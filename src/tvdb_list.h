#ifndef TVDB_LIST_H_INCLUDED
#define TVDB_LIST_H_INCLUDED

#include <string.h>
#include "tvdb_exports.h"

typedef int (*cmpfn_t)(tvdb_list_node_t *a, tvdb_list_node_t *b);

TVDB_API tvdb_list_front_t *tvdb_list_add(tvdb_list_front_t *p, void *data, size_t len);
TVDB_API void tvdb_list_init(tvdb_list_front_t *front);
TVDB_API void tvdb_list_remove(tvdb_list_front_t *front);
TVDB_API unsigned int tvdb_list_size(tvdb_list_front_t *front);
TVDB_API tvdb_list_node_t *tvdb_list_random(tvdb_list_front_t *front);
TVDB_API tvdb_list_node_t *tvdb_list_first(tvdb_list_front_t *front);
TVDB_API tvdb_list_node_t *tvdb_list_last(tvdb_list_front_t *front);
TVDB_API void tvdb_list_reset(tvdb_list_front_t *front);
TVDB_API tvdb_list_node_t *tvdb_list_next(tvdb_list_front_t *front);
TVDB_API tvdb_list_node_t *tvdb_list_prev(tvdb_list_front_t *front);

#if 0
TVDB_API tvdb_list_node_t *tvdb_list_add(tvdb_list_node_t **p, void *data, size_t len);
TVDB_API void tvdb_list_remove(tvdb_list_node_t *p);
TVDB_API tvdb_list_node_t **tvdb_list_find(tvdb_list_node_t **n, tvdb_list_node_t *data, cmpfn_t compare);
TVDB_API tvdb_list_node_t *tvdb_list_new();
TVDB_API void tvdb_list_free(tvdb_list_node_t *node);
#endif

#endif /* TVDB_LIST_H_INCLUDED */
