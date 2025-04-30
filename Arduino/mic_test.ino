#include <driver/i2s.h>

#define I2S_WS  15   // Word Select
#define I2S_SD  32   // Serial Data
#define I2S_SCK 14   // Bit Clock

void setup() {
    Serial.begin(115200);
    
    i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
        .sample_rate = 16000,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
        .communication_format = I2S_COMM_FORMAT_I2S,
        .intr_alloc_flags = 0,
        .dma_buf_count = 8,
        .dma_buf_len = 64,
        .use_apll = false
    };

    i2s_pin_config_t pin_config = {
        .bck_io_num = I2S_SCK,
        .ws_io_num = I2S_WS,
        .data_out_num = I2S_PIN_NO_CHANGE,
        .data_in_num = I2S_SD
    };

    i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
    i2s_set_pin(I2S_NUM_0, &pin_config);
}

void loop() {
    int16_t samples[1024];
    size_t bytesRead;
    
    i2s_read(I2S_NUM_0, samples, sizeof(samples), &bytesRead, portMAX_DELAY);
    
    Serial.print("Mic Data: ");
    for (int i = 0; i < 10; i++) {
        Serial.print(samples[i]);
        Serial.print(" ");
    }
    Serial.println();
    
    delay(500);
}
