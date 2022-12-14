char* create_state_message(){

    states_cJSON = cJSON_CreateObject();
    cJSON_AddStringToObject(states_cJSON, "ip_servidor_central", "127.0.0.1");

    //createPinArray(states_cJSON, "inputs", inputs, inputs_len);

    message = cJSON_Print(states_cJSON);
    cJSON_Delete(states_cJSON);

    return message;
}