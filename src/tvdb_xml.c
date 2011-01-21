#include <libxml/parser.h>
#include "tvdb_types.h"
#include "tvdb_xml.h"
#include "tvdb_results.h"
#include "tvdb_types.h"

#ifdef __cplusplus
extern "C" {
#endif

TVDB_API int tvdb_parse_mirrors(const tvdb_buffer_t *xml, const char *url, tvdb_list_node_t **mirrors) {
   xmlDoc *doc=NULL;
   xmlNode *node=NULL;
   xmlNode *elem=NULL;
   char *tmp=NULL;
   tvdb_mirror_t *mirror=NULL;
   int result=0;

   result = TVDB_E_PARSE_MIRRORS_XML;

   doc = xmlReadMemory(xml->memory, xml->size, url, 0, 0);
   node = xmlDocGetRootElement(doc);

   if (node->type == XML_ELEMENT_NODE && !xmlStrcmp(node->name, (const xmlChar *)"Mirrors")) {
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
                        strcpy(mirror->path, tmp);
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

   if (node->type == XML_ELEMENT_NODE && !xmlStrcmp(node->name, (const xmlChar *)"Items")) {
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

TVDB_API int tvdb_parse_series(const tvdb_buffer_t *xml, const char *url, tvdb_list_node_t **series) {
   xmlDoc *doc;
   xmlNode *node, *elem;
   char *tmp;
   tvdb_series_t *s;
   int result;

   result = TVDB_E_PARSE_SERIES_XML;

   if(xml == NULL || series == NULL) 
   {
     return TVDB_E_PARSE_SERIES_XML;
   }

   doc = xmlReadMemory(xml->memory, xml->size, url, 0, 0);
   node = xmlDocGetRootElement(doc);

   if (node->type == XML_ELEMENT_NODE && !xmlStrcmp(node->name, (const xmlChar *)"Data")) {
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

            tvdb_list_add(series, s, sizeof(s));
         }
      }
      result = TVDB_OK;
   }

   xmlFreeDoc(doc);

   return result;
}

#ifdef __cplusplus
}
#endif
