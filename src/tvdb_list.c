#define libtvdb_EXPORTS

#include <stdlib.h>
#include "tvdb.h"
#include "tvdb_list.h"

TVDB_API tvdb_list_front_t *tvdb_list_add(tvdb_list_front_t *p, void *data, size_t len) 
{
   tvdb_list_node_t *n=NULL;

   n = (tvdb_list_node_t *)calloc(1, sizeof(tvdb_list_node_t));
   if (n == NULL)
     return NULL;

   // Set content
   n->data = data;
   n->size = len;

   // When head == tail == NULL init new list
   if(p->head == NULL && p->tail == NULL)
   {
     // First
     p->head = n;
     p->tail = n;
   } else {
     // Link new to back of list.
     p->tail->next = n;
     n->prev = p->tail;
     p->tail = n;
   }
   p->count++;

   return p;
}

TVDB_API void tvdb_list_init(tvdb_list_front_t *front)
{
  memset(front, 0, sizeof(tvdb_list_front_t));
}

TVDB_API void tvdb_list_remove(tvdb_list_front_t *front) 
{
  tvdb_list_node_t *n=NULL;
  tvdb_list_node_t *p=NULL;

  p = front->head;

  /*
   * Move past every element, delete until current element is NULL
   */
  while(p != NULL) {
    n=p->next;
    free(p->data);
    free(p);
    p=n;
  }

  /*
   * Clear head
   */
  memset(front, 0, sizeof(tvdb_list_front_t));
}

#if 0
TVDB_API tvdb_list_node_t **tvdb_list_find(tvdb_list_front_t *n, tvdb_list_node_t *data, cmpfn_t compare) {
  tvdb_list_node_t *node=NULL;

  if (n == 0)
    return 0;

  node = n->head;

  while (node != NULL) {
    if (compare(data, node) == 0)
      return n;
    node = n->next;
  }

  return 0;
}
#endif

TVDB_API unsigned int tvdb_list_size(tvdb_list_front_t *front)
{
  return front->count; 
}

TVDB_API tvdb_list_node_t *tvdb_list_first(tvdb_list_front_t *front)
{
  return front->head;
}

TVDB_API tvdb_list_node_t *tvdb_list_last(tvdb_list_front_t *front)
{
  return front->tail;
}

TVDB_API tvdb_list_node_t *tvdb_list_random(tvdb_list_front_t *front)
{
  unsigned int count=0;
  unsigned int size=0;
  unsigned int random=0;
  tvdb_list_node_t *pos=NULL;
  
  // Get list size
  size = tvdb_list_size(front);
  
  // Get a random position
  random = rand()%size;
  
  // move pointer to position and return pointer
  pos = front->head;
  while(count < random) {
    pos = pos->next;
    count++;
  }

  return pos;
}

TVDB_API void tvdb_list_reset(tvdb_list_front_t *front)
{
  front->current=NULL;
}

TVDB_API tvdb_list_node_t *tvdb_list_next(tvdb_list_front_t *front)
{
  if(front == NULL) 
  {
    return NULL;
  }

  if(front->current == NULL){
    front->current = front->head;
  } else {
    front->current = front->current->next;
  }
  return front->current;
}

TVDB_API tvdb_list_node_t *tvdb_list_prev(tvdb_list_front_t *front)
{
  if(front == NULL) 
  {
    return NULL;
  }
  
  if(front->current == NULL){
    front->current = front->tail;
  } else {
    front->current = front->current->prev;
  }
  return front->current;
}
