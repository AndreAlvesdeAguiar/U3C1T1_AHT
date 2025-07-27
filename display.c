#include "display.h"
#include "hardware/i2c.h"
#include "inc/ssd1306.h"
#include <string.h>
#include <stdio.h>

// Área de renderização do display
static struct render_area frame_area = {
    .start_column = 0,
    .end_column = ssd1306_width - 1,
    .start_page = 0,
    .end_page = ssd1306_n_pages - 1
};

// Inicialização do I2C e do display SSD1306
void display_init(void) {
    // Inicializa o I2C
    i2c_init(i2c1, SSD1306_I2C_CLOCK * 1000);
    gpio_set_function(SSD1306_I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(SSD1306_I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(SSD1306_I2C_SDA);
    gpio_pull_up(SSD1306_I2C_SCL);

    // Inicializa o display SSD1306
    ssd1306_init();

    // Define o tamanho do buffer de renderização
    calculate_render_area_buffer_length(&frame_area);
}

// Exibe o status no display
void display_status_msg(char status_msg[]) {
    static char linha1[128] = "";
    static char linha2[128] = "";
    static uint8_t ssd[ssd1306_buffer_length];  // buffer persistente
    static int toggle_line = 0;

    // Atualiza a string correspondente
    if (toggle_line == 0) {
        snprintf(linha1, sizeof(linha1), "%s", status_msg);
    } else {
        snprintf(linha2, sizeof(linha2), "%s", status_msg);
    }

    // Limpa o buffer do display
    memset(ssd, 0, sizeof(ssd));

    // Desenha as duas linhas na tela
    ssd1306_draw_string(ssd, 5, 0, linha1);   // Linha superior (Y=0)
    ssd1306_draw_string(ssd, 5, 32, linha2);  // Linha inferior (Y=16) — ajuste se usar fonte maior

    // Atualiza o display
    render_on_display(ssd, &frame_area);

    // Alterna para a próxima linha na próxima chamada
    toggle_line = !toggle_line;
}


