//#define LIBTVDB_STATIC

#include <stdio.h>
#include "tvdb.h"

/* Change <MY_API_KEY> to the key provided to you by TVDB.com */
#define MY_API_KEY "<MY_API_KEY>"

void print_mirrors(const tvdb_list_node_t *mirrors) {
   const tvdb_list_node_t *n=NULL;
   tvdb_mirror_t *m=NULL;

   printf("\nMirrors:\n");
  
   n = mirrors;
   while(n != NULL) {
      m = (tvdb_mirror_t *)n->data;
      printf("  id [%i], path [%s], type [%i]\n", m->id, m->path, m->type);
      n = n->next;
   }
}

void print_series(const tvdb_list_node_t *series) {
   const tvdb_list_node_t *n=NULL;
   tvdb_series_t *s=NULL;

   printf("\nSeries:\n");

   n = series;
   while(n != NULL) {
      s = (tvdb_series_t *)n->data;
      printf("\n  id [%i], seriesid [%i], name [%s], overview: %s\n", s->id, s->series_id, s->name, s->overview);
      n = n->next;
   }
}

void print_series_info(const tvdb_list_node_t *series_info) {
   const tvdb_list_node_t *n=NULL;
   tvdb_series_info_t *s=NULL;

   printf("\nSeries info:\n");

   n = series_info;
   while(n != NULL) {
     s = (tvdb_series_info_t *)n->data;
     printf("\n id [%i], seriesid [%i], season [%i], episode [%i], name [%s], overview: %s\n", 
         s->id, s->series_id, s->season_number, s->episode_number, s->episode_name, s->overview);
     n = n->next;
   }
}

int main(int argc, char *argv[]) {
  int rc=0;

  if(argc != 2) {
    fprintf(stderr, "Wrong number of arguments. Usage %s <series name>\n", argv[0]);
    return -1;
  }

  tvdb_list_node_t *mirrors=NULL;
  tvdb_buffer_t mirrors_xml;
  tvdb_mirror_t *m=NULL;

  tvdb_time_t time;
  tvdb_buffer_t time_xml;

  tvdb_list_node_t *series=NULL;
  tvdb_buffer_t series_xml;
  tvdb_series_t *s=NULL;

  tvdb_list_node_t *series_info=NULL;
  tvdb_buffer_t series_info_xml;

  /* Init libtvdb and get a handle */
  htvdb_t tvdb = tvdb_init(MY_API_KEY);

  /* Get mirrors XML from TVDB and parse it */
  tvdb_mirrors(tvdb, &mirrors_xml);
  tvdb_parse_mirrors(&mirrors_xml, 0, &mirrors);
  print_mirrors(mirrors);
  tvdb_free_buffer(&mirrors_xml);

  /* Get server time XML from TVDB and parse it */
  tvdb_time(tvdb, &time_xml);
  tvdb_parse_time(&time_xml, 0, &time);
  printf("\nServer Time [%s]\n", time);
  tvdb_free_buffer(&time_xml);

  /* Get series XML and parse it */
  tvdb_series(tvdb, argv[1], &series_xml);
  rc = tvdb_parse_series(&series_xml, 0, &series);
  if(rc == TVDB_OK) {
    print_series(series);
    tvdb_free_buffer(&series_xml);
  }

  /* Get series info XML of the first found serie and parse it */
  m = (tvdb_mirror_t *)mirrors->data;
  s = (tvdb_series_t *)series->data;
  tvdb_series_info(tvdb, m->path, s->series_id, "en", &series_info_xml);
  rc = tvdb_parse_series_info(&series_info_xml, 0, &series_info);
  if(rc == TVDB_OK) {
    print_series_info(series_info);
    tvdb_free_buffer(&series_info_xml);
  }

  /* Clean up */
  tvdb_list_remove(mirrors);
  tvdb_list_remove(series);
  tvdb_list_remove(series_info);
  tvdb_uninit(tvdb);

  return 0;
}
