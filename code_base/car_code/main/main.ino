#include <Adafruit_SSD1306.h>

#include <SoftwareSerial.h>
#include <Wire.h>

#include "model.h"

Eloquent::ML::Port::SVM SVM_classifier;

#define OLED_RESET -1 
#define SCREEN_ADDRESS 0x3C
#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define rxPinEsp 4
#define txPinESp 2

#define trigPin A1
#define echoPin A0

#define sensor_left 7
#define sensor_middle 8
#define sensor_right 5

SoftwareSerial SerialEsp = SoftwareSerial(rxPinEsp, txPinESp);

unsigned long esp_read_interval = 1000;
unsigned long time_for_action = 0;

unsigned char state_left;
unsigned char state_middle;
unsigned char state_right;

const int IN_1 = 10;
const int IN_2 = 11;
const int IN_3 = 12;
const int IN_4 = 13;
const int EN_A = 3;
const int EN_B = 6;
int car_mode = 0;

int mode_maunal = 0;
char Incoming_value = 0;
int speed = 60;

int first_signal = 0;
int second_signal = 0; 
int third_signal = 0;

short room = 0;
short prev_predict = 0;

int s_sent = 0;
short counter = 0;

long duration; 
long distance;

void motor_control_IO_config(){
  pinMode(IN_1, OUTPUT);
  pinMode(IN_2, OUTPUT);
  pinMode(IN_3, OUTPUT);
  pinMode(IN_4, OUTPUT);
  pinMode(EN_A, OUTPUT);
  pinMode(EN_B, OUTPUT);
}

void stop(){
  analogWrite(EN_A, 0);
  analogWrite(EN_B, 0);
  car_mode = 0;
}

void forward(){
  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, HIGH);

  digitalWrite(IN_3, HIGH);
  digitalWrite(IN_4, LOW);

  analogWrite(EN_A, speed);
  analogWrite(EN_B, speed + 10);

  car_mode = 1;
}

void right(){
  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, HIGH);

  digitalWrite(IN_3, HIGH);
  digitalWrite(IN_4, LOW);

  analogWrite(EN_A, 0);
  analogWrite(EN_B, 120);

  car_mode = 2;
}

void back(){
  digitalWrite(IN_1, HIGH);
  digitalWrite(IN_2, LOW);

  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, HIGH);

  analogWrite(EN_A, speed);
  analogWrite(EN_B, speed + 10);
  
  car_mode = 3;
}

void left(){
  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, HIGH);

  digitalWrite(IN_3, HIGH);
  digitalWrite(IN_4, LOW);

  analogWrite(EN_A, 100);
  analogWrite(EN_B, 0);

  car_mode = 4;
}

void sensor_IO_config(){
  pinMode(sensor_left, INPUT);
  pinMode(sensor_right, INPUT);
  pinMode(sensor_middle, INPUT);
}

void read_room(){
  
  if(s_sent == 0){
    SerialEsp.write('s');
    s_sent = 1;
  }

  if(millis() > time_for_action){
      time_for_action = millis() + esp_read_interval;
      Serial.println(SerialEsp.available());
      if(SerialEsp.available()==3){
        s_sent = 0;
        Serial.println("prisli bajty");
        Serial.println(SerialEsp.available());
        first_signal = SerialEsp.read()* (-1);
        second_signal = SerialEsp.read()* (-1);
        third_signal = SerialEsp.read() * (-1);
        
        Serial.println(first_signal);
        Serial.println(second_signal);
        Serial.println(third_signal);

        counter = 0;
        get_room();
      }
      else{
        Serial.println("prislo menej bajtov!");
        counter++;
        while(SerialEsp.available() > 0){
         char t = SerialEsp.read();
      }
      if(counter == 5 ){
        SerialEsp.write('s');
        counter= 0; 
        Serial.write("poslal som s");
      }
      first_signal = 0;
      second_signal = 0;
      third_signal = 0;
      return;
      }
   }
}

void get_room(){
  
  if(first_signal == 0 || second_signal == 0 || third_signal == 0){
    Serial.println("prislo malo signalov");
    return;
  }
  
  if(first_signal == 1 || second_signal == 1 || third_signal == 1){
    Serial.println("prislo poskodeny signal");
    return;
  }
  
  float features[] = {first_signal, second_signal, third_signal};
  String output_str = SVM_classifier.predictLabel(features);
  int output = output_str.toInt();
  
  Serial.println("------------------");
  Serial.println("konkretna miestnost:");
  Serial.println(output);
  Serial.println("predchadazaujca miestnost:");
  Serial.println(prev_predict);
  
  if(prev_predict == output){
    room = output;
  }
    
  Serial.println("daná miestnost:");
  
  display.clearDisplay();
  display.setCursor(0,0);
  display.setTextSize(2);           
  display.setTextColor(SSD1306_WHITE);
  display.println(("ROOM:"));
  display.println(room);
  display.display();
  
  Serial.println(room);
  Serial.println("------------------");
  prev_predict = output;
}
  
void setup(){ 
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  display.clearDisplay();
  display.display();
  display.setTextSize(2);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
  display.println(F("ROOM:"));
  display.display();
  
  pinMode(rxPinEsp, INPUT);
  pinMode(txPinESp, OUTPUT);
  
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  
  Serial.begin(9600); 
  SerialEsp.begin(115200);
    
  motor_control_IO_config();
  sensor_IO_config();
}

void tracking_scan(){
  state_left = digitalRead(sensor_left);
  state_middle = digitalRead(sensor_middle);
  state_right = digitalRead(sensor_right);
}

void loop(){
  
  if (Serial.available() > 0){
    Incoming_value = Serial.read();
    Serial.print(Incoming_value);  
    Serial.print("\n");
    
    if (Incoming_value == 'c'){
      mode_maunal = 1;
    }
    else if (Incoming_value == 'C'){
      mode_maunal = 0;
    }
  
  if (mode_maunal == 1){
    if (Incoming_value == '1'){
      forward();
    }
    else if (Incoming_value == '2'){
      right();
    }
    else if (Incoming_value == '3'){
      back();
    }
    else if (Incoming_value == '4'){
      left();
    }
    else if (Incoming_value == '5'){
      digitalWrite(IN_1, LOW);
      digitalWrite(IN_2, HIGH);

      digitalWrite(IN_3, HIGH);
      digitalWrite(IN_4, LOW);

      analogWrite(EN_A, 40);
      analogWrite(EN_B, 100);
    }
    else if (Incoming_value == '6'){//doprava_dole

      digitalWrite(IN_1, HIGH);
      digitalWrite(IN_2, LOW);

      digitalWrite(IN_3, LOW);
      digitalWrite(IN_4, HIGH);

      analogWrite(EN_A, 40);
      analogWrite(EN_B, 100);
    }
    else if (Incoming_value == '7'){//dolava_dole
      digitalWrite(IN_1, HIGH);
      digitalWrite(IN_2, LOW);

      digitalWrite(IN_3, LOW);
      digitalWrite(IN_4, HIGH);

      analogWrite(EN_A, 100);
      analogWrite(EN_B, 50);
    }
    else if (Incoming_value == '8'){ //dolava hore
      digitalWrite(IN_1, LOW);
      digitalWrite(IN_2, HIGH);

      digitalWrite(IN_3, HIGH);
      digitalWrite(IN_4, LOW);

      analogWrite(EN_A, 100);
      analogWrite(EN_B, 50);
      }
      else{
        stop();
      }
    }
    read_room();
  }
    if (mode_maunal == 5){
      read_room(); 
      tracking_scan();
      
      if (state_middle == HIGH){
        if (state_left == LOW & state_right == HIGH){
          right();
      }
        else if (state_right == LOW & state_left == HIGH){
          left();
        }
        else{
          forward();
        }
      }
      else{
        if (state_left == LOW & state_right == HIGH){
          right();
        }
        else if (state_right == LOW & state_left == HIGH){
           left();
        }
        else{
          back();
          delay(100);
          stop();
         }
      }
   }
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  // Displays the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  if(distance < 15){
    stop();
   }
  
}
