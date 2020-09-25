#include "wifi.h"

//gets called when WiFiManager enters configuration mode
void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  //if you used auto generated SSID, print it
  Serial.println(".....................");
  Serial.println(myWiFiManager->getConfigPortalSSID());
}
  
bool _connect_to_wifi(String& ssid, String& password){
//bool _connect_to_wifi(){
//  WiFi.begin(ssid.c_str(), password.c_str());
//  Serial.print("Connecting to WiFi...");
//  while (WiFi.status() != WL_CONNECTED) {
//    delay(500);
//    Serial.print(".");
//  }
//  Serial.println();
//  
//  Serial.print("IP Address: http://");
//  Serial.println(WiFi.localIP());
//  return true;
  WiFi.mode(WIFI_STA);
  WiFiManager wm;
  wm.setAPCallback(configModeCallback);
  if (!wm.autoConnect(ssid.c_str(), password.c_str())) {
    Serial.println("failed to connect and hit timeout");
    return false;
  }
//  ssid = wm.getWiFiSSID(true);
//  password = wm.getWiFiPass(true);
  return true;
}
