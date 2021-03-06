#include <libxml/parser.h>
#include "tvdb.h"
#include "tvdb_types.h"
#include "tvdb_xml.h"
#include "tvdb_results.h"
#include "tvdb_types.h"

#ifdef __cplusplus
extern "C" {
#endif

TVDB_API int tvdb_parse_mirrors(const tvdb_buffer_t *xml, const char *url, tvdb_list_front_t *mirrors) {
   xmlDoc *doc=NULL;
   xmlNode *node=NULL;
   xmlNode *elem=NULL;
   char *tmp=NULL;
   tvdb_mirror_t *mirror=NULL;
   int result=0;

   result = TVDB_E_PARSE_MIRRORS_XML;

   doc = xmlReadMemory(xml->memory, xml->size, url, 0, 0);
   node = xmlDocGetRootElement(doc);

   if (node != NULL && node->type == XML_ELEMENT_NODE && !xmlStrcmp(node->name, (const xmlChar *)"Mirrors")) {
      /* iterate Mirror nodes */
      for (node = node->children; node; node = node->next) {
         if (node->type == XML_ELEMENT_NODE && !xmlStrcmp(node->name, (const xmlChar *)"Mirror")) {
            mirror = tvdb_alloc_mirror();

            /* iterate child elements of each Mirror node */
            for (elem = node->children; elem; elem = elem->next) {
               if (elem->type == XML_ELEMENT_NODE) {
                  if (!xmlStrcmp(elem->name, (const xmlChar *)"id")) {
                     if ((tmp = (char*) xmlNodeGetContent(elem))) {
                        mirror->id = atoi(tmp);
                        xmlFree(tmp);
                     }
                  }
                  else if (!xmlStrcmp(elem->name, (const xmlChar *)"mirrorpath")) {
                     if ((tmp = (char*) xmlNodeGetContent(elem))) {
                        strncpy(mirror->path, tmp, TVDB_STRING_SIZE);
                        xmlFree(tmp);
                     }
                  }
                  else if (!xmlStrcmp(elem->name, (const xmlChar *)"typemask")) {
                     if ((tmp = (char*) xmlNodeGetContent(elem))) {
                        mirror->type = atoi(tmp);
                        xmlFree(tmp);
                     }
                  }
               }
            }

            tvdb_list_add(mirrors, mirror, sizeof(mirror));
         }
      }

      result = TVDB_OK;
   }

   xmlFreeDoc(doc);

   return result;
}

TVDB_API int tvdb_parse_time(const tvdb_buffer_t *xml, const char *url, tvdb_time_t *server_time) {
   xmlDoc *doc=NULL;
   xmlNode *node=NULL;
   xmlNode *elem=NULL;
   char *tmp=NULL;
   int len=0;
   int result=0;

   result = TVDB_E_PARSE_TIME_XML;

   doc = xmlReadMemory(xml->memory, xml->size, url, 0, 0);
   node = xmlDocGetRootElement(doc);

   if (node != NULL && node->type == XML_ELEMENT_NODE && !xmlStrcmp(node->name, (const xmlChar *)"Items")) {
      elem = node->children;

      while (elem && elem->type != XML_ELEMENT_NODE)
         elem = elem->next;

      if (elem && elem->type == XML_ELEMENT_NODE && !xmlStrcmp(elem->name, (const xmlChar *)"Time")) {
         if ((tmp = (char*) xmlNodeGetContent(elem))) {
            len = xmlStrlen(BAD_CAST tmp);
            memcpy(*server_time, tmp, len);
            (*server_time)[len] = 0;
            xmlFree(tmp);
            result = TVDB_OK;
         }
      }
   }

   xmlFreeDoc(doc);

   return result;
}

/*
 * Get the updates from the XML
 * @arguments
 * xml XML data from the tvdb_series_info function
 * url URL the data came from (can be left NULL)
 * server_time server time on the moment of this update
 * updates list holding updates
 * @return
 */
TVDB_API int tvdb_parse_updates(const tvdb_buffer_t *xml, const char *url, tvdb_list_front_t *updates)
{
  xmlDoc *doc=NULL;
  xmlNode *node=NULL;
  xmlNode *elem=NULL;
  char *tmp=NULL;
  tvdb_updates_t *s=NULL;
  int len=0;

  int result = TVDB_E_PARSE_SERIES_XML;

  if(xml == NULL || updates == NULL) 
  {
    return TVDB_E_PARSE_SERIES_XML;
  }

  doc = xmlReadMemory(xml->memory, xml->size, url, 0, 0);
  node = xmlDocGetRootElement(doc);

  if (node != NULL && node->type == XML_ELEMENT_NODE && !xmlStrcmp(node->name, (const xmlChar *)"Items")) {
    /* iterate Series nodes */
    for (elem = node->children; elem; elem = elem->next) {
      if (elem->type == XML_ELEMENT_NODE) {
        s = tvdb_alloc_updates();
        if (!xmlStrcmp(elem->name, (const xmlChar *)"Time")) {
          if ((tmp = (char*) xmlNodeGetContent(elem))) {
            s->type = tvdb_type_time;
            len = xmlStrlen(BAD_CAST tmp);
            memcpy(s->time, tmp, len);
            (s->time)[len] = 0;
            xmlFree(tmp);
            /* When we found the time things are okay */
            result = TVDB_OK;
          }
        }
        if (!xmlStrcmp(elem->name, (const xmlChar *)"Series")) {
          if ((tmp = (char*) xmlNodeGetContent(elem))) {
            s->type = tvdb_type_series;
            s->id = atoi(tmp);
            xmlFree(tmp);
          }
        }
        if (!xmlStrcmp(elem->name, (const xmlChar *)"Episode")) {
          if ((tmp = (char*) xmlNodeGetContent(elem))) {
            s->type = tvdb_type_episode;
            s->id = atoi(tmp);
            xmlFree(tmp);
          }
        }
        tvdb_list_add(updates, s, sizeof(tvdb_updates_t));
      }
    }
  }

  xmlFreeDoc(doc);

  return result;
}

TVDB_API int tvdb_parse_series(const tvdb_buffer_t *xml, const char *url, tvdb_list_front_t *series) {
  xmlDoc *doc=NULL;
  xmlNode *node=NULL;
  xmlNode *elem=NULL;
  char *tmp=NULL;
  tvdb_series_t *s=NULL;
  int result = TVDB_E_PARSE_SERIES_XML;

  if(xml == NULL || series == NULL) 
  {
    return TVDB_E_PARSE_SERIES_XML;
  }

  doc = xmlReadMemory(xml->memory, xml->size, url, 0, 0);
  node = xmlDocGetRootElement(doc);

  if (node != NULL && node->type == XML_ELEMENT_NODE && !xmlStrcmp(node->name, (const xmlChar *)"Data")) {
    /* iterate Series nodes */
    for (node = node->children; node; node = node->next) {
      if (node->type == XML_ELEMENT_NODE && !xmlStrcmp(node->name, (const xmlChar *)"Series")) {
        s = tvdb_alloc_series();

        /* iterate child elements of each Series node */
        for (elem = node->children; elem; elem = elem->next) {
          if (elem->type == XML_ELEMENT_NODE) {
            if (!xmlStrcmp(elem->name, (const xmlChar *)"id")) {
              if ((tmp = (char*) xmlNodeGetContent(elem))) {
                s->id = atoi(tmp);
                xmlFree(tmp);
              }
                  }
                  else if (!xmlStrcmp(elem->name, (const xmlChar *)"seriesid")) {
                     if ((tmp = (char*) xmlNodeGetContent(elem))) {
                        s->series_id = atoi(tmp);
                        xmlFree(tmp);
                     }
                  }
                  else if (!xmlStrcmp(elem->name, (const xmlChar *)"language")) {
                     if ((tmp = (char*) xmlNodeGetContent(elem))) {
                        strncpy(s->lang, tmp, TVDB_STRING_SIZE);
                        xmlFree(tmp);
                     }
                  }
                  else if (!xmlStrcmp(elem->name, (const xmlChar *)"SeriesName")) {
                     if ((tmp = (char*) xmlNodeGetContent(elem))) {
                        strncpy(s->name, tmp, TVDB_STRING_SIZE);
                        xmlFree(tmp);
                     }
                  }
                  else if (!xmlStrcmp(elem->name, (const xmlChar *)"banner")) {
                     if ((tmp = (char*) xmlNodeGetContent(elem))) {
                        strncpy(s->banner, tmp, TVDB_STRING_SIZE);
                        xmlFree(tmp);
                     }
                  }
                  else if (!xmlStrcmp(elem->name, (const xmlChar *)"Overview")) {
                     if ((tmp = (char*) xmlNodeGetContent(elem))) {
                        strncpy(s->overview, tmp, TVDB_STRING_SIZE);
                        xmlFree(tmp);
                     }
                  }
                  else if (!xmlStrcmp(elem->name, (const xmlChar *)"FirstAired")) {
                     if ((tmp = (char*) xmlNodeGetContent(elem))) {
                        strncpy(s->first_aired, tmp, TVDB_STRING_SIZE);
                        xmlFree(tmp);
                     }
                  }
                  else if (!xmlStrcmp(elem->name, (const xmlChar *)"IMDB_ID")) {
                     if ((tmp = (char*) xmlNodeGetContent(elem))) {
                        strncpy(s->imdb_id, tmp, TVDB_STRING_SIZE);
                        xmlFree(tmp);
                     }
                  }
                  else if (!xmlStrcmp(elem->name, (const xmlChar *)"zap2it_id")) {
                     if ((tmp = (char*) xmlNodeGetContent(elem))) {
                        strncpy(s->zap2it_id, tmp, TVDB_STRING_SIZE);
                        xmlFree(tmp);
                     }
                  }
               }
               tmp=NULL;
            }

            tvdb_list_add(series, s, sizeof(tvdb_series_t));
         }
      }
      result = TVDB_OK;
   }

   xmlFreeDoc(doc);

   return result;
}

TVDB_API int tvdb_parse_series_info(const tvdb_buffer_t *xml, const char *url, tvdb_list_front_t *series_info)
{
  xmlDoc *doc=NULL;
  xmlNode *node=NULL;
  xmlNode *elem=NULL;
  char *tmp=NULL;
  tvdb_series_info_t *s=NULL;
  int result = TVDB_E_PARSE_SERIES_XML;

  if(xml == NULL || series_info == NULL) 
  {
    return TVDB_E_PARSE_SERIES_INFO_XML;
  }

  doc = xmlReadMemory(xml->memory, xml->size, url, 0, 0);
  node = xmlDocGetRootElement(doc);

  if (node != NULL && node->type == XML_ELEMENT_NODE && !xmlStrcmp(node->name, (const xmlChar *)"Data")) {
    /* iterate Series nodes */
    for (node = node->children; node; node = node->next) {
      if (node->type == XML_ELEMENT_NODE && !xmlStrcmp(node->name, (const xmlChar *)"Episode")) {
        s = tvdb_alloc_series_info();

        /* iterate child elements of each Series node 
        */
        for (elem = node->children; elem; elem = elem->next) {
          if (elem->type == XML_ELEMENT_NODE) {
            if (!xmlStrcmp(elem->name, (const xmlChar *)"id")) {
              if ((tmp = (char*) xmlNodeGetContent(elem))) {
                s->id = atoi(tmp);
                xmlFree(tmp);
              }
            }
            else if (!xmlStrcmp(elem->name, (const xmlChar *)"Director")) {
              if ((tmp = (char*) xmlNodeGetContent(elem))) {
                strncpy(s->director, tmp, TVDB_STRING_SIZE);
                xmlFree(tmp);
              }
            }
            else if (!xmlStrcmp(elem->name, (const xmlChar *)"EpisodeName")) {
              if ((tmp = (char*) xmlNodeGetContent(elem))) {
                strncpy(s->episode_name, tmp, TVDB_STRING_SIZE);
                xmlFree(tmp);
              }
            }
            else if (!xmlStrcmp(elem->name, (const xmlChar *)"EpisodeNumber")) {
              if ((tmp = (char*) xmlNodeGetContent(elem))) {
                s->episode_number = atoi(tmp);
                xmlFree(tmp);
              }
            }
            else if (!xmlStrcmp(elem->name, (const xmlChar *)"FirstAired")) {
              if ((tmp = (char*) xmlNodeGetContent(elem))) {
                strncpy(s->first_aired, tmp, TVDB_STRING_SIZE);
                xmlFree(tmp);
              }
            }
            else if (!xmlStrcmp(elem->name, (const xmlChar *)"GuestStars")) {
              if ((tmp = (char*) xmlNodeGetContent(elem))) {
                strncpy(s->guest_stars, tmp, TVDB_STRING_SIZE);
                xmlFree(tmp);
              }
            }
            else if (!xmlStrcmp(elem->name, (const xmlChar *)"IMDB_ID")) {
              if ((tmp = (char*) xmlNodeGetContent(elem))) {
                strncpy(s->imdb_id, tmp, TVDB_STRING_SIZE);
                xmlFree(tmp);
              }
            }
            else if (!xmlStrcmp(elem->name, (const xmlChar *)"Language")) {
              if ((tmp = (char*) xmlNodeGetContent(elem))) {
                strncpy(s->language, tmp, TVDB_STRING_SIZE);
                xmlFree(tmp);
              }
            }
            else if (!xmlStrcmp(elem->name, (const xmlChar *)"Overview")) {
              if ((tmp = (char*) xmlNodeGetContent(elem))) {
                strncpy(s->overview, tmp, TVDB_STRING_SIZE);
                xmlFree(tmp);
              }
            }
            else if (!xmlStrcmp(elem->name, (const xmlChar *)"Rating")) {
              if ((tmp = (char*) xmlNodeGetContent(elem))) {
                s->rating = atof(tmp);
                xmlFree(tmp);
              }
            }
            else if (!xmlStrcmp(elem->name, (const xmlChar *)"RatingCount")) {
              if ((tmp = (char*) xmlNodeGetContent(elem))) {
                s->rating_count = atoi(tmp);
                xmlFree(tmp);
              }
            }
            else if (!xmlStrcmp(elem->name, (const xmlChar *)"SeasonNumber")) {
              if ((tmp = (char*) xmlNodeGetContent(elem))) {
                s->season_number = atoi(tmp);
                xmlFree(tmp);
              }
            }
            else if (!xmlStrcmp(elem->name, (const xmlChar *)"Writer")) {
              if ((tmp = (char*) xmlNodeGetContent(elem))) {
                strncpy(s->writer, tmp, TVDB_STRING_SIZE);
                xmlFree(tmp);
              }
            }
            else if (!xmlStrcmp(elem->name, (const xmlChar *)"filename")) {
              if ((tmp = (char*) xmlNodeGetContent(elem))) {
                strncpy(s->filename, tmp, TVDB_STRING_SIZE);
                xmlFree(tmp);
              }
            }
            else if (!xmlStrcmp(elem->name, (const xmlChar *)"seasonid")) {
              if ((tmp = (char*) xmlNodeGetContent(elem))) {
                s->season_id = atoi(tmp);
                xmlFree(tmp);
              }
            }
            else if (!xmlStrcmp(elem->name, (const xmlChar *)"seriesid")) {
              if ((tmp = (char*) xmlNodeGetContent(elem))) {
                s->series_id = atoi(tmp);
                xmlFree(tmp);
              }
            }
          }
        }
        tvdb_list_add(series_info, s, sizeof(tvdb_series_info_t));
      }
    }
    result = TVDB_OK;
  }

  xmlFreeDoc(doc);

  return result;
}

TVDB_API int tvdb_parse_rating(const tvdb_buffer_t *xml, const char *url, float *rating)
{
  xmlDoc *doc=NULL;
  xmlNode *node=NULL;
  xmlNode *elem=NULL;
  xmlNode *data=NULL;
  int result = TVDB_E_PARSE_RATING_XML;
  char *tmp=NULL;

  if(xml == NULL)
  {
    return TVDB_E_PARSE_RATING_XML;
  }

  *rating = 0.0;

  doc = xmlReadMemory(xml->memory, xml->size, url, 0, 0);
  node = xmlDocGetRootElement(doc);

  if (node != NULL && node->type == XML_ELEMENT_NODE && !xmlStrcmp(node->name, (const xmlChar *)"Data")) {
    for (elem = node->children; elem; elem = elem->next) {
      if (elem->type == XML_ELEMENT_NODE && !xmlStrcmp(elem->name, (const xmlChar *)"Series")) {
        for (data = elem->children; data; data = data->next) {
          if (!xmlStrcmp(data->name, (const xmlChar *)"Rating")) {
            if ((tmp = (char*) xmlNodeGetContent(data))) {
              *rating = atof(tmp);
              xmlFree(tmp);
              if(*rating != 0.0) {
                result = TVDB_OK;
              }
            }
          }
        }
      }
    }
  }
  xmlFreeDoc(doc);

  return result;
}

#ifdef __cplusplus
}
#endif
