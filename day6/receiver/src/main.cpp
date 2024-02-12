#include <Arduino.h>
#include <RadioLib.h>
#include <Wire.h>
#include <Ticker.h>
#include <U8g2lib.h>

#include "config.h"

#define PACKET_LENGTH 32

U8G2_SSD1306_128X64_NONAME_F_HW_I2C *u8g2 = nullptr;
Ticker ledTicker;

SX1262 radio = NULL;

// flag to indicate that a packet was sent or received
volatile bool operationDone = false;


void debugOutput(String lineOne,
                 String lineTwo = "",
                 String lineThree = "",
                 String lineFour = "",
                 String lineFive = "",
                 String lineSix = "")
{
  if (u8g2)
  {
    u8g2->setFont(u8g2_font_6x10_tf);
    u8g2->clearBuffer();
    u8g2->drawStr(0, 10, lineOne.c_str());
    u8g2->drawStr(0, 20, lineTwo.c_str());
    u8g2->drawStr(0, 30, lineThree.c_str());
    u8g2->drawStr(0, 40, lineFour.c_str());
    u8g2->drawStr(0, 50, lineFive.c_str());
    u8g2->drawStr(0, 60, lineSix.c_str());
    u8g2->sendBuffer();
  }
}


void fillScreen()
{
  if (u8g2)
  {
    u8g2->clearBuffer();
    u8g2->drawBox(0, 0, 128, 64);
    u8g2->sendBuffer();
  }
}

void checkersScreen()
{
  if (u8g2)
  {
    u8g2->clearBuffer();

    // Add code here to fill the screen with a checkered patter
    // xoxoxoxo
    // xoxoxoxo
    // xoxoxoxo
    // xoxoxoxo    
    //

    u8g2->sendBuffer();
  }
}


void clearScreen()
{
  if (u8g2)
  {
    u8g2->clearBuffer();
    u8g2->sendBuffer();
  }
}


// this function is called when a complete packet
// is transmitted or received by the module
// IMPORTANT: this function MUST be 'void' type
//            and MUST NOT have any arguments!
#if defined(ESP8266) || defined(ESP32)
  ICACHE_RAM_ATTR
#endif
void setFlag(void) {
  // we sent or received  packet, set the flag
  operationDone = true;
}



void setupRadio()
{
  radio = new Module(RADIO_CS_PIN,
                     RADIO_DIO1_PIN,
                     RADIO_RST_PIN,
                     RADIO_BUSY_PIN);

  
  int state = radio.begin(DEFAULT_FREQUENCY,
                          DEFAULT_BANDWIDTH,
                          DEFAULT_SPREADING_FACTOR,
                          DEFAULT_CODING_RATE);
  if (state != RADIOLIB_ERR_NONE)
  {
    debugOutput("Failed!", "Error at radio.begin");
    while (true)
      ;
  }

  radio.implicitHeader(PACKET_LENGTH);

  if (radio.setOutputPower(MAX_RADIO_OUTPUT_POWER) != RADIOLIB_ERR_NONE)
  {
    debugOutput("Failed!", "Invalid power level.");
    while (true)
      ;
  }

  // set the function that will be called
  // when packet transmission is finished
  radio.setDio1Action(setFlag);

  radio.startReceive();
}


void initBoard()
{
    SPI.begin(RADIO_SCLK_PIN, RADIO_MISO_PIN, RADIO_MOSI_PIN);
    Wire.begin(OLED_SDA, OLED_SCL);
    
    Serial.begin(115200);
    Serial1.begin(115200, SERIAL_8N1, 47, 48);

    Serial1.println("Initializing board.");
    //delay(2000);

    pinMode(BOARD_LED, OUTPUT);
    ledTicker.attach_ms(350, []() {
        static bool level;
        digitalWrite(BOARD_LED, level);
        level = !level;
    });
    Serial1.println("Set up ticker.");

    #if OLED_RST
    pinMode(OLED_RST, OUTPUT);
    
    digitalWrite(OLED_RST, HIGH);
    delay(50);

    digitalWrite(OLED_RST, LOW);
    delay(200);

    digitalWrite(OLED_RST, HIGH);
    delay(50);    
    #endif

    Serial1.println("Reset OLED.");
    //delay(1000);

    Wire.beginTransmission(0x3C);
    if (Wire.endTransmission() == 0) {
        u8g2 = new U8G2_SSD1306_128X64_NONAME_F_HW_I2C(U8G2_R0, OLED_RST);
        u8g2->begin();
        u8g2->clearBuffer();
        u8g2->setFlipMode(0);
        u8g2->setFontMode(1); // Transparent
        u8g2->setDrawColor(1);
        u8g2->setFontDirection(0);
        u8g2->setFont(u8g2_font_t0_11_t_all);

        u8g2->drawStr(0, 10, "TUMO");
        u8g2->drawStr(0, 30, "Receiver");
        u8g2->drawUTF8(0, 50, "ԱԲԳ");
        u8g2->sendBuffer();
    }
}


void setup()
{
  initBoard();
  delay(1500);

  //
  // Add code here to print your name to the screen
  // 

  delay(1000);

  setupRadio();
}


void doRxWork()
{
  if (!operationDone)
  {
    return;
  }
  
  operationDone = false;

  uint8_t buffer[PACKET_LENGTH];
  int state = radio.readData(buffer, PACKET_LENGTH);
  if (state != RADIOLIB_ERR_NONE)
  {
    debugOutput("bad packet?");
    return;
  }

  uint8_t packetLength = radio.getPacketLength();
  if (packetLength != PACKET_LENGTH)
  {
    return;
  }

  if (buffer[0] == 0x00) 
  {
    fillScreen();
  } 
  else if (buffer[0] == 0x01)
  {
    clearScreen();
  }

  // Add code to draw a checkered screen if a packet is
  // received which has 0x02 as its first byte
}


void loop()
{
  doRxWork();
}
