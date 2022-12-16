#include "cJSON.h"

char *read_json_message(){

    char buf[10000];

    cJSON *message = cJSON_Parse(buf);

    const cJSON *nome = NULL;

    nome = cJSON_GetObjectItemCaseSensitive(message, "nome");

    printf("%s", nome->valuestring);

    return nome->valuestring;
}