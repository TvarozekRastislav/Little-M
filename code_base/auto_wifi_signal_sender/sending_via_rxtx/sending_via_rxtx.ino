#include <ESP8266WiFi.h>
int numNetworks;
void setup()
{
    Serial.begin(115200);
}

void loop()
{
    numNetworks = WiFi.scanNetworks();
    int a = 0; 
    for (int i = 0; i < numNetworks; i++)
    {
        a = 0; 
        if (WiFi.SSID(i) == "rododendron")
        {   
            Serial.println("");
            int a = WiFi.RSSI(i);
            Serial.println(a);
            a = a*-1;
            Serial.println(a);
            Serial.println("");
            Serial.write(a);
            Serial.write('e');
            Serial.println("");
        }
        else if (WiFi.SSID(i) == "TP-LINK_9FE2A0")
        {
          int a = WiFi.RSSI(i);
           Serial.println(a);
            a = a*-1;
          Serial.println(a);
          Serial.println("");
          Serial.write(a);
          Serial.write('e');
          Serial.println("");
        }
        else if (WiFi.SSID(i) == "Lidcombe")
        {
           int a = WiFi.RSSI(i);
            Serial.println(a);
            a = a*-1;
           Serial.println(a);
           Serial.println("");
           Serial.write(a);
           Serial.write('e');
           Serial.println("");
        }
    }
    Serial.println("odoslane");
    delay(100);
}
