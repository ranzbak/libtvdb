#ifndef TVDB_TYPES_H_INCLUDED
#define TVDB_TYPES_H_INCLUDED


#include <curl/curl.h>

/** \brief Holds tvdb context
 */
typedef struct tvdb_context {
   char key[17];
   CURLcode curl_global;
   CURL *curl;
   char *mirror;
} tvdb_context_t;

/** \brief tvdb context handle
 */
//typedef long htvdb_t;


/** \brief Allocates a buffer with default size
 */
tvdb_buffer_t *tvdb_alloc_buffer();

/** \brief Frees a buffer and any memory it has reserved
 */
void tvdb_free_buffer(tvdb_buffer_t *);
#if 0
/** \brief Data for one mirror
 */
typedef struct tvdb_mirror {
   int   id;
   char  path[TVDB_STRING_SIZE+1];
   int   type;
} tvdb_mirror_t;
#endif

/** \brief Allocates a mirror struct and zeros it
 */
tvdb_mirror_t *tvdb_alloc_mirror();

/** \brief Allocates a tvdb_time_t struct and zeros it
 */
tvdb_time_t *tvdb_alloc_time();

#if 0
/** \brief Data for one series
 */
typedef struct tvdb_series {
   int   id;
   int   series_id;
   char  lang[TVDB_STRING_SIZE+1];
   char  name[TVDB_STRING_SIZE+1];
   char  banner[TVDB_STRING_SIZE+1];
   char  overview[TVDB_STRING_SIZE+1];
   char  first_aired[TVDB_STRING_SIZE+1];
   char  imdb_id[TVDB_STRING_SIZE+1];
   char  zap2it_id[TVDB_STRING_SIZE+1];
} tvdb_series_t;
#endif

/** \brief Allocates a updates struct and zeros it
 */
tvdb_updates_t *tvdb_alloc_updates();

/** \brief Allocates a series struct and zeros it
 */
tvdb_series_t *tvdb_alloc_series();

#if 0
/** \brief Data for one series information
 */
typedef struct tvdb_series_info {
  int   id;
  char  director[TVDB_STRING_SIZE+1];
  char  episode_name[TVDB_STRING_SIZE+1];
  int   episode_number;
  char  first_aired[TVDB_STRING_SIZE+1];
  char  guest_stars[TVDB_STRING_SIZE+1];
  char  imdb_id[TVDB_STRING_SIZE+1];
  char  language[TVDB_STRING_SIZE+1];
  char  overview[TVDB_STRING_SIZE+1];
  float rating;
  int   rating_count;
  int   season_number;
  char  writer[TVDB_STRING_SIZE+1];
  char  filename[TVDB_STRING_SIZE+1];
  int   season_id;
  int   series_id;
} tvdb_series_info_t;
#endif


/** \brief Allocates a series info struct and zeros it
 */
tvdb_series_info_t *tvdb_alloc_series_info();

/*
 * Should be in an generic functions file not types
 * For now we leave it here.
 */
void tvdb_init_buffer(tvdb_buffer_t *buf);

#endif /* TVDB_TYPES_H_INCLUDED */
