#include <stdlib.h>	
#include <stdio.h>
#include "ics-stream.h"

void ics_stream_init(ics_stream_t *stream_data, pj_caching_pool *cp, pj_pool_t *pool) { 
    if (cp != NULL) {
        stream_data->cp = cp;
        if (pool != NULL)
            stream_data->pool = pool;
        else
            stream_data->pool = pj_pool_create(&stream_data->cp->factory, "pool", 4000, 4000, NULL);
    }
    else {
        stream_data->cp = (pj_caching_pool *)malloc(sizeof(pj_caching_pool));
        pj_caching_pool_init(stream_data->cp, NULL, 1024);
        stream_data->pool = pj_pool_create(&stream_data->cp->factory, "pool", 4000, 4000, NULL);
    }
}
void ics_stream_create(ics_stream_t *stream_data, int _local_port, char *_remote_addr, char *endpoint) {
    pj_status_t status;
    pj_uint16_t local_port;
    //Remote
    pj_sockaddr_in remote_addr;  
    pj_str_t ip;
    pj_uint16_t port;

    pjmedia_stream_info info;
    pjmedia_transport *transport;
    const pjmedia_codec_info *codec_info;

    char addr[50] ;
    char *_ip, *_port;

    //Set values
    //Local
    local_port = _local_port;
    //Remote
    strcpy(addr, _remote_addr);
    _ip = addr;
    _port = strchr(_ip, ':');
    *_port = '\0';
    _port++;

    ip = pj_str(_ip);
    port = atoi(_port);

    pj_bzero(&remote_addr, sizeof(remote_addr));
    status = pj_sockaddr_in_init(&remote_addr, &ip,port);
    //Create Endpoint
    status = pjmedia_endpt_create(&stream_data->cp->factory, NULL, 1, &stream_data->endpoint);
    //Register all supported codecs
    pjmedia_codec_register_audio_codecs(stream_data->endpoint, NULL);
    pjmedia_codec_mgr_get_codec_info(pjmedia_endpt_get_codec_mgr(stream_data->endpoint), 0, &codec_info);

    //Initialize stream info formats
    pj_bzero(&info, sizeof(info));

    info.type = PJMEDIA_TYPE_AUDIO;
    if (0 == strcmp("source", endpoint))
        info.dir = PJMEDIA_DIR_ENCODING;
    else if (0 == strcmp("sink", endpoint))
        info.dir = PJMEDIA_DIR_DECODING;
    else
        printf("Something wrong of endpoint\n");

    pj_memcpy(&info.fmt, codec_info, sizeof(pjmedia_codec_info));
    pj_memcpy(&info.rem_addr, &remote_addr, sizeof(pj_sockaddr_in));

    //Create media transport
    status = pjmedia_transport_udp_create(stream_data->endpoint, NULL, local_port, 0, &transport);

    //Create the stream
    status = pjmedia_stream_create(stream_data->endpoint, stream_data->pool, &info, transport, NULL, &stream_data->stream);
}

void ics_stream_connect_with_snd_source(ics_stream_t *stream_data) {
    pj_status_t status;
    pjmedia_port *stream_port;

    /* Get the port interface of the stream */
    status = pjmedia_stream_get_port(stream_data->stream, &stream_port);

    status = pjmedia_snd_port_create_rec(stream_data->pool, -1,
            PJMEDIA_PIA_SRATE(&stream_port->info),
            PJMEDIA_PIA_CCNT(&stream_port->info),
            PJMEDIA_PIA_SPF(&stream_port->info),
            PJMEDIA_PIA_BITS(&stream_port->info),
            0, &stream_data->snd_port);

    // Connect sound port to stream
    status = pjmedia_snd_port_connect(stream_data->snd_port, stream_port);
}

void ics_stream_connect_with_snd_sink(ics_stream_t *stream_data) {
    pj_status_t status;
    pjmedia_port *stream_port;

    /* Get the port interface of the stream */
    status = pjmedia_stream_get_port(stream_data->stream, &stream_port);

    status = pjmedia_snd_port_create_player(stream_data->pool, -1,
            PJMEDIA_PIA_SRATE(&stream_port->info),
            PJMEDIA_PIA_CCNT(&stream_port->info),
            PJMEDIA_PIA_SPF(&stream_port->info),
            PJMEDIA_PIA_BITS(&stream_port->info),
            0, &stream_data->snd_port);

    // Connect sound port to stream
    status = pjmedia_snd_port_connect(stream_data->snd_port, stream_port);
}
void ics_stream_start(ics_stream_t *stream_data) {
    //START STREAMING
    pjmedia_stream_start(stream_data->stream);
}

void ics_stream_clean(ics_stream_t *stream_data) {
    // Destroy sound device
    if (stream_data->snd_port) {
        pjmedia_snd_port_destroy(stream_data->snd_port);
    }

    // Destroy stream
    if (stream_data->stream) {
        pjmedia_transport *tp;

        tp = pjmedia_stream_get_transport(stream_data->stream);
        pjmedia_stream_destroy(stream_data->stream);

        pjmedia_transport_close(tp);
    }

    pjmedia_endpt_destroy(stream_data->endpoint);

    pj_pool_release(stream_data->pool);
    pj_caching_pool_destroy(stream_data->cp);
    pj_shutdown();
}

