#include <SoftwareSerial.h>
#include "PulsePattern.h"

#include <Process.h>
#define PRESSURE 0
#define BUTTON 1
#define BUTTON_PIN 2
#define TEMPERATURE 3
#define PRESSURE_SENSE A0
#define TEMPERATURE_SENSE A1
#define HEART_BEAT 60000
#define RX 8
#define TX 7
SoftwareSerial sigfox(RX, TX); // ARDUINO RX, TX
long lastTimeMillis = 0;

uint16_t SOSpattern[] = {  
  500,500,500,500,500,1500,  // SOS in morse
  1500,500,1500,500,1500,1500, 
  500,500,500,500,500,1500 };

uint16_t XYZpattern[] = {  
  100,100,100,100,100,100,  // SOS in morse
  500,500,500,500,500,500, 
  1000,1000,1000,1000,1000,1000 };

uint8_t patternSize = 18;
uint8_t startLevel = LOW;

void setup() {
  // SETUP OF THE SOFTWARESERIAL
  pinMode(RX, INPUT);
  pinMode(TX, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);
  sigfox.begin(9600);
  Serial.begin(9600);
  Bridge.begin();
}

void loop() {

  
  if(!digitalRead(BUTTON_PIN)){
    byte flags, data = 0;
    flags |= (1<<BUTTON);
    data |= (1<<BUTTON);
    
    SIGFOX_SEND(flags, data,0);
    Process p;
    p.begin("aplay");
    p.addParameter("/root/sos_signal.wav");
    p.run();

    
    p.begin("aplay");
    p.addParameter("/root/Alert.wav");
    p.run();
  //WAIT 5s BEFORE SENDING A MESSAGE AGAIN
    delay(1000);
    PPGenerator.init(13, SOSpattern, patternSize, startLevel, PRESCALE_1024);
    PPGenerator.start();
    
  }
  if(analogRead(PRESSURE_SENSE)>900){
    byte flags,data =0;
    flags |= (1<<PRESSURE);
    data |= (1<<PRESSURE);
    SIGFOX_SEND(flags, data, 0);
  //WAIT 5s BEFORE SENDING A MESSAGE AGAIN
    delay(3000);
  }
  float temp_value =0;
  for(int i=0; i<1024;i++){
     temp_value = analogRead(TEMPERATURE_SENSE) * 5.0;
  }
  
  temp_value /= 1024.0;
  
  byte temperature = (temp_value - 0.5) * 100;
  if(temperature>100){
    temperature =0;
  }
  Serial.println(temperature);
  if(temperature > 50){
    byte sensorMask =0;
    sensorMask |= (1<<TEMPERATURE);
    Serial.println(temperature);
    SIGFOX_SEND(sensorMask,0, temperature);
  }
  // send every 5 mins
  if (millis() - lastTimeMillis >= HEART_BEAT) {
    byte sensorMask =0, data=0;
    sensorMask |= (1<<BUTTON) | (1<<PRESSURE);
    if(!digitalRead(BUTTON_PIN)){
      data |= (1<<BUTTON);
    }
    if(analogRead(PRESSURE_SENSE>900)){
      data |= (1<<PRESSURE);
    }
    SIGFOX_SEND(sensorMask, data, temperature);
    //peopleCounter = 0;
    lastTimeMillis = millis();
  }
  
}
  


String SIGFOX_SEND(byte sensorMask, byte data, byte temperature){
  char* buf_str = (char*) malloc (10 + 1);
  String protocol="AT$SF=";
  
  sprintf(buf_str, "%02X\0", sensorMask);
  protocol += String(buf_str);

  sprintf(buf_str, "%02X\0", data);
  protocol += String(buf_str);

  sprintf(buf_str, "%02X\0", temperature);
  protocol += String(buf_str);
  
  sprintf(buf_str, "%02X\0", 0);
  protocol += String(buf_str);
  protocol +="\r";
  sigfox.print(String(protocol));
  delay(1000);
  return "OK";
}


