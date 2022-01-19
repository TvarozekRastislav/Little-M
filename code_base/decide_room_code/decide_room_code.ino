#include <Arduino.h>
#include "model.h"
#include <ESP8266WiFi.h>

//------------set-up---------------
char wifi_ap_1[] = "rododendron";
char wifi_ap_2[] = "TP-LINK_9FE2A0";
char wifi_ap_3[] = "Lidcombe";
//---------------------------------

Eloquent::ML::Port::SVM SVM_classifier;

void setup(){
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
}

void loop(){

  int num1 = 0;
  int num2 = 0;
  int num3 = 0;
  int numNetworks = WiFi.scanNetworks();
  for (int i = 0; i < numNetworks; i++){

    if (WiFi.SSID(i) == wifi_ap_1){
      num1 = WiFi.RSSI(i);
    }
    else if (WiFi.SSID(i) == wifi_ap_2){
      num2 = WiFi.RSSI(i);
    }
    else if (WiFi.SSID(i) == wifi_ap_3){
      num3 = WiFi.RSSI(i);
    }
  }

  Serial.println(num1);
  Serial.println(num2);
  Serial.println(num3);

  if(num1 != 0|| num2 != 0 || num3 != 0){
    float features[] = {num1, num2, num3};
    String output_str = SVM_classifier.predictLabel(features);
    Serial.println("-----------rooom-----");
    Serial.println(output_str);
    Serial.println("-----------rooom-----");
    delay(2000);
  }
}
