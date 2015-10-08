#ifndef __ICS_STREAM_H__
#define __ICS_STREAM_H__

#include <pjlib.h>
#include <pjmedia.h>

typedef struct ics_stream_s ics_stream_t;

struct ics_stream_s {
    pj_caching_pool   *cp;
    pj_pool_t         *pool;
    pjmedia_endpt     *endpoint;
    pjmedia_snd_port *snd_port;
    
    //Stream values
    pjmedia_stream *stream;
};

void ics_stream_init(ics_stream_t *stream_data, pj_caching_pool *cp, pj_pool_t *pool);
void ics_stream_create(ics_stream_t *stream_data, int _local_port, char *_remote_addr, char *endpoint);

void ics_stream_connect_with_snd_source(ics_stream_t *stream_data);
void ics_stream_connect_with_snd_sink(ics_stream_t *stream_data);
void ics_stream_connect_with_file_source(ics_stream_t *stream_data);
void ics_stream_connect_with_snd_sink(ics_stream_t *stream_data);

void ics_stream_start(ics_stream_t *stream_data);
void ics_stream_clean(ics_stream_t *stream_data);
#endif
