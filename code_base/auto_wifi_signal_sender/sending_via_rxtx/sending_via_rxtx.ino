#include <ESP8266WiFi.h>

//------------set-up---------------
char wifi_ap_1[] = "rododendron";
char wifi_ap_2[] = "TP-LINK_9FE2A0";
char wifi_ap_3[] = "Router_3";
//---------------------------------

int numNetworks;

void setup(){
    Serial.begin(115200);
}

void loop(){
 if(Serial.available()){
    if(Serial.read() == 's'){
      numNetworks = WiFi.scanNetworks();
       for (int i = 0; i < numNetworks; i++){
        if (WiFi.SSID(i) == wifi_ap_1){   
          Serial.write(WiFi.RSSI(i)*(-1)); 
        }
        else if (WiFi.SSID(i) == wifi_ap_2){
          Serial.write(WiFi.RSSI(i)*(-1));
        }
        else if (WiFi.SSID(i) == wifi_ap_3){
          Serial.write(WiFi.RSSI(i)*(-1));
        }
      }
    }  
  }
}
