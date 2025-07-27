#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "display.h"
#include "aht10.h"

int main()
{
    stdio_init_all();
    display_init(); 
    aht10_i2c_init();
    aht10_init();
    float temp, hum;
    char buffer1[20], buffer2[20];
    
    while (true) {
        aht10_measurement();
        sleep_ms(1000);
        aht10_read_data(&temp, &hum);
        sprintf(buffer1, "Temp: %.2f °C", temp);
        sprintf(buffer2, "Hum: %.2f %%", hum);
        display_status_msg(buffer1);
        display_status_msg(buffer2);
        sleep_ms(1000);
    }
}
