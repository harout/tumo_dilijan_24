#include <Arduino.h>
#include <RadioLib.h>
#include "config.h"

#define PACKET_LENGTH 32
#define MY_ID 0x05


SX1262 radio = NULL;
U8G2_SSD1306_128X64_NONAME_F_HW_I2C *u8g2 = nullptr;
Ticker ledTicker;

// flag to indicate that a packet was sent or received
volatile bool operationDone = false;



void initBoard()
{
    SPI.begin(RADIO_SCLK_PIN, RADIO_MISO_PIN, RADIO_MOSI_PIN);
    Wire.begin(OLED_SDA, OLED_SCL);
    Serial.begin(115200);
    delay(2000);

    pinMode(BOARD_LED, OUTPUT);
    ledTicker.attach_ms(350, []() {
        static bool level;
        digitalWrite(BOARD_LED, level);
        level = !level;
    });

    #if OLED_RST
    pinMode(OLED_RST, OUTPUT);
    
    digitalWrite(OLED_RST, HIGH);
    delay(50);

    digitalWrite(OLED_RST, LOW);
    delay(200);

    digitalWrite(OLED_RST, HIGH);
    delay(50);    
    #endif

    delay(1000);

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
        u8g2->drawStr(0, 30, "Button Transmitter");
        u8g2->drawUTF8(0, 50, "աբգ");
        u8g2->sendBuffer();
        //u8g2->setFont(u8g2_font_fur11_tf);
        delay(2000);
    }
}



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

  int state = radio.begin();
  if (state == RADIOLIB_ERR_NONE)
  {
    debugOutput("Radio up.");
  }
  else
  {
    debugOutput("Failed!", "Radio not online.");
    while (true)
      ;
  }

  radio.implicitHeader(PACKET_LENGTH);

  if (radio.setFrequency(DEFAULT_FREQUENCY) != RADIOLIB_ERR_NONE)
  {
    debugOutput("Failed!", "Freq. not set.");
    while (true)
      ;
  }

  if (radio.setOutputPower(MAX_RADIO_OUTPUT_POWER) != RADIOLIB_ERR_NONE)
  {
    debugOutput("Failed!", "Invalid power level.");
    while (true)
      ;
  }

  if (radio.setSpreadingFactor(DEFAULT_SPREADING_FACTOR) != RADIOLIB_ERR_NONE) {
      debugOutput("Failed!", "Bad spreading factor.");
      while (true);
  }

  if (radio.setCodingRate(DEFAULT_CODING_RATE) != RADIOLIB_ERR_NONE) {
      debugOutput("Failed!", "Bad coding rate.");
      while (true);
  }

  if (radio.setBandwidth(DEFAULT_BANDWIDTH) != RADIOLIB_ERR_NONE) {
    debugOutput("Failed!", "Bad bandwidth.");
    while (true);
  }

  // set the function that will be called
  // when packet transmission is finished
  radio.setDio1Action(setFlag);
}


void setup()
{
  initBoard();
  delay(1500);


  debugOutput("Starting up.", "Will bring up radio.");
  delay(1000);

  setupRadio();

  pinMode(BUTTON_PIN, INPUT);
}


void txMessage(){
  uint8_t b[PACKET_LENGTH] = {0};
  b[0] = MY_ID;

  bool isButtonPressed = digitalRead(BUTTON_PIN) == LOW;
  b[1] = isButtonPressed;

  char myIdStr[12];
  sprintf(myIdStr, "ID: %d", MY_ID);

  int status = radio.transmit(b, PACKET_LENGTH);
  if (status == RADIOLIB_ERR_NONE) 
  {
    if (isButtonPressed)
    {
      // display a debug message so that we know what the radio transmitted
    }
    else
    {
      // display a debug message so that we know what the radio transmitted
    }
  }
  else
  {
    debugOutput("Did not TX.", myIdStr);
  }
}


void loop()
{
  txMessage();
  delay(200);
}
