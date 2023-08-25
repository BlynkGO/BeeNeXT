#ifndef __BEENEXT_CONFIG_H__
#define __BEENEXT_CONFIG_H__

/**********************************************
 * BeeNeXT config
 *********************************************/
#define BEENEXT_USE_SOFTWARESERIAL      1
#define BEENEXT_USE_SOFTTIMER           1
#define BEENEXT_USE_HEARTBEAT           1   // ทำงานได้ต้องเปิด BEENEXT_USE_SOFTTIMER เป็น 1 ด้วย
#define BEENEXT_USE_DEBUG               0


#if defined(__STM32F0__) || defined(__STM32F1__) || defined(__STM32F2__) || defined(__STM32F3__) || defined(__STM32F4__) || defined(__STM32F7__) || defined(__STM32H7__)
  #undef BEENEXT_USE_SOFTWARESERIAL
  #define BEENEXT_USE_SOFTWARESERIAL    0
#endif

#ifdef BEENEXT_USE_DEBUG
#define DEBUG_PRINT(x)      Serial.print(x)
#define DEBUG_PRINTLN(x)    Serial.println(x)
#else
#define DEBUG_PRINT(x)
#define DEBUG_PRINTLN(x)
#define DEBUG_PRINTF(...)
#endif


#endif // __BEENEXT_CONFIG_H__

