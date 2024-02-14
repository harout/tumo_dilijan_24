#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Ticker.h>
#include <U8g2lib.h>
#include <RadioLib.h>

#include "config.h"

#define PACKET_LENGTH 128
#define FRAMED_PACKET_LENGTH (PACKET_LENGTH + 2 * NUM_SENTINEL_BYTES)

#define PREFIX_BYTE 0x00
#define SUFFIX_BYTE 0xff
#define NUM_SENTINEL_BYTES 3

U8G2_SSD1306_128X64_NONAME_F_HW_I2C *u8g2 = nullptr;
//Ticker ledTicker;

SX1262 radio = NULL;
float frequency = DEFAULT_FREQUENCY;
uint8_t spreadingFactor = MIN_SPREADING_FACTOR;
uint8_t codingRate = MAX_CODING_RATE;
float bandwidth = MAX_BANDWIDTH;
int8_t outputPower = MAX_RADIO_OUTPUT_POWER;

// flag to indicate that a packet was sent or received
volatile bool operationDone = false;

// save transmission states between loops
int transmissionState = RADIOLIB_ERR_NONE;

// Is this module currently the transmitter or a receiver.
bool wasTransmitting = false;
bool wasReceiving = false;

uint8_t prefixBuffer[NUM_SENTINEL_BYTES] = {0x00, 0x00, 0x00};
uint8_t suffixBuffer[NUM_SENTINEL_BYTES] = {0xff, 0xff, 0xff};


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


void displaySettings()
{
  String pwr = ("TX Powwer: " + std::to_string(outputPower)).c_str();

  String sf = "Spreading Factor: ";
  sf += std::to_string(spreadingFactor).c_str();

  String cr = "Coding Rate: ";
  cr += std::to_string(codingRate).c_str();

  String bw = "Bandwidth: ";
  bw += std::to_string(bandwidth).c_str();

  String freq = "Frequency: ";
  freq += std::to_string(DEFAULT_FREQUENCY).c_str();

  debugOutput(pwr, sf, cr, bw, freq);
}

void readRadioSettings()
{
  Serial1.println("Waiting for settings from client.");

  String settings = "";
  // When a serial connection is made, the client 
  // transmits the radio frequency, bandwidth, 
  // spreading factor, coding rate, and output power.
  // The values are comma separated and terminated
  // with a newline character.
  while((settings = Serial.readStringUntil('\n')) == "")
  {
    delay(10);
  }
  Serial1.println("Received settings from client.");
  Serial1.println(settings);

  // Split the string into an array of strings.
  // The first string is the frequency, the second
  // is the bandwidth, the third is the spreading
  // factor, the fourth is the coding rate, and the
  // fifth is the output power.
  String values[5];
  int index = 0;
  int start = 0;
  int end = 0;
  for (int i = 0; i < settings.length(); i++)
  {
    if (settings[i] == ',')
    {
      end = i;
      values[index] = settings.substring(start, end);
      start = end + 1;
      index++;
    }
  }

  values[index] = settings.substring(start, settings.length());

  // The various settings are now in string form in the array.
  // We need to convert them to the correct data type and store
  // them in the appropriate variable.
  // ???
  // ???
  
  Serial1.print("Frequency:");
  Serial1.println(frequency);

  Serial1.print("Bandwidth:");
  Serial1.println(bandwidth);

  Serial1.print("Spreading Factor:");
  Serial1.println(spreadingFactor);

  Serial1.print("Coding Rate:");
  Serial1.println(codingRate);

  Serial1.print("Output Power:");
  Serial1.println(outputPower);
}


void clearSerialInput()
{
  while(Serial.available())
  {
    Serial.read();
  }
}


void checkTx()
{
  if (!operationDone || !wasTransmitting)
  {
    return;
  }

  operationDone = false;
  wasTransmitting = false;

  Serial1.println("Transmitted packet.");
}


void relayRx()
{
  if (!operationDone || !wasReceiving)
  {
    return;
  }

  operationDone = false;
  wasReceiving = false;

  // Create an array to hold a packet from the radio
  // ????

  // Read a packet from the radio
  // ????
  
  if (state != RADIOLIB_ERR_NONE)
  {
    Serial1.println("Failed to read packet from radio.");
    return;
  }

  uint8_t packetLength = radio.getPacketLength();
  if (packetLength != PACKET_LENGTH)
  {
    Serial1.println("Received packet of invalid length.");
    return;
  }

  Serial.write(prefixBuffer, NUM_SENTINEL_BYTES);
  Serial.write(buffer, PACKET_LENGTH);
  Serial.write(suffixBuffer, NUM_SENTINEL_BYTES);
  Serial.flush();

  Serial1.println("Relayed packet to client.");
  Serial1.flush();

  // Turn on the BOARD_LED for 100 ms
  // ??
  // ??
  // ??
}


void relayTx()
{
  if (operationDone || wasTransmitting)
  {
    return;
  }

  if (Serial.available() < FRAMED_PACKET_LENGTH)
  {
    return;
  }

  
  for(size_t i = 0; i < NUM_SENTINEL_BYTES; i++)
  {
    uint8_t b = Serial.read();
    if(b != PREFIX_BYTE)
    {
      Serial1.println("Invalid prefix byte.");
      Serial1.println(b, DEC);
      clearSerialInput();
      return;
    }
  }

  // Create an array to hold the packet data
  // ?????

  // Read packet length bytes into the array
  // ???
  // ???
  // ???

  for(size_t i = 0; i < NUM_SENTINEL_BYTES; i++)
  {
    uint8_t b = Serial.read();
    if(b != SUFFIX_BYTE)
    {
      Serial1.println("Invalid suffix byte.");
      Serial1.println(b, DEC);
      clearSerialInput();
      return;
    }
  }

  operationDone = false;
  wasTransmitting = true;
  wasReceiving = false;
  radio.startTransmit(buffer, PACKET_LENGTH);

  Serial1.println("Started radio transmission.");

  // Turn on the BOARD_LED for 100 ms
  // ??
  // ??
  // ??
}


void setupRadio()
{
  radio = new Module(RADIO_CS_PIN,
                     RADIO_DIO1_PIN,
                     RADIO_RST_PIN,
                     RADIO_BUSY_PIN);

  
  int state = radio.begin(frequency, bandwidth, spreadingFactor, codingRate);
  if (state != RADIOLIB_ERR_NONE)
  {
    debugOutput("Failed!", "Error at radio.begin");
    while (true)
      ;
  }

  radio.implicitHeader(PACKET_LENGTH);

  if (radio.setOutputPower(outputPower) != RADIOLIB_ERR_NONE)
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
    /*
    ledTicker.attach_ms(350, []() {
        static bool level;
        digitalWrite(BOARD_LED, level);
        level = !level;
    });
    */
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
        u8g2->drawStr(0, 30, "Message Relay");
        u8g2->drawUTF8(0, 50, "ԱԲԳԴԵԶԷԸԹԺԻԼԽԾԿ");
        u8g2->sendBuffer();
        //u8g2->setFont(u8g2_font_fur11_tf);
        //delay(2000);
    }
}


void setup() {
  initBoard();
  Serial1.println("Board initialized.");

  readRadioSettings();
  setupRadio();
}

void loop() {
  checkTx();
  relayRx();
  relayTx();

  if (!wasTransmitting && !wasReceiving)
  {
    radio.startReceive();
    wasReceiving = true;
  }
}

