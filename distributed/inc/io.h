
typedef struct 
{
    char* type; 
    char* tag;
    int gpio;
} gpio_input; 
gpio_input gpio_inputs[6];

typedef struct 
{
    char* type;
    char* tag;
    int gpio;
} gpio_output;
gpio_output gpio_outputs[7];

typedef struct
{
    char* type;
    char* tag;
    int gpio;
} gpio_temps;
gpio_temps gpio_temp;