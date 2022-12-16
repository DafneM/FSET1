// #include "io.h"
#include "cJSON.h"

char *create_json_message(){

    char message;

    cJSON *json_message;
    json_message = cJSON_CreateObject();

    cJSON_AddItemToObject(json_message, "name", "Dafne");

    message = cJSON_Print(json_message);
    cJSON_Delete(json_message);

    return message;
}