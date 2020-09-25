#include "my_mttq.h"
#include <SPI.h>
//#include <WiFi.h>

void send_msg(PubSubClient& client, String topic, String mess){
	if (!client.connected()) {
		reconnect(client);
	}
	client.loop();
	client.publish(topic.c_str(), mess.c_str());
}

bool reconnect(PubSubClient& client) {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP32";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      client.subscribe("publish/id");
      client.subscribe("publish/enroll");
      client.subscribe("publish/delete");
      client.subscribe("publish/door");
      client.subscribe("publish/reng");
      client.subscribe("publish/startStreaming");
      client.subscribe("publish/stopCapturing");
      client.subscribe("publish/stopStreaming");
      return true;
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      return false;
    }
  }
  return true;
}

void subscribe_topic(PubSubClient& client, char *topic){
  while(!reconnect(client)){}
  client.subscribe(topic);
}
