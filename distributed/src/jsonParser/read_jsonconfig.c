#include <stdio.h>
#include "cJSON.h"
#include "cJSON.c"
#include "io.h"
#include "gpio_to_wiring.c"

void read_jsonconfig (char *path){

    FILE* fp = fopen(path, "rb");
    char buf[10000];

    if(fp == NULL) { 
        printf("nao achei");
    }

    if(fp) {
        fread(buf, sizeof(char), 10000, fp); 
        // printf("\nO CONTEÚDO DO ARQUIVO É:\n %s \n", buf);
    }

    cJSON *configuracao_sala_json = cJSON_Parse(buf);

    const cJSON *ip_servidor_central = NULL;
    const cJSON *porta_servidor_central = NULL;
    const cJSON *ip_servidor_distribuido = NULL;
    const cJSON *porta_servidor_distribuido = NULL;
    const cJSON *nome_aux = NULL;

    const cJSON *output = NULL;
    const cJSON *outputs = NULL;
    const cJSON *input = NULL;
    const cJSON *inputs = NULL;
    const cJSON *sensor_temp = NULL;
    const cJSON *sensor_temperatura = NULL;

    // cJSON *configuracao_sala_json = cJSON_Parse(configuracao_sala);

    if (configuracao_sala_json == NULL)
    {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL)
        {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }
        goto end;
    }

    ip_servidor_central = cJSON_GetObjectItemCaseSensitive(configuracao_sala_json, "ip_servidor_central");
    porta_servidor_central = cJSON_GetObjectItemCaseSensitive(configuracao_sala_json, "porta_servidor_central");
    ip_servidor_distribuido = cJSON_GetObjectItemCaseSensitive(configuracao_sala_json, "ip_servidor_distribuido");
    porta_servidor_distribuido = cJSON_GetObjectItemCaseSensitive(configuracao_sala_json, "porta_servidor_distribuido");
    nome_aux = cJSON_GetObjectItemCaseSensitive(configuracao_sala_json, "nome");

    strcpy(nome, nome_aux->valuestring);

    // ip_dist = ip_servidor_distribuido->valuestring;
    porta_dist =  porta_servidor_central->valueint;
    strcpy(ip_dist, ip_servidor_central->valuestring);

    int i = 0;
    outputs = cJSON_GetObjectItemCaseSensitive(configuracao_sala_json, "outputs");
    cJSON_ArrayForEach(output, outputs)
    {
        cJSON *type = cJSON_GetObjectItemCaseSensitive(output, "type");
        cJSON *tag = cJSON_GetObjectItemCaseSensitive(output, "tag");
        cJSON *gpio = cJSON_GetObjectItemCaseSensitive(output, "gpio");

        int gpio_number = convert(gpio->valueint);

        gpio_outputs[i].type = type->valuestring;
        gpio_outputs[i].tag = type->valuestring;
        gpio_outputs[i].gpio = gpio_number;
        i++;
    }
    outputs_size = cJSON_GetArraySize(outputs);

    i = 0;
    inputs = cJSON_GetObjectItemCaseSensitive(configuracao_sala_json, "inputs");
    cJSON_ArrayForEach(input, inputs)
    {
        cJSON *type = cJSON_GetObjectItemCaseSensitive(input, "type");
        cJSON *tag = cJSON_GetObjectItemCaseSensitive(input, "tag");
        cJSON *gpio = cJSON_GetObjectItemCaseSensitive(input, "gpio");

        int gpio_number = convert(gpio->valueint);

        gpio_inputs[i].type = type->valuestring;
        gpio_inputs[i].tag = type->valuestring;
        gpio_inputs[i].gpio = gpio_number;
        i++;
    }
    inputs_size = cJSON_GetArraySize(inputs);

    sensor_temperatura = cJSON_GetObjectItemCaseSensitive(configuracao_sala_json, "sensor_temperatura");
    cJSON_ArrayForEach(sensor_temp, sensor_temperatura)
    {
        cJSON *type = cJSON_GetObjectItemCaseSensitive(sensor_temp, "type");
        cJSON *tag = cJSON_GetObjectItemCaseSensitive(sensor_temp, "tag");
        cJSON *gpio = cJSON_GetObjectItemCaseSensitive(sensor_temp, "gpio");

        int gpio_number = convert(gpio->valueint);

        gpio_temp.type = type->valuestring;
        gpio_temp.tag = type->valuestring;

        gpio_temp.gpio = gpio_number;
    }
    int temp_size = cJSON_GetArraySize(sensor_temperatura);

    // printf("string: %s", nome->valuestring);

    // for  (int x = 0; x<inputs_size; x++){
        // printf("array: %d", gpio_inputs[0].gpio);
    // } 
    printf("json_config %d\n", gpio_outputs[0].gpio);

    end:
        cJSON_Delete(configuracao_sala_json);

}