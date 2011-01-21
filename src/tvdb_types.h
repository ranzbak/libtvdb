#ifndef TVDB_TYPES_H_INCLUDED
#define TVDB_TYPES_H_INCLUDED

#define TVDB_STRING_SIZE 2048

/** \brief tvdb context handle
 */
typedef long htvdb_t;

/** \brief Holds data received from TVDB
 */
typedef struct tvdb_buffer {
   char *memory;
   size_t size;
} tvdb_buffer_t;

/** \brief Allocates a buffer with default size
 */
tvdb_buffer_t *tvdb_alloc_buffer();

/** \brief Frees a buffer and any memory it has reserved
 */
void tvdb_free_buffer(tvdb_buffer_t *);

/** \brief Data for one mirror
 */
typedef struct tvdb_mirror {
   int   id;
   char  path[TVDB_STRING_SIZE+1];
   int   type;
} tvdb_mirror_t;

/** \brief Allocates a mirror struct and zeros it
 */
tvdb_mirror_t *tvdb_alloc_mirror();

/** \brief Time from the TVDB server
 */
typedef char tvdb_time_t[16];

/** \brief Allocates a tvdb_time_t struct and zeros it
 */
tvdb_time_t *tvdb_alloc_time();


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

/** \brief Allocates a series struct and zeros it
 */
tvdb_series_t *tvdb_alloc_series();

/*
 * Should be in an generic functions file not types
 * For now we leave it here.
 */
void tvdb_init_buffer(tvdb_buffer_t *buf);

#endif /* TVDB_TYPES_H_INCLUDED */
