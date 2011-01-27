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
typedef enum tvdb_item_type_e{tvdb_type_series, tvdb_type_episode, tvdb_type_time} tvdb_item_type;
typedef char tvdb_time_t[16];

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

typedef struct tvdb_updates {
  tvdb_item_type type;
  tvdb_time_t time;
  int   id;
} tvdb_updates_t;

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

/* 
 * Initializes the XML download buffer.
 * @arguments
 * buf pointer to XML buffer to be initialized
 */
void tvdb_init_buffer(tvdb_buffer_t *buf);

/*
 * Free the XML buffer
 * @arguments
 * buf Buffer to be freed
 */
void tvdb_free_buffer(tvdb_buffer_t *buf);

/* TVDB functions */

/*
 * Initialize libtvdb 
 * @arguments
 * key API key, to get from thetvdb.com
 * @return
 * tvdb new tvdb handle.
 */
htvdb_t tvdb_init(const char* key);

/*
 * Uninitialize the tvdb handle
 * @arguments
 * htvdb handle to uninitialize
 */
void tvdb_uninit(htvdb_t htvdb);

/*
 * Get XML data, describing mirrors
 * @arguments
 * htvdb tvdb handle
 * buf the XML buffer to store retrieved data in
 * @return
 * If all went well TVDB_OK will be returned
 */
int tvdb_mirrors(htvdb_t htvdb, tvdb_buffer_t *buf);

/*
 * Get XML data, describing server time
 * @arguments
 * htvdb tvdb handle
 * buf the XML buffer to store retrieved data in
 * @return
 * If all went well TVDB_OK will be returned
 */
int tvdb_time(htvdb_t htvdb, tvdb_buffer_t *buf);

/*
 * Retrieve series and episodes updates
 * @arguments
 * htvdb tvdb handle
 * time the server time of the last update
 * buf The buffer the XML is stored into
 * @return
 * If all went well TVDB_OK will be returned
 */
int tvdb_updates(htvdb_t htvdb, tvdb_time_t *server_time, tvdb_buffer_t *buf);

/*
 * Search thetvdb for series 
 * @arguments
 * htvdb tvdb handle
 * name The name or part of the name of de series
 * language The preferred language the series can be in
 * buf The XML buffer the retrieved data is written to
 * @return
 * If all went well TVDB_OK will be returned
 */
int tvdb_series(htvdb_t htvdb, const char *name, const char *language, tvdb_buffer_t *buf);

/*
 * Retieve series info (Episode and season data)
 * @arguments
 * htvdb tvdb handle
 * series_id the series id from tvdb_series data
 * lang The language the series is preffered to be in
 * buf the XML buffer to store retrieved data in
 * @return
 * If all went well TVDB_OK will be returned
 */
int tvdb_series_info(htvdb_t htvdb, int series_id, const char *lang, tvdb_buffer_t *buf);

/*
 * Get banners of series from the filename field 
 * @arguments
 * htvdb tvdb handle
 * filename filename from the XML data
 * buf buffer storing retrieved image data
 * @return
 * If all went well TVDB_OK will be returned
 */
int tvdb_banners(htvdb_t htvdb, const char *filename, tvdb_buffer_t *buf);

/*
 * Post a rating vote to a series
 * @arguments
 * htvdb tvdb handle
 * type the type of object to post a rating for (tvdb_type_series/tvdb_type_episode)
 * item_id the id from the series or series_info to vote on
 * rating the rating from 0 to 10
 * buf The buffer containing the data
 * @return
 * If all went well TVDB_OK will be returned
 */
int tvdb_rate(htvdb_t htvdb, tvdb_item_type type, int item_id, int rating, tvdb_buffer_t *buf);

/*
 * Translate an error code to an error string
 * @arguments
 * err error code
 * @return
 * error string
 */
const char* tvdb_error_text(int err);

/* TVDB parser functions */

/*
 * Parse data from tvdb_mirrors function
 * @arguments
 * xml XML data from the tvdb_mirrors function
 * url URL the data came from (can be left NULL)
 * mirrors
 * @return
 */
int tvdb_parse_mirrors(const tvdb_buffer_t *xml, const char *url, tvdb_list_front_t *mirrors);

/*
 * Parse data from tvdb_time function
 * @arguments
 * xml XML data from the tvdb_time function
 * url URL the data came from (can be left NULL)
 * server_time
 * @return
 */
int tvdb_parse_time(const tvdb_buffer_t *xml, const char *url, tvdb_time_t *server_time);

/*
 * Get the updates from the XML
 * @arguments
 * xml XML data from the tvdb_series_info function
 * url URL the data came from (can be left NULL)
 * server_time server time on the moment of this update
 * updates list holding updates
 * @return
 */
int tvdb_parse_updates(const tvdb_buffer_t *xml, const char *url, tvdb_list_front_t *updates);

/*
 * Parse data from tvdb_series function
 * @arguments
 * xml XML data from the tvdb_series function
 * url URL the data came from (can be left NULL)
 * series
 * @return
 */
int tvdb_parse_series(const tvdb_buffer_t *xml, const char *url, tvdb_list_front_t *series);

/*
 * Parse data from tvdb_series_info function
 * @arguments
 * xml XML data from the tvdb_series_info function
 * url URL the data came from (can be left NULL)
 * series_info list holding series info
 * @return
 */
int tvdb_parse_series_info(const tvdb_buffer_t *xml, const char *url, tvdb_list_front_t *series_info);

/*
 * Parse data from the tvdb_rating function
 * @arguments
 * xml XML data from the tvdb_rating function
 * url URL the data came from (can be left NULL)
 * rating returned from the server
 * @return
 */
int tvdb_parse_rating(const tvdb_buffer_t *xml, const char *url, float *rating);

/* List manipulation functions */

/*
 * Initialize the list
 * @arguments
 * front The list to initialize
 */
void tvdb_list_init(tvdb_list_front_t *front);

/*
 * Free all elements in the linked list
 * When the front object is allocated it self it has to be freed separately
 * @arguments
 * front pointer to front object of list
 */
void tvdb_list_remove(tvdb_list_front_t *front);

/*
 * Get the number of elements in the list
 * @arguments
 * front pointer to front object of list
 * @return
 * Number of objects in the list
 */
unsigned int tvdb_list_size(tvdb_list_front_t *front);

/*
 * Get the first element in the list
 * @arguments
 * front pointer to front object of list
 * @return
 * Pointer to the first object, NULL when no objects are in the list.
 */
tvdb_list_node_t *tvdb_list_first(tvdb_list_front_t *front);

/*
 * Set the current object back to startingpoint (NULL)
 * @arguments
 * front pointer to front object of list
 */
void tvdb_list_reset(tvdb_list_front_t *front);

/*
 * Get the next item
 * When the current pointer is NULL get the first object
 * @arguments
 * front pointer to front object of list
 * @return
 * Pointer to next object in database, NULL when on the end of the list
 */
tvdb_list_node_t *tvdb_list_next(tvdb_list_front_t *front);

/*
 * Get the previous item
 * When the current pointer is NULL get the last object
 * @arguments
 * front pointer to front object of list
 * @return
 * Pointer to previous object in database, NULL when on the end of the list
 */
tvdb_list_node_t *tvdb_list_prev(tvdb_list_front_t *front);

#endif /* TVDB_H_INCLUDED */
