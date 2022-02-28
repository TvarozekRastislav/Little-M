#include <Adafruit_SSD1306.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include "model.h"

//module.c
Eloquent::ML::Port::SVM SVM_classifier;

//display
#define OLED_RESET -1 
#define SCREEN_ADDRESS 0x3C
#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 32

//ESP comunication
#define rxPinEsp 4
#define txPinESp 2
//line sensors

#define sensor_left 7
#define sensor_middle 8
#define sensor_right 5
//ultrasound sensor

#define trigPin A1
#define echoPin A0

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//comunication with ESP
SoftwareSerial SerialEsp = SoftwareSerial(rxPinEsp, txPinESp);

//waiting for response multitasking
unsigned long esp_read_interval = 1000;
unsigned long time_for_action = 0;

//motor pins
const int IN_1 = 10;
const int IN_2 = 11;
const int IN_3 = 12;
const int IN_4 = 13;
const int EN_A = 3;
const int EN_B = 6;

//beginning mode, default speed and bluetooth variable
int mode_maunal = 0;
char Incoming_value = 0;
int speed = 60;

//room deciding
int first_signal = 0;
int second_signal = 0; 
int third_signal = 0;

//line following sensors
unsigned char state_left;
unsigned char state_middle;
unsigned char state_right;


//room decision
short room = 0;
short prev_predict = 0;

//sending and recieving from ESP
int s_sent = 0;
short counter = 0;

//front sensor
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
}

void ride(uint8_t in_1,uint8_t in_2, uint8_t in_3, uint8_t in_4, int en_A, int en_B ){

  digitalWrite(IN_1, in_1);
  digitalWrite(IN_2, in_2);

  digitalWrite(IN_3, in_3);
  digitalWrite(IN_4, in_4);

  analogWrite(EN_A, en_A);
  analogWrite(EN_B, en_B );
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

void tracking_scan(){
  state_left = digitalRead(sensor_left);
  state_middle = digitalRead(sensor_middle);
  state_right = digitalRead(sensor_right);
}  

void calcDistance(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2; 
  }
  
void setup(){ 
  //display setup
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  display.clearDisplay();
  display.display();
  display.setTextSize(2);            
  display.setTextColor(SSD1306_WHITE);       
  display.setCursor(0,0);             
  display.println(F("ROOM:"));
  display.display();
  
  //com with ESP
  pinMode(rxPinEsp, INPUT);
  pinMode(txPinESp, OUTPUT);
  //sensor setup
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT); 
  //com setup
  Serial.begin(9600); 
  SerialEsp.begin(115200);
  //motor setup
  motor_control_IO_config();
  //line sensors setup
  sensor_IO_config();
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
        Serial.print("idzeeeeeeem j ");
        ride(LOW, HIGH, HIGH, LOW ,speed, speed+10); //forward
      }
      else if (Incoming_value == '2'){
        ride(LOW, HIGH, HIGH, LOW ,0, 120); //right
      }
      else if (Incoming_value == '3'){
        ride(HIGH, LOW, LOW, HIGH ,speed, speed+10); //back 
      }
      else if (Incoming_value == '4'){
        ride(LOW, HIGH, HIGH, LOW ,100, 0); //left    
      }
      else{
      stop();
      }
    }
  read_room();
  }
  
  if (mode_maunal == 0){
    read_room(); 
    tracking_scan();
      
    if (state_middle == HIGH){
      if (state_left == LOW & state_right == HIGH){
        ride(LOW, HIGH, HIGH, LOW ,0, 120); //right
      }
      else if (state_right == LOW & state_left == HIGH){
        ride(LOW, HIGH, HIGH, LOW ,100, 0); //left    
      }
      else{
        ride(LOW, HIGH, HIGH, LOW ,speed, speed+10); //forward
      }
    }
    else{
      if (state_left == LOW & state_right == HIGH){
        ride(LOW, HIGH, HIGH, LOW ,0, 120); //right
      }
      else if (state_right == LOW & state_left == HIGH){
        ride(LOW, HIGH, HIGH, LOW ,100, 0); //left 
      }
      else{
        ride(HIGH, LOW, LOW, HIGH ,speed, speed+10); //back 
        delay(100);
        stop();
      }
    }
  }
  
  do{
    calcDistance();
  if(distance < 10){
    stop();
    }
  }
  while(distance <10);
}
