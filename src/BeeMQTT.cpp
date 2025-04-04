#include "BeeMQTT.h"
#include <BeeNeXT.h>

#if defined(BEENEXT_2_4) || defined(BEENEXT_2_4C) || defined(BEENEXT_2_8) || defined(BEENEXT_2_8C) || defined(BEENEXT_3_2) || defined(BEENEXT_3_2C) || defined(BEENEXT_4_3) || defined(BEENEXT_4_3C) || defined(BEENEXT_4_3IPS) || defined(BEENEXT_5_0IPS) || defined(BEENEXT_7_0IPS)
#else

#if BEENEXT_USE_BEEMQTT
#if defined(ESP8266) || defined(ESP32)

void(*_func_mqtt_message_cb)(String topic, String message);

static WiFiClient mqtt_wificlient;
static PubSubClient mqtt(mqtt_wificlient);

static String _mqtt_host = "";
static uint16_t _mqtt_port = 1883;
static String _mqtt_user = "";
static String _mqtt_pass = "";
static String _client_id = "";
static bool   _mqtt_connected =false;

typedef struct _beemqtt_subscribe_topic_t {
  char    topic[512];
  uint8_t qos;
} beemqtt_subscribe_topic_t;

std::vector<beemqtt_subscribe_topic_t> _beemqtt_sub_topics;

void BeeMQTT::client_id(String client_id){
  _client_id= client_id;
}


void BeeMQTT::init(String ssid, String pass, String mqtt_host, uint16_t mqtt_port, String mqtt_user, String mqtt_pass, void(*fn)(String topic, String message))
{
  ::WiFi.begin(ssid, pass);
  BeeMQTT::init(mqtt_host, mqtt_port, mqtt_user, mqtt_pass, fn);
}

void BeeMQTT::init(String mqtt_host, uint16_t mqtt_port, String mqtt_user, String mqtt_pass, void(*fn)(String topic, String message))
{
  _func_mqtt_message_cb = fn; 
  _mqtt_host = mqtt_host; _mqtt_port = mqtt_port;
  _mqtt_user = mqtt_user; _mqtt_pass = mqtt_pass;


#if defined(ESP8266)
  ::WiFi.onEvent([](WiFiEvent_t event){
#else  // ESP32
  ::WiFi.onEvent([](arduino_event_t* ev){
#endif
    Serial.printf("[WiFi] connected.IP : %s\n", WiFi.localIP().toString().c_str());

		// ตัวจัดการเชื่อมต่อ MQTT หากหลุดจะกลับมาเชื่อมอัตโนมัติ
		static SoftTimer timer_mqtt_connection;
		timer_mqtt_connection.setInterval(5000,[](){
      if(_mqtt_host == "" ) return;
			if(!WiFi.isConnected()) return;
			if(!mqtt.connected()) {
        if(_mqtt_connected) { 
          _mqtt_connected =false; 
          Serial.println("[MQTT] disconnected");
        }
        mqtt.setServer(_mqtt_host.c_str(), _mqtt_port);
        mqtt.setCallback([](char* _tp, uint8_t*_data, unsigned int _data_len){
          String topic = String(_tp);
          String message = String((char*) _data, _data_len);
          if( _func_mqtt_message_cb != NULL) {
            _func_mqtt_message_cb(topic, message);
          }
        });
        delay(100);

#if defined(ESP8266)
        String clientId = (_client_id== "")? "ESP8266Client_"+ String(random(0xffff), HEX) : _client_id;
#else // ESP32
        String clientId = (_client_id== "")? "ESP32Client_"+ String(random(0xffff), HEX) : _client_id;
#endif
        // Attempt to connect
        if( _mqtt_user != "") {
            Serial.println("[MQTT] connecting... (1)");
          if (mqtt.connect(clientId.c_str(), _mqtt_user.c_str(), _mqtt_pass.c_str())) {
            Serial.println("[MQTT] connected");

          } else {
            Serial.println("[MQTT] can't connect.");
          }
        }else{
          Serial.printf("[MQTT] connecting... %s [%d] by %s\n", _mqtt_host.c_str(), _mqtt_port, clientId.c_str());
          if (mqtt.connect(clientId.c_str())) {
            Serial.println("[MQTT] connected");

          } else {
            Serial.println("[MQTT] can't connect.");
          }
        }

        // subscribe
        if(mqtt.connected() && _mqtt_connected == false) {
          _mqtt_connected = true;
          if( _beemqtt_sub_topics.size() ){
            Serial.println("[MQTT] subscribe...");
            for(int i=0;i< _beemqtt_sub_topics.size(); i++){
              mqtt.subscribe(_beemqtt_sub_topics[i].topic, _beemqtt_sub_topics[i].qos);
            }
          }
        }
			}
		}, true);
		static SoftTimer timer_mqtt_update;
		timer_mqtt_update.setInterval(1,[](){
			if(mqtt.connected()) mqtt.loop();
		});
#if defined(ESP8266)
  }, WIFI_EVENT_STAMODE_GOT_IP);
#else // ESP32
  }, ARDUINO_EVENT_WIFI_STA_GOT_IP);
#endif

	// static SoftTimer timer_temp;
	// timer_temp.setInterval(1000,[](){
	// 	float temp = random(10000)/100.0;
	// 	Serial.printf("[TEMP] %.2f\n",temp);

	// 	// หาก mqtt เชื่อมอยู่ ให้ส่งข้ามไปยังจอ BeeNeXT
	// 	if(mqtt.connected()) {
	// 		mqtt.publish("/topic/temp", String(temp,2).c_str());
	// 	}
	// });
  Serial.println("[LCD] inited.");
}

void BeeMQTT::subscribe(String topic, uint8_t qos){
  beemqtt_subscribe_topic_t new_subscribe_topic;
  snprintf(new_subscribe_topic.topic, sizeof(new_subscribe_topic.topic), "%s", (char*)topic.c_str());
  new_subscribe_topic.qos = qos;
  _beemqtt_sub_topics.push_back( new_subscribe_topic );

  if(mqtt.connected()) {
    mqtt.subscribe(topic.c_str(), qos);
  }
}

bool BeeMQTT::publish(String topic, String message, bool retained){
  if(!WiFi.isConnected()) return false;
  if(!mqtt.connected()) return false;

  if(retained){
    return mqtt.publish(topic.c_str(), message.c_str(), retained);
  }else{
    return mqtt.publish(topic.c_str(), message.c_str());
  }
}

#endif //#if defined(ESP8266) || #defined(ESP32)
#endif //#if BEENEXT_USE_BEEMQTT

#endif