#define libtvdb_EXPORTS

#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <libxml/parser.h>
#include "tvdb_types.h"
#include "tvdb.h"
#include "tvdb_xml.h"

#define tvdb_default_mirror "http://www.thetvdb.com"

#define member_size(type, member) sizeof(((type *)0)->member)

#define URL_SIZE 1024
typedef char URL[URL_SIZE+1];

static char *select_mirror(tvdb_context_t *tvdb)
{
  tvdb_list_front_t mirrors;
  tvdb_buffer_t mirrors_xml;
  char *mirror=NULL;
  tvdb_mirror_t *m=NULL;
  char *def=NULL;
  
  tvdb_list_init(&mirrors);

  tvdb_mirrors((htvdb_t) tvdb, &mirrors_xml);
  tvdb_parse_mirrors(&mirrors_xml, 0, &mirrors);

  /* When no mirror is found (no API_KEY) use default. */
  if(tvdb_list_size(&mirrors) == 0) 
  {
    def=strdup(tvdb_default_mirror);
    return def;
  }

  // @@TODO Create real random retrieval
  m = (tvdb_mirror_t*) tvdb_list_random(&mirrors)->data;
  mirror = strdup(m->path);
  
  tvdb_free_buffer(&mirrors_xml);
  tvdb_list_remove(&mirrors);

  return mirror;
}

TVDB_API htvdb_t tvdb_init(const char* key) {
   tvdb_context_t *tvdb=NULL;
   size_t key_len=0;

   xmlInitParser();
   LIBXML_TEST_VERSION

   key_len = strlen(key);

   if (key_len > member_size(tvdb_context_t, key) - 1)
      return TVDB_E_INVALID_KEY;

   tvdb = (tvdb_context_t *)calloc(1, sizeof(tvdb_context_t));
   
   if (!tvdb)
      return TVDB_E_MEMORY;
      
   memcpy(tvdb->key, key, key_len);
   tvdb->key[key_len] = 0;

   if ((tvdb->curl_global = curl_global_init(CURL_GLOBAL_ALL))) {
      free(tvdb);
      return TVDB_E_CURL_GLOBAL_INIT;
   }

   if ((tvdb->curl = curl_easy_init()) == 0) {
      curl_global_cleanup();
      free(tvdb);
      return TVDB_E_CURL_EASY_INIT;
   }

   // Get random mirror
   tvdb->mirror = select_mirror(tvdb);

   return (htvdb_t)tvdb;
}

TVDB_API void tvdb_uninit(htvdb_t htvdb) {
   tvdb_context_t *tvdb;

   if (htvdb) {
      tvdb = (tvdb_context_t *)htvdb;

      free(tvdb->mirror);

      if (tvdb->curl)
         curl_easy_cleanup(tvdb->curl);

      if (tvdb->curl_global == 0)
         curl_global_cleanup();

      xmlCleanupParser();

      
      free(tvdb);
   }
}

/*******************************************************************
 * stuff that probably needs to be in another file
 *******************************************************************/

size_t write_buf_cb(void *ptr, size_t size, size_t nmemb, void *data)
{
   size_t realsize = size *nmemb;
   tvdb_buffer_t *buf = (tvdb_buffer_t*)data;

   buf->memory = (char*)realloc(buf->memory, buf->size + realsize + 1);
   if (buf->memory == NULL) {
      /* out of memory! */ 
      printf("not enough memory (realloc returned NULL)\n");
      exit(EXIT_FAILURE);
   }

   memcpy(&(buf->memory[buf->size]), ptr, realsize);
   buf->size += realsize;
   buf->memory[buf->size] = 0;

   return realsize;
}

#if 0
size_t write_file_cb(void* ptr, size_t size, size_t nmemb, file_fetcher* ffetch) {
   ffetch->m_hf->Put((const byte*)ptr, size* nmemb);
   size_t written = fwrite(ptr, size, nmemb, ffetch->m_fp);
   return written;
}
#endif

CURLcode get_XML(CURL *curl, const char *url, tvdb_buffer_t *buf) {
   CURLcode cc;

   if ((cc = curl_easy_setopt(curl, CURLOPT_URL, url)) != CURLE_OK)
      return cc;

   if ((cc = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_buf_cb)) != CURLE_OK)
      return cc;

   if ((cc = curl_easy_setopt(curl, CURLOPT_WRITEDATA, buf)) != CURLE_OK)
      return cc;

   if ((cc = curl_easy_setopt(curl, CURLOPT_USERAGENT, "libtvdb-agent/1.0")) != CURLE_OK)
      return cc;

   cc = curl_easy_perform(curl);

   return cc;
}

CURLcode get_file(CURL *curl, const char *url, tvdb_buffer_t *file) {
   CURLcode cc;

   if ((cc = curl_easy_setopt(curl, CURLOPT_URL, url)) != CURLE_OK)
      return cc;

   if ((cc = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_buf_cb)) != CURLE_OK)
      return cc;

   if ((cc = curl_easy_setopt(curl, CURLOPT_WRITEDATA, file)) != CURLE_OK)
      return cc;

   if ((cc = curl_easy_setopt(curl, CURLOPT_USERAGENT, "libtvdb-agent/1.0")) != CURLE_OK)
      return cc;

   cc = curl_easy_perform(curl);

   return cc;
}

/*******************************************************************/

TVDB_API int tvdb_mirrors(htvdb_t htvdb, tvdb_buffer_t *buf)  {
   tvdb_context_t *tvdb=NULL;
   URL url;
   CURLcode cc;

   /*
    * Sanity checks
    */
   if (htvdb <= 0)
      return TVDB_E_INVALID_HANDLE;
   tvdb = (tvdb_context_t *)htvdb;

   /*
    * Initialize the buffer
    */
   tvdb_init_buffer(buf);

   snprintf(url, URL_SIZE, "http://www.thetvdb.com/api/%s/mirrors.xml", tvdb->key);

   if ((cc = get_XML(tvdb->curl, url, buf)) != CURLE_OK)
      return TVDB_E_CURL;

   return TVDB_OK;
}

TVDB_API int tvdb_time(htvdb_t htvdb, tvdb_buffer_t *buf) {
   tvdb_context_t *tvdb=NULL;
   URL url;
   CURLcode cc;

   /* 
    * Sanity checks
    */
   if (htvdb <= 0)
      return TVDB_E_INVALID_HANDLE;
   tvdb = (tvdb_context_t *)htvdb;

   /*
    * Initialize the buffer
    */
   tvdb_init_buffer(buf);

   snprintf(url, URL_SIZE, "http://www.thetvdb.com/api/Updates.php?type=none");

   if ((cc = get_XML(tvdb->curl, url, buf)) != CURLE_OK)
      return TVDB_E_CURL;

   return TVDB_OK;
}

TVDB_API int tvdb_series(htvdb_t htvdb, const char *name, const char *language, tvdb_buffer_t *buf) {
   tvdb_context_t *tvdb=NULL;
   URL url;
   CURLcode cc;
   char *urlname=NULL;
   int result=TVDB_E_CURL;

   /* 
    * Sanity checks
    */
   if (htvdb <= 0)
      return TVDB_E_INVALID_HANDLE;
   tvdb = (tvdb_context_t *)htvdb;

   tvdb_init_buffer(buf);

   /*
    * build query
    */
   urlname = curl_easy_escape(tvdb->curl, name, strlen(name));
   snprintf(url, URL_SIZE, "http://www.thetvdb.com/api/GetSeries.php?seriesname=%s&language=%s", urlname, language);

   if ((cc = get_XML(tvdb->curl, url, buf)) == CURLE_OK)
   {
     result=TVDB_OK;
   } 

   curl_free(urlname);

   return result;
}

TVDB_API int tvdb_series_info(htvdb_t htvdb, int series_id, const char *lang, tvdb_buffer_t *buf) {
   tvdb_context_t *tvdb;
   URL url;
   CURLcode cc;
   int result=TVDB_E_CURL;

   if (htvdb <= 0)
      return TVDB_E_INVALID_HANDLE;

   tvdb = (tvdb_context_t *)htvdb;

   /*
    * Initialize buffer
    */
   tvdb_init_buffer(buf);

   /*
    * build query
    */
   snprintf(url, URL_SIZE, "%s/api/%s/series/%d/all/%s.xml", tvdb->mirror, tvdb->key, series_id, lang);
   if ((cc = get_file(tvdb->curl, url, buf)) == CURLE_OK)
   {
      result = TVDB_OK;
   }

   return result;
}

TVDB_API int tvdb_banners(htvdb_t htvdb, const char *filename, tvdb_buffer_t *buf)
{
   URL url;
   CURLcode cc;
   int result=TVDB_E_CURL;
   tvdb_context_t *tvdb;

   if (htvdb <= 0)
      return TVDB_E_INVALID_HANDLE;

   tvdb = (tvdb_context_t *)htvdb;

   /*
    * Initialize buffer
    */
   tvdb_init_buffer(buf);

   /*
    * build query
    */
   snprintf(url, URL_SIZE, "%s/banners/%s", tvdb->mirror, filename);
   if ((cc = get_file(tvdb->curl, url, buf)) == CURLE_OK)
   {
      result = TVDB_OK;
   }

   return result;
}

static char *tvdb_get_item_type(tvdb_item_type type)
{
  char *itemtype=NULL;

  switch(type) {
    case tvdb_type_series:
      itemtype="series";
      break;
    case tvdb_type_episode:
      itemtype="episode";
      break;
  }

  return itemtype;
}

TVDB_API int tvdb_rate(htvdb_t htvdb, tvdb_item_type type, int item_id, int rating, tvdb_buffer_t *buf)
{
  URL url;
  CURLcode cc=0;
  int result=TVDB_E_CURL;
  char *itemtype=NULL;
  tvdb_context_t *tvdb=NULL;

  if (htvdb <= 0)
    return TVDB_E_INVALID_HANDLE;

  if (rating < 0 || rating > 10) 
    return TVDB_E_ERROR;

  tvdb = (tvdb_context_t *)htvdb;

  itemtype = tvdb_get_item_type(type);
  if(itemtype == NULL)
    return TVDB_E_ERROR;

  tvdb_init_buffer(buf);

  snprintf(url, URL_SIZE, "%s/api/User_Rating.php?accountid=%s&itemtype=%s&itemid=%d&rating=%d", tvdb->mirror, tvdb->key, itemtype, item_id, rating  );
  if ((cc = get_file(tvdb->curl, url, buf)) == CURLE_OK)
  {
    result = TVDB_OK;
  }

  return result;
}

TVDB_API const char* tvdb_error_text(int err) {
  switch (err) {
    case TVDB_E_ERROR:
      return "TVDB_E_ERROR";
    case TVDB_E_MEMORY:
      return "TVDB_E_MEMORY";
      case TVDB_E_INVALID_HANDLE:
         return "TVDB_E_INVALID_HANDLE";
      case TVDB_E_INVALID_KEY:
         return "TVDB_E_INVALID_KEY";
      case TVDB_E_CURL_GLOBAL_INIT:
         return "TVDB_E_CURL_GLOBAL_INIT";
      case TVDB_E_CURL_EASY_INIT:
         return "TVDB_E_CURL_EASY_INIT";
      case TVDB_E_CURL:
         return "TVDB_E_CURL";
      case TVDB_E_PARSE_MIRRORS_XML:
         return "TVDB_E_PARSE_MIRRORS_XML";
      case TVDB_E_PARSE_TIME_XML:
         return "TVDB_E_PARSE_TIME_XML";
      case TVDB_E_PARSE_SERIES_XML:
         return "TVDB_E_PARSE_SERIES_XML";
      default:
         break;
   }
   return "UNRECOGNISED ERROR CODE";
}

TVDB_API void tvdb_free(void *mem) {
   free(mem);
}
