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

   //for (n = series; n; n = n->next) {
   //   s = (tvdb_series_t *)n->data;
   //   printf("\n  id [%i], seriesid [%i], name [%s], overview: %s\n", s->id, s->series_id, s->name, s->overview);
   //}
   n = series;
   while(n != NULL) {
      s = (tvdb_series_t *)n->data;
      printf("\n  id [%i], seriesid [%i], name [%s], overview: %s\n", s->id, s->series_id, s->name, s->overview);
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

  tvdb_time_t time;
  tvdb_buffer_t time_xml;

  tvdb_list_node_t *series=NULL;
  tvdb_buffer_t series_xml;

  /* init libtvdb and get a handle */
  htvdb_t tvdb = tvdb_init(MY_API_KEY);

  /* get mirrors XML from TVDB and parse it */
  tvdb_mirrors(tvdb, &mirrors_xml);
  tvdb_parse_mirrors(&mirrors_xml, 0, &mirrors);
  print_mirrors(mirrors);
  tvdb_free_buffer(&mirrors_xml);
  tvdb_list_remove(mirrors);

  /* get server time XML from TVDB and parse it */
  tvdb_time(tvdb, &time_xml);
  tvdb_parse_time(&time_xml, 0, &time);
  printf("\nServer Time [%s]\n", time);
  tvdb_free_buffer(&time_xml);

  /* get series XML and parse it */
  tvdb_series(tvdb, argv[1], &series_xml);
  rc = tvdb_parse_series(&series_xml, 0, &series);
  if(rc == TVDB_OK) {
    print_series(series);
    tvdb_free_buffer(&series_xml);
    tvdb_list_remove(series);
  }

  tvdb_uninit(tvdb);

  return 0;
}
