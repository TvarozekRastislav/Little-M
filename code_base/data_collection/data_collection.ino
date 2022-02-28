#include <ESP8266WiFi.h>

//------------set-up---------------
short num_of_rooms = 3;
short num_of_records = 30;
char wifi_ap_1[] = "rododendron";
char wifi_ap_2[] = "TP-LINK_9FE2A0";
char wifi_ap_3[] = "Router_3";
char delimeter[] = "|";
//---------------------------------

int numNetworks;

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
}

void loop() {
  numNetworks = WiFi.scanNetworks();
  Serial.println();
  for(int x = 0; x < num_of_rooms; x++){
    for(int y = 0; y < num_of_records; y++){
      numNetworks = WiFi.scanNetworks();
      Serial.print(x+1);
      for(int i = 0; i<numNetworks;i++){
        if(WiFi.SSID(i) == wifi_ap_1){
          Serial.print(delimeter);
          Serial.print(WiFi.RSSI(i));
          Serial.print(delimeter);
        }
        else if(WiFi.SSID(i) == wifi_ap_2){
          Serial.print(delimeter);
          Serial.print(WiFi.RSSI(i));
          Serial.print(delimeter);
        }
        else if(WiFi.SSID(i) == wifi_ap_3){
          Serial.print(delimeter);
          Serial.print(WiFi.RSSI(i));
          Serial.print(delimeter);
        }
      }
      Serial.println();
      delay(3000);
    }
    Serial.println("change room");
    delay(7000);
  }
  Serial.println("exit");
}
