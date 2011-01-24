//#define LIBTVDB_STATIC

#include <stdio.h>
#include "tvdb.h"

/* Change <MY_API_KEY> to the key provided to you by TVDB.com */
#define MY_API_KEY "8C0846801AB418E3"

void print_mirrors(tvdb_list_front_t *mirrors) {
   tvdb_list_node_t *n=NULL;
   tvdb_mirror_t *m=NULL;

   tvdb_list_reset(mirrors);

   printf("\nMirrors:\n");

   n=tvdb_list_next(mirrors);
   while(n != NULL) {
      m = (tvdb_mirror_t *)n->data;
      printf("  id [%i], path [%s], type [%i]\n", m->id, m->path, m->type);
      n=tvdb_list_next(mirrors);
   }
}

void print_series(htvdb_t htvdb, tvdb_list_front_t *series) {
   const tvdb_list_node_t *n=NULL;
   tvdb_series_t *s=NULL;
   tvdb_buffer_t buf;
   int rc=0;

   tvdb_init_buffer(&buf);
   tvdb_list_reset(series);

   printf("\nSeries:\n");

   n = tvdb_list_next(series);
   while(n != NULL) {
      s = (tvdb_series_t *)n->data;
      printf("\n  id [%i], seriesid [%i], name [%s], overview: %s\n", s->id, s->series_id, s->name, s->overview);
      // Get image banner, and as 'proof' we got it, diplay filesize.
      rc = tvdb_banners(htvdb, s->banner, &buf);
      printf("Banner file size: %ld\n", buf.size);
      tvdb_free_buffer(&buf);
      n = tvdb_list_next(series);
   }
}

void print_series_info(tvdb_list_front_t *series_info) {
   const tvdb_list_node_t *n=NULL;
   tvdb_series_info_t *s=NULL;

   tvdb_list_reset(series_info);

   printf("\nSeries info:\n");

   n = tvdb_list_next(series_info);
   while(n != NULL) {
     s = (tvdb_series_info_t *)n->data;
     printf("\n id [%i], seriesid [%i], season [%i], episode [%i], name [%s], overview: %s\n", 
         s->id, s->series_id, s->season_number, s->episode_number, s->episode_name, s->overview);
     n = tvdb_list_next(series_info);
   }
}

int main(int argc, char *argv[]) {
  int rc=0;

  if(argc != 2) {
    fprintf(stderr, "Wrong number of arguments. Usage %s <series name>\n", argv[0]);
    return -1;
  }

  tvdb_list_front_t mirrors;
  tvdb_buffer_t mirrors_xml;

  tvdb_time_t time;
  tvdb_buffer_t time_xml;

  tvdb_list_front_t series;
  tvdb_list_node_t *series_node=NULL;
  tvdb_buffer_t series_xml;
  tvdb_series_t *s=NULL;

  tvdb_list_front_t series_info;
  tvdb_buffer_t series_info_xml;

  float rating=0.0;
  tvdb_buffer_t series_rating_xml;

  /* Init libtvdb and get a handle */
  htvdb_t tvdb = tvdb_init(MY_API_KEY);

  /* Initialize lists */
  tvdb_list_init(&mirrors);
  tvdb_list_init(&series);
  tvdb_list_init(&series_info);

  /* Get mirrors XML from TVDB and parse it */
  tvdb_mirrors(tvdb, &mirrors_xml);
  tvdb_parse_mirrors(&mirrors_xml, 0, &mirrors);
  print_mirrors(&mirrors);
  tvdb_free_buffer(&mirrors_xml);

  /* Get server time XML from TVDB and parse it */
  tvdb_time(tvdb, &time_xml);
  tvdb_parse_time(&time_xml, 0, &time);
  printf("\nServer Time [%s]\n", time);
  tvdb_free_buffer(&time_xml);

  /* Get series XML and parse it */
  tvdb_series(tvdb, argv[1], "en", &series_xml);
  rc = tvdb_parse_series(&series_xml, 0, &series);
  if(rc == TVDB_OK) {
    print_series(tvdb, &series);
    tvdb_free_buffer(&series_xml);
  }

  /* Get series info XML of the first found serie and parse it */
  if( tvdb_list_size(&mirrors) != 0 && tvdb_list_size(&series) != 0) {
    series_node = tvdb_list_first(&series);
    s = (tvdb_series_t *)series_node->data;
    tvdb_series_info(tvdb, s->series_id, "en", &series_info_xml);
    rc = tvdb_parse_series_info(&series_info_xml, 0, &series_info);
    if(rc == TVDB_OK) {
      print_series_info(&series_info);
      tvdb_free_buffer(&series_info_xml);
    }
  }

  /* Rate a series and get the new rating including ours back */
  rc = tvdb_rate(tvdb, tvdb_type_series, 1983237, 7, &series_rating_xml);
  if(rc == TVDB_OK) {
    tvdb_parse_rating(&series_rating_xml, NULL, &rating);
    printf("XML:\n%s\n", series_rating_xml.memory);
    printf("Series rating after vote [%.2f].\n", rating);
    tvdb_free_buffer(&series_rating_xml);
  }

  /* Clean up */
  tvdb_list_remove(&mirrors);
  tvdb_list_remove(&series);
  tvdb_list_remove(&series_info);
  tvdb_uninit(tvdb);

  return 0;
}
