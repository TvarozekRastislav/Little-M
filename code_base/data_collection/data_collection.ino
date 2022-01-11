#include <ESP8266WiFi.h>

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
}

void loop() {
  int numNetworks = WiFi.scanNetworks();
  Serial.println();
  for(int x = 0; x < 3 ; x++){
    for(int y = 0; y < 10; y++){
      numNetworks = WiFi.scanNetworks();
      Serial.print(x+1);
      for(int i = 0; i<numNetworks;i++){
        if(WiFi.SSID(i) == "rododendron"){
          Serial.print("|");
        //Serial.println(WiFi.SSID(i));
          Serial.print(WiFi.RSSI(i));
          Serial.print("|");
        }
        else if(WiFi.SSID(i) == "TP-LINK_9FE2A0"){
        //Serial.println(WiFi.SSID(i));
          Serial.print("|");
          Serial.print(WiFi.RSSI(i));
          Serial.print("|");
        }
        else if(WiFi.SSID(i) == "Lidcombe"){
        //Serial.println(WiFi.SSID(i));
          Serial.print("|");
          Serial.print(WiFi.RSSI(i));
          Serial.print("|");
        }
      }
      Serial.println();
      delay(4000);
    }
    delay(7000);
  }
  Serial.println("exit");
}
