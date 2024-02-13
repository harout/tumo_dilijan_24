#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Ticker.h>
#include <U8g2lib.h>

#define BUILD_FOR                   "heltec"
#define OLED_SDA                     17
#define OLED_SCL                     18
#define OLED_RST                    21

#define RADIO_SCLK_PIN              9
#define RADIO_MISO_PIN              11
#define RADIO_MOSI_PIN              10
#define RADIO_CS_PIN                8
#define RADIO_DIO1_PIN              14
#define RADIO_RST_PIN               12
#define RADIO_BUSY_PIN              13

#define DEFAULT_FREQUENCY			912
#define MAX_RADIO_OUTPUT_POWER		22
#define MIN_SPREADING_FACTOR		5
#define MAX_SPREADING_FACTOR		12
#define DEFAULT_SPREADING_FACTOR    9
#define MAX_CODING_RATE				8
#define DEFAULT_CODING_RATE         8
#define DEFAULT_BANDWIDTH			250

#define BOARD_LED                   35
#define LED_ON                      HIGH

#define BUTTON_PIN                  0


