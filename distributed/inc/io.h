#ifndef GPIO_H
#define GPIO_H

typedef struct 
{
    char* type; 
    char* tag;
    int gpio;
} gpio_input; 
gpio_input gpio_inputs[6];

int inputs_size;

typedef struct 
{
    char* type;
    char* tag;
    int gpio;
    int outputs_size
} gpio_output;
gpio_output gpio_outputs[7];

int outputs_size;

typedef struct
{
    char* type;
    char* tag;
    int gpio;
} gpio_temps;
gpio_temps gpio_temp;

#endif