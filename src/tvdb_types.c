#include <stdlib.h>
#include <string.h>
#include "tvdb.h"
#include "tvdb_types.h"

tvdb_mirror_t *tvdb_alloc_mirror() {
   tvdb_mirror_t *mirror=NULL;
   mirror = (tvdb_mirror_t *)calloc(1, sizeof(tvdb_mirror_t));
   return mirror;
}

tvdb_updates_t *tvdb_alloc_updates() {
   tvdb_updates_t *updates=NULL;
   updates = (tvdb_updates_t *)calloc(1, sizeof(tvdb_updates_t));
   return updates;
}

tvdb_series_t *tvdb_alloc_series() {
   tvdb_series_t *series=NULL;
   series = (tvdb_series_t *)calloc(1, sizeof(tvdb_series_t));
   return series;
}

tvdb_series_info_t *tvdb_alloc_series_info() {
   tvdb_series_info_t *series_info=NULL;
   series_info = (tvdb_series_info_t *)calloc(1, sizeof(tvdb_series_info_t));
   return series_info;
}

void tvdb_init_buffer(tvdb_buffer_t *buf) {
  memset(buf, 0, sizeof(tvdb_buffer_t)); 
}

void tvdb_free_buffer(tvdb_buffer_t *buf) {
   free(buf->memory);
}
