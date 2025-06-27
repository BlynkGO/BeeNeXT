#ifndef __BEEMQTT_H__
#define __BEEMQTT_H__

#include "beenext_config.h"

#if defined(BEENEXT_2_4) || defined(BEENEXT_2_4C) || defined(BEENEXT_2_8) || defined(BEENEXT_2_8C) || defined(BEENEXT_3_2) || defined(BEENEXT_3_2C) || defined(BEENEXT_3_5) || defined(BEENEXT_3_5C) || defined(BEENEXT_4_3) || defined(BEENEXT_4_3C) || defined(BEENEXT_4_3IPS) || defined(BEENEXT_5_0IPS) || defined(BEENEXT_7_0IPS)
#else

#if BEENEXT_USE_BEEMQTT
#if defined(ESP8266) || defined(ESP32)

#if defined(ESP8266)
  #include <ESP8266WiFi.h>
#elif defined(ESP32)
  #include <WiFi.h>
#endif

#include <PubSubClient.h>
#include <WiFiClient.h>
#include <vector>

namespace BeeMQTT {
  void client_id(String client_id);
  void init(String ssid, String pass, String mqtt_host, uint16_t mqtt_port, String mqtt_user, String mqtt_pass, void(*fn)(String topic, String message));
  inline void init(String ssid, String pass, String mqtt_host, uint16_t mqtt_port, void(*fn)(String topic, String message))
  {
    BeeMQTT::init(ssid, pass, mqtt_host, mqtt_port, "", "", fn);
  }

  void init(String mqtt_host, uint16_t mqtt_port, String mqtt_user, String mqtt_pass, void(*fn)(String topic, String message));
  inline void init(String mqtt_host, uint16_t mqtt_port, void(*fn)(String topic, String message))
  {
    BeeMQTT::init(mqtt_host, mqtt_port, "", "", fn);
  }

  void subscribe(String topic, uint8_t qos =0);
  bool publish(String topic, String message, bool retained=false);
}


#endif //#if defined(ESP8266) || #defined(ESP32)
#endif //#if BEENEXT_USE_BEEMQTT

#endif

#endif //__BEEMQTT_H__

