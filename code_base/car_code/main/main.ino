#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
#include "model.h"

Eloquent::ML::Port::SVM SVM_classifier;

#define rxPin 4
#define txPin 2
#define sensor_left 7
#define sensor_middle 8
#define sensor_right 5

SoftwareSerial SerialEsp =  SoftwareSerial(rxPin, txPin);

LiquidCrystal lcd(14, 15, 16, 17, 18, 19);  

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
int speed = 80;

int first_signal = 0;
int second_signal = 0; 
int third_signal = 0;

short room = 0;
short prev_predict = 0;

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
  while(Serial.available() > 0) {
      char t = Serial.read();
  }
  
  SerialEsp.write('s');
  delay(100);
  
  if(SerialEsp.available()){
    Serial.println("prisli 3 bajty");
    
    first_signal = SerialEsp.read()* (-1);
    second_signal = SerialEsp.read()* (-1);
    third_signal = SerialEsp.read() * (-1);
    
    Serial.println(first_signal);
    Serial.println(second_signal);
    Serial.println(third_signal);
    }
  else{
    Serial.println("prislo menej bajtov!");
    
    while(Serial.available() > 0) {
      char t = Serial.read();
      }
      
    first_signal = 0;
    second_signal = 0;
    third_signal = 0;
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
  lcd.setCursor(0, 1);
  lcd.print(room);
  Serial.println(room);
  Serial.println("------------------");
  prev_predict = output;
  
}
  
void setup(){ 
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
    
  Serial.begin(9600); 
  SerialEsp.begin(115200);
    
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(A4, OUTPUT);
  pinMode(A5, OUTPUT);
    
  lcd.begin(8, 2);
  lcd.print("hello");
  motor_control_IO_config();
  sensor_IO_config();
}

void tracking_scan(){
  state_left = digitalRead(sensor_left);
  state_middle = digitalRead(sensor_middle);
  state_right = digitalRead(sensor_right);
}

void loop(){
  delay(500);
  if (Serial.available() > 0){
    Incoming_value = Serial.read(); //Read the incoming data and store it into variable Incoming_value
    Serial.print(Incoming_value);   //Print Value of Incoming_value in Serial monitor
    Serial.print("\n");
    
    if (Incoming_value == 'c'){
      mode_maunal = 1;
    }
    else if (Incoming_value == 'C'){
      mode_maunal = 0;
    }
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
    if (mode_maunal == 0){ 
      read_room();
      tracking_scan();
      get_room(); 
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
}
