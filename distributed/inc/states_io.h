#ifndef STATES_JSON_H
#define STATES_JSON_H

typedef struct
{   
    char *ip_servidor_central;
    int porta_servidor_central;
    char *ip_servidor_distribuido;
    int porta_servidor_distribuido;
    char *nome;
    int id_servidor_distribuido;
    int L_01_state;
    int L_02_state;
    int PR_state;
    int AC_state;
    int AL_BZ_state;

    int SPres_state;
    int SFum_state;
    int SJan_state;
    int SPor_state;
    
    int SC_IN_state;
    int SC_OUT_state;
    int DHT22_state;
} state;
state states;

#endif