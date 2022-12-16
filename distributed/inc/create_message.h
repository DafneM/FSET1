#ifndef CREATE_JSON_H
#define CREATE_JSON_H

char *create_json_message(void);

#include "cJSON.h"
#include <stdio.h>
#include "states_io.h"

char *create_json_message(){

    char *string = NULL;

    cJSON *L_01_state = NULL;
    cJSON *L_02_state = NULL;
    cJSON *PR_state = NULL;
    cJSON *AC_state = NULL;
    cJSON *AL_BZ_state = NULL;
    cJSON *SPres_state = NULL;
    cJSON *SFum_state = NULL;
    cJSON *SJan_state = NULL;
    cJSON *SPor_state = NULL;
    cJSON *SC_IN_state = NULL;
    cJSON *SC_OUT_state = NULL;
    cJSON *DHT22_state = NULL;

    L_01_state = cJSON_CreateNumber(states.L_01_state);
    L_02_state = cJSON_CreateNumber(states.L_02_state);
    PR_state = cJSON_CreateNumber(states.PR_state);
    AC_state = cJSON_CreateNumber(states.AC_state);

    AL_BZ_state = cJSON_CreateNumber(states.AL_BZ_state);
    SPres_state = cJSON_CreateNumber(states.SPres_state);
    SFum_state = cJSON_CreateNumber(states.SFum_state);
    SJan_state = cJSON_CreateNumber(states.SJan_state);
    SPor_state = cJSON_CreateNumber(states.SPor_state);

    SC_IN_state = cJSON_CreateNumber(states.SC_IN_state);
    SC_OUT_state = cJSON_CreateNumber(states.SC_OUT_state);
    DHT22_state = cJSON_CreateNumber(states.DHT22_state);

    cJSON *message = cJSON_CreateObject();

    cJSON_AddItemToObject(message, "L_01_state", L_01_state);
    cJSON_AddItemToObject(message, "L_02_state", L_02_state);
    cJSON_AddItemToObject(message, "PR_state", PR_state);
    cJSON_AddItemToObject(message, "AC_state", AC_state);

    cJSON_AddItemToObject(message, "AL_BZ_state", AL_BZ_state);
    cJSON_AddItemToObject(message, "SPres_state", SPres_state);
    cJSON_AddItemToObject(message, "SFum_state", SFum_state);
    cJSON_AddItemToObject(message, "SJan_state", SJan_state);
    cJSON_AddItemToObject(message, "SPor_state", SPor_state);

    cJSON_AddItemToObject(message, "SC_IN_state", SC_IN_state);
    cJSON_AddItemToObject(message, "SC_OUT_state", SC_OUT_state);
    cJSON_AddItemToObject(message, "DHT22_state", DHT22_state);

    string = cJSON_Print(message);
    
    if (string == NULL)
    {
        fprintf(stderr, "Failed to print message.\n");
    }

    return string;
}


#endif