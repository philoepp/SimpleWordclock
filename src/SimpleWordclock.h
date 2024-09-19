#ifndef SIMPLEWORDCLOCK_H
#define SIMPLEWORDCLOCK_H

#include "Arduino.h"

typedef struct LED_Struct  
{
  uint8_t Led113: 1;
  uint8_t Led114: 1;
  uint8_t Reserve: 6;

  uint8_t Led105: 1;
  uint8_t Led106: 1;
  uint8_t Led107: 1;
  uint8_t Led108: 1;
  uint8_t Led109: 1;
  uint8_t Led110: 1;
  uint8_t Led111: 1;
  uint8_t Led112: 1;

  uint8_t Led97: 1;
  uint8_t Led98: 1;
  uint8_t Led99: 1;
  uint8_t Led100: 1;
  uint8_t Led101: 1;
  uint8_t Led102: 1;
  uint8_t Led103: 1;
  uint8_t Led104: 1;

  uint8_t Led89: 1;
  uint8_t Led90: 1;
  uint8_t Led91: 1;
  uint8_t Led92: 1;
  uint8_t Led93: 1;
  uint8_t Led94: 1;
  uint8_t Led95: 1;
  uint8_t Led96: 1;

  uint8_t Led81: 1;
  uint8_t Led82: 1;
  uint8_t Led83: 1;
  uint8_t Led84: 1;
  uint8_t Led85: 1;
  uint8_t Led86: 1;
  uint8_t Led87: 1;
  uint8_t Led88: 1;

  uint8_t Led73: 1;
  uint8_t Led74: 1;
  uint8_t Led75: 1;
  uint8_t Led76: 1;
  uint8_t Led77: 1;
  uint8_t Led78: 1;
  uint8_t Led79: 1;
  uint8_t Led80: 1;

  uint8_t Led65: 1;
  uint8_t Led66: 1;
  uint8_t Led67: 1;
  uint8_t Led68: 1;
  uint8_t Led69: 1;
  uint8_t Led70: 1;
  uint8_t Led71: 1;
  uint8_t Led72: 1;

  uint8_t Led57: 1;
  uint8_t Led58: 1;
  uint8_t Led59: 1;
  uint8_t Led60: 1;
  uint8_t Led61: 1;
  uint8_t Led62: 1;
  uint8_t Led63: 1;
  uint8_t Led64: 1;

  uint8_t Led49: 1;
  uint8_t Led50: 1;
  uint8_t Led51: 1;
  uint8_t Led52: 1;
  uint8_t Led53: 1;
  uint8_t Led54: 1;
  uint8_t Led55: 1;
  uint8_t Led56: 1;

  uint8_t Led41: 1;
  uint8_t Led42: 1;
  uint8_t Led43: 1;
  uint8_t Led44: 1;
  uint8_t Led45: 1;
  uint8_t Led46: 1;
  uint8_t Led47: 1;
  uint8_t Led48: 1;

  uint8_t Led33: 1;
  uint8_t Led34: 1;
  uint8_t Led35: 1;
  uint8_t Led36: 1;
  uint8_t Led37: 1;
  uint8_t Led38: 1;
  uint8_t Led39: 1;
  uint8_t Led40: 1;

  uint8_t Led25: 1;
  uint8_t Led26: 1;
  uint8_t Led27: 1;
  uint8_t Led28: 1;
  uint8_t Led29: 1;
  uint8_t Led30: 1;
  uint8_t Led31: 1;
  uint8_t Led32: 1;

  uint8_t Led17: 1;
  uint8_t Led18: 1;
  uint8_t Led19: 1;
  uint8_t Led20: 1;
  uint8_t Led21: 1;
  uint8_t Led22: 1;
  uint8_t Led23: 1;
  uint8_t Led24: 1;

  uint8_t Led9: 1;
  uint8_t Led10: 1;
  uint8_t Led11: 1;
  uint8_t Led12: 1;
  uint8_t Led13: 1;
  uint8_t Led14: 1;
  uint8_t Led15: 1;
  uint8_t Led16: 1;

  uint8_t Led1: 1;
  uint8_t Led2: 1;
  uint8_t Led3: 1;
  uint8_t Led4: 1;     
  uint8_t Led5: 1;
  uint8_t Led6: 1;
  uint8_t Led7: 1;     
  uint8_t Led8: 1;
} LED_Micro;

#endif // SIMPLEWORDCLOCK_H