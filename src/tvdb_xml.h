#ifndef TVDB_XML_H_INCLUDED
#define TVDB_XML_H_INCLUDED

#include "tvdb_exports.h"
#include "tvdb_types.h"
#include "tvdb_list.h"

#ifdef __cplusplus
extern "C" {
#endif

TVDB_API int tvdb_parse_mirrors(const tvdb_buffer_t *xml, const char *url, tvdb_list_front_t *mirrors);
TVDB_API int tvdb_parse_time(const tvdb_buffer_t *xml, const char *url, tvdb_time_t *server_time);
TVDB_API int tvdb_parse_series(const tvdb_buffer_t *xml, const char *url, tvdb_list_front_t *series);
TVDB_API int tvdb_parse_series_info(const tvdb_buffer_t *xml, const char *url, tvdb_list_front_t *series);
TVDB_API int tvdb_parse_rating(const tvdb_buffer_t *xml, const char *url, float *rating);
TVDB_API int tvdb_parse_updates(const tvdb_buffer_t *xml, const char *url, tvdb_list_front_t *updates);

#ifdef __cplusplus
}
#endif

#endif /* TVDB_XML_H_INCLUDED */
