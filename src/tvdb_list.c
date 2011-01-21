#define libtvdb_EXPORTS

#include <stdlib.h>
#include "tvdb_list.h"

/*
 * For this add method to work, an empty list must be represented by a NULL Pointer.
 */
TVDB_API tvdb_list_node_t *tvdb_list_add(tvdb_list_node_t **p, void *data, size_t len) {
   tvdb_list_node_t *n=NULL;
  
   if (p == NULL){
      return 0;
   }

   n = (tvdb_list_node_t *)calloc(1, sizeof(tvdb_list_node_t));
   if (n == NULL)
     return NULL;

   /*
    * Initialize the new node
    */
   n->data = data;
   n->size = len;
   n->next = *p;
   *p = n;

   return *p;
}

TVDB_API void tvdb_list_remove(tvdb_list_node_t *p) {
  tvdb_list_node_t *n=NULL;

  /*
   * Move past every element, delete until current element is NULL
   */
  while(p != NULL) {
    n=p->next;
    free(p->data);
    free(p);
    p=n;
  }
}

  TVDB_API tvdb_list_node_t **tvdb_list_find(tvdb_list_node_t **n, tvdb_list_node_t *data, cmpfn_t compare) {
    if (n == 0)
      return 0;

    while (*n != 0) {
      if (compare(data, *n) == 0)
        return n;
      n = &(*n)->next;
    }

    return 0;
  }

TVDB_API void tvdb_list_free(tvdb_list_node_t *node) {
  free(node);
}
