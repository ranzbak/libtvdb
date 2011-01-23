#ifndef TVDB_H_INCLUDED
#define TVDB_H_INCLUDED

#include "tvdb_exports.h"
#include "tvdb_list.h"
#include "tvdb_results.h"
#include "tvdb_xml.h"
#include "tvdb_types.h"

typedef enum tvdb_item_type_e{tvdb_type_series, tvdb_type_episode}  tvdb_item_type;

TVDB_API htvdb_t tvdb_init(const char* key);
TVDB_API void tvdb_uninit(htvdb_t htvdb);
TVDB_API int tvdb_mirrors(htvdb_t htvdb, tvdb_buffer_t *buf);
TVDB_API int tvdb_time(htvdb_t htvdb, tvdb_buffer_t *buf);
TVDB_API int tvdb_series(htvdb_t htvdb, const char *name, const char *language, tvdb_buffer_t *buf);
TVDB_API int tvdb_series_info(htvdb_t htvdb, int series_id, const char *lang, tvdb_buffer_t *buf);
TVDB_API int tvdb_banners(htvdb_t htvdb, const char *filename, tvdb_buffer_t *buf);
TVDB_API int tvdb_rate(htvdb_t htvdb, tvdb_item_type type, int item_id, int rating, tvdb_buffer_t *buf);
TVDB_API const char* tvdb_error_text(int err);
TVDB_API void tvdb_free(void *);

#endif /* TVDB_H_INCLUDED */
