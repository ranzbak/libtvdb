#include <stdlib.h>
#include <string.h>
#include "tvdb_types.h"

tvdb_mirror_t *tvdb_alloc_mirror() {
   tvdb_mirror_t *mirror;
   mirror = (tvdb_mirror_t *)calloc(1, sizeof(tvdb_mirror_t));
   return mirror;
}

tvdb_series_t *tvdb_alloc_series() {
   tvdb_series_t *series;
   series = (tvdb_series_t *)calloc(1, sizeof(tvdb_series_t));
   return series;
}

void tvdb_init_buffer(tvdb_buffer_t *buf) {
  memset(buf, 0, sizeof(tvdb_buffer_t)); 

#if 0
   tvdb_buffer_t *buf;
   buf = (tvdb_buffer_t *)malloc(sizeof(tvdb_series_t));
   buf->memory = (char*)malloc(1); 
   buf->size = 0;
   return buf;
#endif
}

void tvdb_free_buffer(tvdb_buffer_t *buf) {
   free(buf->memory);
   //free(buf);
}
