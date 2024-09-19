#include <Arduino.h>
#include "SimpleWordclock.h"
#include <WiFiManager.h>
#include <Adafruit_NeoPixel.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>
#include <time.h> 

/* -------------------------------------------------------------------------- 
* DEFINES
---------------------------------------------------------------------------- */
#define MY_NTP_SERVER "de.pool.ntp.org"           
#define MY_TZ "CET-1CEST,M3.5.0/02,M10.5.0/03"
#define MINIMAL_BRIGHTNESS 64
#define MAXIMAL_BRIGHTNESS 200 // up to 255
#define MINIMAL_LDR_VALUE 10
#define MAXIMAL_LDR_VALUE 1023

// Defines for MicroWordClock
#define ENABLE  14
#define LATCH   16
#define CLK     13
#define DATA    12

/* -------------------------------------------------------------------------- 
Objects/variables
---------------------------------------------------------------------------- */
Adafruit_NeoPixel LED(114, D2, NEO_GRB + NEO_KHZ800);
LED_Micro Leds = {0};
uint8_t rgb_colors[3] = {0};
uint8_t hue = 0;
uint8_t saturation = 255;
uint8_t brightness = MAXIMAL_BRIGHTNESS;
uint16_t advalue = MAXIMAL_LDR_VALUE;
time_t now;
tm tm;

// Objects for over-the-air update
ESP8266WebServer httpServer(81);
ESP8266HTTPUpdateServer httpUpdater;

/* -------------------------------------------------------------------------- 
* STATIC FUNCTION PROTOTYPES
---------------------------------------------------------------------------- */
void drawPixel(uint8_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b);
void clearPixels(void);
void vEs(uint8_t r, uint8_t g, uint8_t b);
void vIst(uint8_t r, uint8_t g, uint8_t b);
void vFuenfMinuten(uint8_t r, uint8_t g, uint8_t b);
void vZehnMinuten(uint8_t r, uint8_t g, uint8_t b);
void vZwanzigMinuten(uint8_t r, uint8_t g, uint8_t b);
void vViertel(uint8_t r, uint8_t g, uint8_t b);
void vDreiviertel(uint8_t r, uint8_t g, uint8_t b);
void vNach(uint8_t r, uint8_t g, uint8_t b);
void vVor(uint8_t r, uint8_t g, uint8_t b);
void vHalb(uint8_t r, uint8_t g, uint8_t b);
void vZwoelf(uint8_t r, uint8_t g, uint8_t b);
void vZwei(uint8_t r, uint8_t g, uint8_t b);
void vEins(uint8_t r, uint8_t g, uint8_t b);
void vSieben(uint8_t r, uint8_t g, uint8_t b);
void vDrei(uint8_t r, uint8_t g, uint8_t b);
void vFuenf(uint8_t r, uint8_t g, uint8_t b);
void vElf(uint8_t r, uint8_t g, uint8_t b);
void vNeun(uint8_t r, uint8_t g, uint8_t b);
void vVier(uint8_t r, uint8_t g, uint8_t b);
void vAcht(uint8_t r, uint8_t g, uint8_t b);
void vZehn(uint8_t r, uint8_t g, uint8_t b);
void vSechs(uint8_t r, uint8_t g, uint8_t b);
void vUhr(uint8_t r, uint8_t g, uint8_t b);
void vMinute1(uint8_t r, uint8_t g, uint8_t b);
void vMinute2(uint8_t r, uint8_t g, uint8_t b);
void vMinute3(uint8_t r, uint8_t g, uint8_t b);
void vMinute4(uint8_t r, uint8_t g, uint8_t b);
void vSetzteLEDs(uint8_t stunde, uint8_t minute, uint8_t r, uint8_t g, uint8_t b);
void vHSV2RGB(void);
void vLedShiftOut(uint8_t *data, uint16_t brightness);

/* -------------------------------------------------------------------------- 
* FUNCTIONS
---------------------------------------------------------------------------- */
void setup(void)
{
    configTime(MY_TZ, MY_NTP_SERVER);
    WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
    LED.begin();

    // Initialize outputs
    pinMode(ENABLE, OUTPUT);
    pinMode(LATCH, OUTPUT);
    pinMode(CLK, OUTPUT);
    pinMode(DATA, OUTPUT);
    pinMode(ENABLE, HIGH);

    // Show some dummy text, everybody knows the clock is on..
    vEs(0, 0, 255);
    vIst(0, 0, 255);
    vUhr(0, 0, 255);

    LED.show();
    Serial.begin(115200);
    
    WiFiManager wm;
    bool res;
    res = wm.autoConnect("Wordclock");

    if(!res) {
    } 
    else {
        //if you get here you have connected to the WiFi    
        Serial.println("Wifi connected!");
    }

    delay(50);

    //-------------------------------------
    // mDNS
    //-------------------------------------
    MDNS.begin("Wordclock");
    MDNS.addService("http", "tcp", 80);
    MDNS.addService("http", "tcp", 81);

    //-------------------------------------
    // OTA--
    //-------------------------------------
    httpUpdater.setup(&httpServer);
    httpServer.onNotFound([]() {
        // redirect port 81 not found pages to port 80
        httpServer.sendHeader(
            "Location", "http://" + WiFi.localIP().toString() + "/", true);
        httpServer.send(308, "text/plain", "");
    });
    httpServer.begin();

    Serial.printf("Chip ID         : %08X\n", ESP.getChipId());
    Serial.printf("Flash ID        : %08X\n\n", ESP.getFlashChipId());
    Serial.printf("CPU Speed       : %u MHz \n\n", ESP.getCpuFreqMHz());
    Serial.printf("Flash real Size : %u KByte\n", ESP.getFlashChipRealSize() / 1024);
    Serial.printf("Flash ide  Size : %u KByte\n", ESP.getFlashChipSize() / 1024);
    Serial.printf("Flash ide Speed : %u\n\n", ESP.getFlashChipSpeed());
    Serial.printf("Free Heap Size  : %u Byte\n", ESP.getFreeHeap());
    Serial.printf("Sketch Size     : %u Byte \n", ESP.getSketchSize());
    Serial.printf("Free Sketch Size: %u Byte \n\n", ESP.getFreeSketchSpace());
    Serial.printf("SDK Version     : %s\n", ESP.getSdkVersion());
    Serial.print("RESET Info      : ");
    Serial.println(ESP.getResetInfo());
    Serial.print("COMPILED        : ");
    Serial.print(__DATE__);
    Serial.print(" ");
    Serial.println(__TIME__);
    Serial.println("Setup end.");
}

void loop(void)
{ 
    // Collect time from NTP
    time(&now);                       // read the current time
    localtime_r(&now, &tm);           // update the structure tm with the current time
    MDNS.update();

    // Increment hue for color wheel
    hue++;

    if(hue >= 360)
      hue = 0;

    // Collect value from LDR
    advalue = min(analogRead(A0), MAXIMAL_LDR_VALUE);
    advalue = max((int)advalue, MINIMAL_LDR_VALUE);
    Serial.print("LDR AD: ");
    Serial.println(advalue);

    // Adjust Brightness according LDR value, and add some simple floating average to smooth the process
    brightness = ((long)brightness * 9 + map(advalue, MINIMAL_LDR_VALUE, MAXIMAL_LDR_VALUE, MINIMAL_BRIGHTNESS, MAXIMAL_BRIGHTNESS)) / 10;
    Serial.print("Brightness: ");
    Serial.println(brightness);

    vHSV2RGB();

    vSetzteLEDs(tm.tm_hour, tm.tm_min, rgb_colors[0], rgb_colors[1], rgb_colors[2]);
    Serial.print("Current time: ");
    Serial.print(tm.tm_hour);
    Serial.print(":");
    Serial.print(tm.tm_min);
    Serial.print(":");
    Serial.print(tm.tm_sec);
    Serial.println("");
    delay(500);
}

/* -------------------------------------------------------------------------- 
* STATIC FUNCTIONS
---------------------------------------------------------------------------- */
void drawPixel(uint8_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b) {
  uint8_t pixel;
  if(x & 1)
    pixel = (x + 1) * 10 - y - 1;
  else
    pixel =  x * 10 + y;
  LED.setPixelColor(pixel, r, g, b);
}

void clearPixels(void) {
    for(uint8_t i = 0; i<114; i++)
    {
      LED.setPixelColor(i, 0);
    }

    LED_Micro Leds_Clear = {0};
    Leds = Leds_Clear;
}

void vEs(uint8_t r, uint8_t g, uint8_t b)
{
  drawPixel(0, 9, r, g, b);
  drawPixel(1, 9, r, g, b);
  Leds.Led14 = 1;
  Leds.Led15 = 1;  
}

void vIst(uint8_t r, uint8_t g, uint8_t b)
{
  drawPixel(3, 9, r, g, b);
  drawPixel(4, 9, r, g, b);
  drawPixel(5, 9, r, g, b);
  Leds.Led7 = 1;
  Leds.Led8 = 1;
  Leds.Led12 = 1;   
}

void vFuenfMinuten(uint8_t r, uint8_t g, uint8_t b)
{
  drawPixel(7, 9, r, g, b);
  drawPixel(8, 9, r, g, b);
  drawPixel(9, 9, r, g, b);
  drawPixel(10, 9, r, g, b);
  Leds.Led2 = 1;
  Leds.Led3 = 1;  
  Leds.Led4 = 1;
  Leds.Led5 = 1;
}

void vZehnMinuten(uint8_t r, uint8_t g, uint8_t b)
{
  drawPixel(0, 8, r, g, b);
  drawPixel(1, 8, r, g, b);
  drawPixel(2, 8, r, g, b);
  drawPixel(3, 8, r, g, b);
  Leds.Led9 = 1;
  Leds.Led10 = 1;  
  Leds.Led11 = 1;
  Leds.Led32 = 1; 
}

void vZwanzigMinuten(uint8_t r, uint8_t g, uint8_t b)
{
  drawPixel(4, 8, r, g, b);
  drawPixel(5, 8, r, g, b);
  drawPixel(6, 8, r, g, b);
  drawPixel(7, 8, r, g, b);
  drawPixel(8, 8, r, g, b);
  drawPixel(9, 8, r, g, b);
  drawPixel(10, 8, r, g, b);
  Leds.Led17 = 1;  
  Leds.Led18 = 1;  
  Leds.Led19 = 1;  
  Leds.Led20 = 1;  
  Leds.Led29 = 1;
  Leds.Led30 = 1;  
  Leds.Led31 = 1;
}

void vDreiviertel(uint8_t r, uint8_t g, uint8_t b)
{
  drawPixel(0, 7, r, g, b);
  drawPixel(1, 7, r, g, b);
  drawPixel(2, 7, r, g, b);
  drawPixel(3, 7, r, g, b);
  drawPixel(4, 7, r, g, b);
  drawPixel(5, 7, r, g, b);
  drawPixel(6, 7, r, g, b);
  drawPixel(7, 7, r, g, b);
  drawPixel(8, 7, r, g, b);
  drawPixel(9, 7, r, g, b);
  drawPixel(10, 7, r, g, b);
  Leds.Led21 = 1;  
  Leds.Led22 = 1;  
  Leds.Led23 = 1;  
  Leds.Led24 = 1;  
  Leds.Led25 = 1;
  Leds.Led26 = 1;  
  Leds.Led27 = 1;
  Leds.Led28 = 1;
  Leds.Led46 = 1;
  Leds.Led47 = 1;
  Leds.Led48 = 1;
}

void vViertel(uint8_t r, uint8_t g, uint8_t b)
{
  drawPixel(4, 7, r, g, b);
  drawPixel(5, 7, r, g, b);
  drawPixel(6, 7, r, g, b);
  drawPixel(7, 7, r, g, b);
  drawPixel(8, 7, r, g, b);
  drawPixel(9, 7, r, g, b);
  drawPixel(10, 7, r, g, b);
  Leds.Led21 = 1;  
  Leds.Led22 = 1;  
  Leds.Led23 = 1;  
  Leds.Led24 = 1;  
  Leds.Led25 = 1;
  Leds.Led26 = 1;  
  Leds.Led27 = 1;
}

void vNach(uint8_t r, uint8_t g, uint8_t b)
{
  drawPixel(2, 6, r, g, b);
  drawPixel(3, 6, r, g, b);
  drawPixel(4, 6, r, g, b);
  drawPixel(5, 6, r, g, b);
  Leds.Led38 = 1;  
  Leds.Led39 = 1;  
  Leds.Led40 = 1;  
  Leds.Led43 = 1; 
}

void vVor(uint8_t r, uint8_t g, uint8_t b)
{
  drawPixel(6, 6, r, g, b);
  drawPixel(7, 6, r, g, b);
  drawPixel(8, 6, r, g, b);
  Leds.Led35 = 1;  
  Leds.Led36 = 1;  
  Leds.Led37 = 1;
}

void vHalb(uint8_t r, uint8_t g, uint8_t b)
{
  drawPixel(0, 5, r, g, b);
  drawPixel(1, 5, r, g, b);
  drawPixel(2, 5, r, g, b);
  drawPixel(3, 5, r, g, b);
  Leds.Led41 = 1;
  Leds.Led42 = 1;
  Leds.Led63 = 1;
  Leds.Led64 = 1; 
}

void vZwoelf(uint8_t r, uint8_t g, uint8_t b)
{
  drawPixel(5, 5, r, g, b);
  drawPixel(6, 5, r, g, b);
  drawPixel(7, 5, r, g, b);
  drawPixel(8, 5, r, g, b);
  drawPixel(9, 5, r, g, b);
  Leds.Led50 = 1;
  Leds.Led51 = 1;
  Leds.Led52 = 1;
  Leds.Led60 = 1;
  Leds.Led61 = 1;
}

void vZwei(uint8_t r, uint8_t g, uint8_t b)
{
  drawPixel(0, 4, r, g, b);
  drawPixel(1, 4, r, g, b);
  drawPixel(2, 4, r, g, b);
  drawPixel(3, 4, r, g, b);
  Leds.Led77 = 1;
  Leds.Led78 = 1; 
  Leds.Led79 = 1;
  Leds.Led80 = 1;
}

void vEins(uint8_t r, uint8_t g, uint8_t b)
{
  drawPixel(2, 4, r, g, b);
  drawPixel(3, 4, r, g, b);
  drawPixel(4, 4, r, g, b);
  drawPixel(5, 4, r, g, b);
  Leds.Led58 = 1;
  Leds.Led59 = 1;
  Leds.Led77 = 1;
  Leds.Led78 = 1; 
}

void vEin(uint8_t r, uint8_t g, uint8_t b)
{
  drawPixel(2, 4, r, g, b);
  drawPixel(3, 4, r, g, b);
  drawPixel(4, 4, r, g, b);
  Leds.Led59 = 1;
  Leds.Led77 = 1;
  Leds.Led78 = 1;  
}

void vSieben(uint8_t r, uint8_t g, uint8_t b)
{
  drawPixel(5, 4, r, g, b);
  drawPixel(6, 4, r, g, b);
  drawPixel(7, 4, r, g, b);
  drawPixel(8, 4, r, g, b);
  drawPixel(9, 4, r, g, b);
  drawPixel(10, 4, r, g, b);
  Leds.Led53 = 1;
  Leds.Led54 = 1;
  Leds.Led55 = 1;
  Leds.Led56 = 1;
  Leds.Led57 = 1;
  Leds.Led58 = 1;
}

void vDrei(uint8_t r, uint8_t g, uint8_t b)
{
  drawPixel(1, 3, r, g, b);
  drawPixel(2, 3, r, g, b);
  drawPixel(3, 3, r, g, b);
  drawPixel(4, 3, r, g, b);
  Leds.Led71 = 1;
  Leds.Led72 = 1;
  Leds.Led74 = 1;
  Leds.Led75 = 1;
}

void vFuenf(uint8_t r, uint8_t g, uint8_t b)
{
  drawPixel(7, 3, r, g, b);
  drawPixel(8, 3, r, g, b);
  drawPixel(9, 3, r, g, b);
  drawPixel(10, 3, r, g, b);
  Leds.Led65 = 1;
  Leds.Led66 = 1;
  Leds.Led67 = 1;
  Leds.Led68 = 1;
}

void vElf(uint8_t r, uint8_t g, uint8_t b)
{
  drawPixel(0, 2, r, g, b);
  drawPixel(1, 2, r, g, b);
  drawPixel(2, 2, r, g, b);
  Leds.Led73 = 1;
  Leds.Led95 = 1;
  Leds.Led96 = 1; 
}

void vNeun(uint8_t r, uint8_t g, uint8_t b)
{
  drawPixel(3, 2, r, g, b);
  drawPixel(4, 2, r, g, b);
  drawPixel(5, 2, r, g, b);
  drawPixel(6, 2, r, g, b);
  Leds.Led91 = 1;
  Leds.Led92 = 1;
  Leds.Led93 = 1;
  Leds.Led94 = 1; 
}

void vVier(uint8_t r, uint8_t g, uint8_t b)
{
  drawPixel(7, 2, r, g, b);
  drawPixel(8, 2, r, g, b);
  drawPixel(9, 2, r, g, b);
  drawPixel(10, 2, r, g, b);
  Leds.Led81 = 1;
  Leds.Led82 = 1;
  Leds.Led83 = 1;
  Leds.Led84 = 1; 
}

void vAcht(uint8_t r, uint8_t g, uint8_t b)
{
  drawPixel(1, 1, r, g, b);
  drawPixel(2, 1, r, g, b);
  drawPixel(3, 1, r, g, b);
  drawPixel(4, 1, r, g, b);
  Leds.Led102 = 1;
  Leds.Led104 = 1;
  Leds.Led107 = 1;
  Leds.Led109 = 1; 
}

void vZehn(uint8_t r, uint8_t g, uint8_t b)
{
  drawPixel(5, 1, r, g, b);
  drawPixel(6, 1, r, g, b);
  drawPixel(7, 1, r, g, b);
  drawPixel(8, 1, r, g, b);
  Leds.Led87 = 1;
  Leds.Led88 = 1;
  Leds.Led89 = 1;
  Leds.Led90 = 1; 
}

void vSechs(uint8_t r, uint8_t g, uint8_t b)
{
  drawPixel(1, 0, r, g, b);
  drawPixel(2, 0, r, g, b);
  drawPixel(3, 0, r, g, b);
  drawPixel(4, 0, r, g, b);
  drawPixel(5, 0, r, g, b);
  Leds.Led101 = 1;
  Leds.Led103 = 1;
  Leds.Led105 = 1;
  Leds.Led106 = 1;  
  Leds.Led108 = 1;
}

void vUhr(uint8_t r, uint8_t g, uint8_t b)
{
  drawPixel(8, 0, r, g, b);
  drawPixel(9, 0, r, g, b);
  drawPixel(10, 0, r, g, b);
  Leds.Led97 = 1;
  Leds.Led98 = 1;
  Leds.Led113 = 1;
}

void vMinute1(uint8_t r, uint8_t g, uint8_t b)
{
  LED.setPixelColor(113, r, g, b);
  Leds.Led16 = 1;  
}

void vMinute2(uint8_t r, uint8_t g, uint8_t b)
{
  LED.setPixelColor(112, r, g, b);
  Leds.Led1 = 1;  
}

void vMinute3(uint8_t r, uint8_t g, uint8_t b)
{
  LED.setPixelColor(111, r, g, b);
  Leds.Led114 = 1;  
}

void vMinute4(uint8_t r, uint8_t g, uint8_t b)
{
  LED.setPixelColor(110, r, g, b);
  Leds.Led112 = 1;  
}

void vSetzteLEDs(uint8_t stunde, uint8_t minute, uint8_t r, uint8_t g, uint8_t b)
{
  clearPixels();

  vEs(r, g, b);
  vIst(r, g, b);

  if(stunde > 11)
    stunde = stunde - 12;

  if(minute < 5)
  {
    vUhr(r, g, b);
  }
  else if(minute < 10)
  {
    vFuenfMinuten(r, g, b);
    vNach(r, g, b);
  }
  else if(minute < 15)
  {
    vZehnMinuten(r, g, b);
    vNach(r, g, b);
  }
  else if(minute < 20)
  {
    stunde++;
    vViertel(r, g, b);
  }
  else if(minute < 25)
  {
    stunde++;
    vZehnMinuten(r, g, b);
    vVor(r, g, b);
    vHalb(r, g, b);
  }
  else if(minute < 30)
  {
    stunde++;
    vFuenfMinuten(r, g, b);
    vVor(r, g, b);
    vHalb(r, g, b);
  }
  else if(minute < 35)
  {
    stunde++;
    vHalb(r, g, b);
  }
  else if(minute < 40)
  {
    stunde++;
    vFuenfMinuten(r, g, b);
    vNach(r, g, b);
    vHalb(r, g, b);
  }
  else if(minute < 45)
  {
    stunde++;
    vZehnMinuten(r, g, b);
    vNach(r, g, b);
    vHalb(r, g, b);
  }
  else if(minute < 50)
  {
    stunde++;
    vDreiviertel(r, g, b);
  }
  else if(minute < 55)
  {
    stunde++;
    vZehnMinuten(r, g, b);
    vVor(r, g, b);
  }
  else if(minute < 60)
  {
    stunde++;
    vFuenfMinuten(r, g, b);
    vVor(r, g, b);
  }

  switch(stunde)
  {
  case 0:
  case 12:
    vZwoelf(r, g, b);
    break;
  case 1:
    if(minute < 5)
      vEin(r, g, b); // Ein Uhr
    else
      vEins(r, g, b);
    break;
  case 2:
    vZwei(r, g, b);
    break;
  case 3:
    vDrei(r, g, b);
    break;
  case 4:
    vVier(r, g, b);
    break;
  case 5:
    vFuenf(r, g, b);
    break;
  case 6:
    vSechs(r, g, b);
    break;
  case 7:
    vSieben(r, g, b);
    break;
  case 8:
    vAcht(r, g, b);
    break;
  case 9:
    vNeun(r, g, b);
    break;
  case 10:
    vZehn(r, g, b);
    break;
  case 11:
    vElf(r, g, b);
    break;
  }

  minute = minute % 5;

  switch(minute)
  {
  case 1:
    vMinute1(r, g, b);
    break;
  case 2:
    vMinute1(r, g, b);
    vMinute2(r, g, b);
    break;
  case 3:
    vMinute1(r, g, b);
    vMinute2(r, g, b);
    vMinute3(r, g, b);
    break;
  case 4:
    vMinute1(r, g, b);
    vMinute2(r, g, b);
    vMinute3(r, g, b);
    vMinute4(r, g, b);
    break;
  }

  LED.show();
  vLedShiftOut((uint8_t*)&Leds, brightness);
}

void vHSV2RGB(void) {
  uint8_t r,g,b, i, f;
  uint8_t p, q, t;

  if( saturation == 0 )
  { r = g = b = brightness;
  }
  else
  { i=hue/43;
    f=hue%43;
    p = (brightness * (255 - saturation))/256;
    q = (brightness * ((10710 - (saturation * f))/42))/256;
    t = (brightness * ((10710 - (saturation * (42 - f)))/42))/256;

    switch( i )
    { case 0:
        r = brightness; g = t; b = p; break;
      case 1:
        r = q; g = brightness; b = p; break;
      case 2:
        r = p; g = brightness; b = t; break;
      case 3:
        r = p; g = q; b = brightness; break;
      case 4:
        r = t; g = p; b = brightness; break;
      case 5:
      default:
        r = brightness; g = p; b = q; break;
    }
  }

  rgb_colors[0] = r;
  rgb_colors[1] = g;
  rgb_colors[2] = b;
}

void vLedShiftOut(uint8_t *data, uint16_t brightness) 
{
  for(uint8_t i = 0; i < 15; i++)
  {
    shiftOut(DATA, CLK, MSBFIRST, data[i]);
  }  
  digitalWrite(LATCH, HIGH);  
  digitalWrite(LATCH, LOW); 
  analogWrite(ENABLE, brightness); // 10bit PWM 
}