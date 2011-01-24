#ifndef TVDB_H_INCLUDED
#define TVDB_H_INCLUDED

/* Return codes from libTVDB */
#define TVDB_OK                        1
#define TVDB_E_ERROR                  -1
#define TVDB_E_MEMORY                 -2
#define TVDB_E_INVALID_HANDLE         -3
#define TVDB_E_INVALID_KEY            -4
#define TVDB_E_CURL_GLOBAL_INIT       -100
#define TVDB_E_CURL_EASY_INIT         -101
#define TVDB_E_CURL                   -102
#define TVDB_E_PARSE_MIRRORS_XML      -200
#define TVDB_E_PARSE_TIME_XML         -201
#define TVDB_E_PARSE_SERIES_XML       -202
#define TVDB_E_PARSE_SERIES_INFO_XML  -203
#define TVDB_E_PARSE_RATING_XML       -204

/* String size for libtvdb text fields. */ 
#define TVDB_STRING_SIZE 2048

typedef long htvdb_t;

/** Data structures 
 */
typedef struct tvdb_mirror {
   int   id;
   char  path[TVDB_STRING_SIZE+1];
   int   type;
} tvdb_mirror_t;

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

typedef struct tvdb_buffer {
   char *memory;
   size_t size;
} tvdb_buffer_t;

typedef struct tvdb_list_node {
   struct tvdb_list_node *next;
   struct tvdb_list_node *prev;
   void *data;
   size_t size;
} tvdb_list_node_t;

typedef struct tvdb_list_front {
   struct tvdb_list_node *head;
   struct tvdb_list_node *tail;
   struct tvdb_list_node *current;
   unsigned int count;
} tvdb_list_front_t;

typedef char tvdb_time_t[16];
typedef enum tvdb_item_type_e{tvdb_type_series, tvdb_type_episode}  tvdb_item_type;

/* TVDB generic functions */
void tvdb_init_buffer(tvdb_buffer_t *buf);
void tvdb_free_buffer(tvdb_buffer_t *);

/* TVDB functions */

htvdb_t tvdb_init(const char* key);
void tvdb_uninit(htvdb_t htvdb);
int tvdb_mirrors(htvdb_t htvdb, tvdb_buffer_t *buf);
int tvdb_time(htvdb_t htvdb, tvdb_buffer_t *buf);
int tvdb_series(htvdb_t htvdb, const char *name, const char *language, tvdb_buffer_t *buf);
int tvdb_series_info(htvdb_t htvdb, int series_id, const char *lang, tvdb_buffer_t *buf);
int tvdb_banners(htvdb_t htvdb, const char *filename, tvdb_buffer_t *buf);
int tvdb_rate(htvdb_t htvdb, tvdb_item_type type, int item_id, int rating, tvdb_buffer_t *buf);
const char* tvdb_error_text(int err);
void tvdb_free(void *);

/* TVDB parser functions */
int tvdb_parse_mirrors(const tvdb_buffer_t *xml, const char *url, tvdb_list_front_t *mirrors);
int tvdb_parse_time(const tvdb_buffer_t *xml, const char *url, tvdb_time_t *server_time);
int tvdb_parse_series(const tvdb_buffer_t *xml, const char *url, tvdb_list_front_t *series);
int tvdb_parse_series_info(const tvdb_buffer_t *xml, const char *url, tvdb_list_front_t *series);
int tvdb_parse_rating(const tvdb_buffer_t *xml, const char *url, float *rating);

/* List manipulation functions */
void tvdb_list_init(tvdb_list_front_t *p);
void tvdb_list_remove(tvdb_list_front_t *front);
unsigned int tvdb_list_size(tvdb_list_front_t *front);
tvdb_list_node_t *tvdb_list_first(tvdb_list_front_t *front);
void tvdb_list_reset(tvdb_list_front_t *front);
tvdb_list_node_t *tvdb_list_next(tvdb_list_front_t *front);
tvdb_list_node_t *tvdb_list_prev(tvdb_list_front_t *front);

#endif /* TVDB_H_INCLUDED */
