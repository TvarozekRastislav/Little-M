#include <ESP8266WiFi.h>

//------------set-up---------------
char wifi_ap_1[] = "rododendron";
char wifi_ap_2[] = "TP-LINK_9FE2A0";
char wifi_ap_3[] = "Lidcombe";
//---------------------------------

int numNetworks;

int s1;
int s2;
int s3;

void setup(){
    Serial.begin(115200);
    numNetworks = WiFi.scanNetworks();
       for (int i = 0; i < numNetworks; i++){
        if (WiFi.SSID(i) == wifi_ap_1){   
          s1 = WiFi.RSSI(i);
        }
        if (WiFi.SSID(i) == wifi_ap_2){
          s2 = WiFi.RSSI(i);
        }
        if (WiFi.SSID(i) == wifi_ap_3){
          s3 = WiFi.RSSI(i);
      }
    }  
}

void loop(){
   if(Serial.available()){
    if(Serial.read()== 's'){
      Serial.write(s1*(-1));
      Serial.write(s2*(-1));
      Serial.write(s3*(-1));
    }
  }
      numNetworks = WiFi.scanNetworks();
       for (int i = 0; i < numNetworks; i++){
        if (WiFi.SSID(i) == wifi_ap_1){   
          s1 = WiFi.RSSI(i);
        }
        if (WiFi.SSID(i) == wifi_ap_2){
          s2 = WiFi.RSSI(i);
        }
        if (WiFi.SSID(i) == wifi_ap_3){
          s3 = WiFi.RSSI(i);
      }
    }  
 
}
