#pragma once

// Pin assignments for the voice_s3 node (ESP32-S3 DevKitC-1, assumed N16R8).
// This is the only place pins may be defined for this board.
// All values below are assumed, confirm against the actual board and wiring
// (docs/product-spec.md section 7, open questions 1).

// Status LED. The DevKitC-1 has an RGB LED on GPIO 48, but the exact board
// is TBD per product spec section 7, so a plain GPIO is used for now.
#define PIN_LED 2

// SSD1306 128x64 OLED over I2C, address 0x3C. Assumed, confirm.
#define PIN_OLED_SDA 8
#define PIN_OLED_SCL 9

// INMP441 I2S microphone. Assumed, confirm.
#define PIN_I2S_WS 4   // word select / LRCLK
#define PIN_I2S_SD 5   // serial data
#define PIN_I2S_SCK 6  // bit clock
