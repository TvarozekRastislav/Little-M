#include <ESP8266WiFi.h>
int numNetworks;
void setup()
{
    Serial.begin(115200);
}

void loop(){
 if(Serial.available()){
    if(Serial.read() == 's'){
      numNetworks = WiFi.scanNetworks();
       for (int i = 0; i < numNetworks; i++){
        if (WiFi.SSID(i) == "rododendron"){   
          Serial.write(WiFi.RSSI(i)*(-1)); 
        }
        if (WiFi.SSID(i) == "TP-LINK_9FE2A0"){
          Serial.write(WiFi.RSSI(i)*(-1));
        }
        if (WiFi.SSID(i) == "Lidcombe"){
          Serial.write(WiFi.RSSI(i)*(-1));
        }
      }
    }  
  }
}
