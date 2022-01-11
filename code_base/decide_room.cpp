#include <Arduino.h>
#include "model.h"
#include <ESP8266WiFi.h>

Eloquent::ML::Port::SVM SVM_classifier;

void setup()
{
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
}

void loop()
{
  int num1 = 0;
  int num2 = 0;
  int num3 = 0;
  int numNetworks = WiFi.scanNetworks();
  for (int i = 0; i < numNetworks; i++)
  {
    if (WiFi.SSID(i) == "rododendron")
    {
      num1 = WiFi.RSSI(i);
    }
    else if (WiFi.SSID(i) == "TP-LINK_9FE2A0")
    {
      num2 = WiFi.RSSI(i);
    }
    else if (WiFi.SSID(i) == "Lidcombe")
    {
      num3 = WiFi.RSSI(i);
    }
  }

  Serial.println(num1);
  Serial.println(num2);
  
  Serial.println(num3);
  float features[] = {num1, num2, num3};
  String output_str = SVM_classifier.predictLabel(features);
  Serial.println(output_str);
  delay(5000);
}

