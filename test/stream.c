#include <pjlib.h>
#include <pjmedia.h>
#include <stdlib.h>	
#include <stdio.h>
#include "ansi-utils.h"

#include "ics.h"

#include "ics-stream.h"

struct {
    ics_t ics_data;
    ics_stream_t stream_data;
} app_data;

int main(int argc, char *argv[])
{
    char tmp[10];
    pj_status_t status;
 
#if 1
    status = pj_init();
    pj_caching_pool_init(&app_data.ics_data.cp, &pj_pool_factory_default_policy, 0);
    app_data.ics_data.pool = pj_pool_create(&app_data.ics_data.cp.factory, "app", 4000, 4000, NULL);
#endif

#if 0
    ics_init(&app_data.ics_data);
#endif
    ics_stream_init(&app_data.stream_data, &app_data.ics_data.cp, app_data.ics_data.pool);
    printf("Init done.\n");

    ics_stream_create(&app_data.stream_data,1234,"127.0.0.1:4321", "source");
    ics_stream_connect_with_snd_source(&app_data.stream_data);
    ics_stream_start(&app_data.stream_data);
    
    for (;;) {
        if (fgets(tmp, sizeof(tmp), stdin) == NULL) {
            puts("EOF while reading stdin, will quit now..");
            break;
        }
        if (tmp[0] == 'q')
            break;
    }

    //Destroy and clean`
    ics_stream_clean(&app_data.stream_data);
    return 0;
}
