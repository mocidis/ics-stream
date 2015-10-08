#include <pjlib.h>
#include <pjmedia.h>
#include <stdlib.h>	
#include <stdio.h>
#include "ansi-utils.h"

#include "ics-stream.h"

int main(int argc, char *argv[])
{
    ics_stream_t stream_data;
    char tmp[10];
    pj_status_t status;
    pj_caching_pool cp;
    pj_pool_t *pool;

    status = pj_init();
    pj_caching_pool_init(&cp, &pj_pool_factory_default_policy, 0);
    pool = pj_pool_create(&cp.factory, "app", 4000, 4000, NULL);

    ics_stream_init(&stream_data, &cp, pool);
    ics_stream_create(&stream_data, 4321, "127.0.0.1:1234", "sink");
    ics_stream_connect_with_snd_sink(&stream_data);
    ics_stream_start(&stream_data);
    
    for (;;) {
        if (fgets(tmp, sizeof(tmp), stdin) == NULL) {
            puts("EOF while reading stdin, will quit now..");
            break;
        }
        if (tmp[0] == 'q')
            break;
    }

    //Destroy and clean`
    ics_stream_clean(&stream_data);
    return 0;
}
