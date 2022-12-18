#ifndef READ_JSON_H
#define READ_JSON_H

// void read_json_message(char *buf); 

#include "cJSON.h"
#include <stdio.h>
#include "states_io.h"

void read_json_message(char *buf){

    cJSON *message = cJSON_Parse(buf);

    const cJSON *ip_servidor_central = NULL;
    const cJSON *porta_servidor_central = NULL;
    const cJSON *ip_servidor_distribuido = NULL;
    const cJSON *porta_servidor_distribuido = NULL;
    const cJSON *nome = NULL;
    const cJSON *id_servidor_distribuido = NULL;

    const cJSON *L_01_state = NULL;
    const cJSON *L_02_state = NULL;
    const cJSON *PR_state = NULL;
    
    const cJSON *AC_state = NULL;
    const cJSON *AL_BZ_state = NULL;

    const cJSON *SPres_state = NULL;
    const cJSON *SFum_state = NULL;
    const cJSON *SJan_state = NULL;
    const cJSON *SPor_state = NULL;

    const cJSON *SC_IN_state = NULL;
    const cJSON *SC_OUT_state = NULL;
    const cJSON *SC_qtd = NULL;
    const cJSON *DHT22_state = NULL;

    if (message == NULL)
    {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL)
        {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }
        goto end;
    }

    ip_servidor_central = cJSON_GetObjectItemCaseSensitive(message, "ip_servidor_central");
    porta_servidor_central = cJSON_GetObjectItemCaseSensitive(message, "porta_servidor_central");
    ip_servidor_distribuido = cJSON_GetObjectItemCaseSensitive(message, "ip_servidor_distribuido");
    porta_servidor_distribuido = cJSON_GetObjectItemCaseSensitive(message, "porta_servidor_distribuido");
    nome = cJSON_GetObjectItemCaseSensitive(message, "nome");

    L_01_state = cJSON_GetObjectItemCaseSensitive(message, "L_01_state");
    L_02_state = cJSON_GetObjectItemCaseSensitive(message, "L_02_state");
    PR_state = cJSON_GetObjectItemCaseSensitive(message, "PR_state");
    AC_state = cJSON_GetObjectItemCaseSensitive(message, "AC_state");
    AL_BZ_state = cJSON_GetObjectItemCaseSensitive(message, "AL_BZ_state");

    SPres_state = cJSON_GetObjectItemCaseSensitive(message, "SPres_state");
    SFum_state = cJSON_GetObjectItemCaseSensitive(message, "SFum_state");
    SJan_state = cJSON_GetObjectItemCaseSensitive(message, "SJan_state");
    SPor_state = cJSON_GetObjectItemCaseSensitive(message, "SPor_state");

    SC_IN_state = cJSON_GetObjectItemCaseSensitive(message, "SC_IN_state");
    SC_OUT_state = cJSON_GetObjectItemCaseSensitive(message, "SC_OUT_state");
    SC_qtd = cJSON_GetObjectItemCaseSensitive(message, "SC_qtd");
    DHT22_state = cJSON_GetObjectItemCaseSensitive(message, "DHT22_state");

    states.L_01_state = L_01_state->valueint;
    states.L_02_state = L_02_state->valueint;
    states.PR_state = PR_state->valueint;
    states.AC_state = AC_state->valueint;
    states.AL_BZ_state = AL_BZ_state->valueint;

    states.SPres_state = SPres_state->valueint;
    states.SFum_state = SFum_state->valueint;
    states.SJan_state = SJan_state->valueint;
    states.SPor_state = SPor_state->valueint;

    states.SC_IN_state = SC_IN_state->valueint;
    states.SC_OUT_state = SC_OUT_state->valueint;
    states.SC_qtd = SC_qtd->valueint;

    states.DHT22_state = DHT22_state->valueint;

    end:
        cJSON_Delete(message);

}

#endif