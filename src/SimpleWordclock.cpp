#include <Arduino.h>
#include <WiFiManager.h>
#include <Adafruit_NeoPixel.h>
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

/* -------------------------------------------------------------------------- 
* DEFINES
---------------------------------------------------------------------------- */


/* -------------------------------------------------------------------------- 
Objects/variables
---------------------------------------------------------------------------- */
Adafruit_NeoPixel LED(114, D2, NEO_GRB + NEO_KHZ800);
uint8_t rgb_colors[3] = {0};
uint8_t hue = 0;
uint8_t saturation = 255;
uint8_t brightness = 128;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600, 30000);

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

/* -------------------------------------------------------------------------- 
* FUNCTIONS
---------------------------------------------------------------------------- */
void setup(void)
{
    WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
    LED.begin();
    LED.show(); // Initialize all pixels to 'off'
    Serial.begin(115200);
    
    WiFiManager wm;
    bool res;
    res = wm.autoConnect("Wordclock");

    if(!res) {
    } 
    else {
        //if you get here you have connected to the WiFi    
        timeClient.begin();
        Serial.println("Wifi connected!");
    }
}

void loop(void)
{ 
    // Collect time from NTP
    timeClient.update();

    // Increment hue for color wheel
    hue++;

    if(hue >= 360)
      hue = 0;

    vHSV2RGB();

    int minutes = timeClient.getMinutes();
    int hours = timeClient.getHours();

    //Serial.println(timeClient.getFormattedTime());

    vSetzteLEDs(hours, minutes, rgb_colors[0], rgb_colors[1], rgb_colors[2]);
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
      LED.setPixelColor(i, 0);
}

void vEs(uint8_t r, uint8_t g, uint8_t b)
{
  drawPixel(0, 9, r, g, b);
  drawPixel(1, 9, r, g, b);
}

void vIst(uint8_t r, uint8_t g, uint8_t b)
{
  drawPixel(3, 9, r, g, b);
  drawPixel(4, 9, r, g, b);
  drawPixel(5, 9, r, g, b);
}

void vFuenfMinuten(uint8_t r, uint8_t g, uint8_t b)
{
  drawPixel(7, 9, r, g, b);
  drawPixel(8, 9, r, g, b);
  drawPixel(9, 9, r, g, b);
  drawPixel(10, 9, r, g, b);
}

void vZehnMinuten(uint8_t r, uint8_t g, uint8_t b)
{
  drawPixel(0, 8, r, g, b);
  drawPixel(1, 8, r, g, b);
  drawPixel(2, 8, r, g, b);
  drawPixel(3, 8, r, g, b);
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
}

void vNach(uint8_t r, uint8_t g, uint8_t b)
{
  drawPixel(2, 6, r, g, b);
  drawPixel(3, 6, r, g, b);
  drawPixel(4, 6, r, g, b);
  drawPixel(5, 6, r, g, b);
}

void vVor(uint8_t r, uint8_t g, uint8_t b)
{
  drawPixel(6, 6, r, g, b);
  drawPixel(7, 6, r, g, b);
  drawPixel(8, 6, r, g, b);
}

void vHalb(uint8_t r, uint8_t g, uint8_t b)
{
  drawPixel(0, 5, r, g, b);
  drawPixel(1, 5, r, g, b);
  drawPixel(2, 5, r, g, b);
  drawPixel(3, 5, r, g, b);
}

void vZwoelf(uint8_t r, uint8_t g, uint8_t b)
{
  drawPixel(5, 5, r, g, b);
  drawPixel(6, 5, r, g, b);
  drawPixel(7, 5, r, g, b);
  drawPixel(8, 5, r, g, b);
  drawPixel(9, 5, r, g, b);
}

void vZwei(uint8_t r, uint8_t g, uint8_t b)
{
  drawPixel(0, 4, r, g, b);
  drawPixel(1, 4, r, g, b);
  drawPixel(2, 4, r, g, b);
  drawPixel(3, 4, r, g, b);
}

void vEins(uint8_t r, uint8_t g, uint8_t b)
{
  drawPixel(2, 4, r, g, b);
  drawPixel(3, 4, r, g, b);
  drawPixel(4, 4, r, g, b);
  drawPixel(5, 4, r, g, b);
}

void vEin(uint8_t r, uint8_t g, uint8_t b)
{
  drawPixel(2, 4, r, g, b);
  drawPixel(3, 4, r, g, b);
  drawPixel(4, 4, r, g, b);
}

void vSieben(uint8_t r, uint8_t g, uint8_t b)
{
  drawPixel(5, 4, r, g, b);
  drawPixel(6, 4, r, g, b);
  drawPixel(7, 4, r, g, b);
  drawPixel(8, 4, r, g, b);
  drawPixel(9, 4, r, g, b);
  drawPixel(10, 4, r, g, b);
}

void vDrei(uint8_t r, uint8_t g, uint8_t b)
{
  drawPixel(1, 3, r, g, b);
  drawPixel(2, 3, r, g, b);
  drawPixel(3, 3, r, g, b);
  drawPixel(4, 3, r, g, b);
}

void vFuenf(uint8_t r, uint8_t g, uint8_t b)
{
  drawPixel(7, 3, r, g, b);
  drawPixel(8, 3, r, g, b);
  drawPixel(9, 3, r, g, b);
  drawPixel(10, 3, r, g, b);
}

void vElf(uint8_t r, uint8_t g, uint8_t b)
{
  drawPixel(0, 2, r, g, b);
  drawPixel(1, 2, r, g, b);
  drawPixel(2, 2, r, g, b);
}

void vNeun(uint8_t r, uint8_t g, uint8_t b)
{
  drawPixel(3, 2, r, g, b);
  drawPixel(4, 2, r, g, b);
  drawPixel(5, 2, r, g, b);
  drawPixel(6, 2, r, g, b);
}

void vVier(uint8_t r, uint8_t g, uint8_t b)
{
  drawPixel(7, 2, r, g, b);
  drawPixel(8, 2, r, g, b);
  drawPixel(9, 2, r, g, b);
  drawPixel(10, 2, r, g, b);
}

void vAcht(uint8_t r, uint8_t g, uint8_t b)
{
  drawPixel(1, 1, r, g, b);
  drawPixel(2, 1, r, g, b);
  drawPixel(3, 1, r, g, b);
  drawPixel(4, 1, r, g, b);
}

void vZehn(uint8_t r, uint8_t g, uint8_t b)
{
  drawPixel(5, 1, r, g, b);
  drawPixel(6, 1, r, g, b);
  drawPixel(7, 1, r, g, b);
  drawPixel(8, 1, r, g, b);
}

void vSechs(uint8_t r, uint8_t g, uint8_t b)
{
  drawPixel(1, 0, r, g, b);
  drawPixel(2, 0, r, g, b);
  drawPixel(3, 0, r, g, b);
  drawPixel(4, 0, r, g, b);
  drawPixel(5, 0, r, g, b);
}

void vUhr(uint8_t r, uint8_t g, uint8_t b)
{
  drawPixel(8, 0, r, g, b);
  drawPixel(9, 0, r, g, b);
  drawPixel(10, 0, r, g, b);
}

void vMinute1(uint8_t r, uint8_t g, uint8_t b)
{
  LED.setPixelColor(113, r, g, b);
}

void vMinute2(uint8_t r, uint8_t g, uint8_t b)
{
LED.setPixelColor(112, r, g, b);
}

void vMinute3(uint8_t r, uint8_t g, uint8_t b)
{
LED.setPixelColor(111, r, g, b);
}

void vMinute4(uint8_t r, uint8_t g, uint8_t b)
{
LED.setPixelColor(110, r, g, b);
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
