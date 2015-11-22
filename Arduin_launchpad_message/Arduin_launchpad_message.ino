#include <SoftwareSerial.h>

#define PRESSURE 0
#define BUTTON 1
#define BUTTON_PIN 2
#define PRESSURE_SENSE A0
#define HEART_BEAT 300000

SoftwareSerial sigfox(8, 7); // ARDUINO RX, TX
long lastTimeMillis = 0;
void setup() {
  // SETUP OF THE SOFTWARESERIAL
  pinMode(8, INPUT);
  pinMode(7, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);
  sigfox.begin(9600);
  Serial.begin(9600);
  
}

void loop() {

  if(!digitalRead(BUTTON_PIN)){
    byte flags, data = 0;
    flags |= (1<<BUTTON);
    data |= (1<<BUTTON);

    SIGFOX_SEND(flags, data);
  //WAIT 5s BEFORE SENDING A MESSAGE AGAIN
    delay(1000);
  }
  if(analogRead(PRESSURE_SENSE)>900){
    byte flags,data =0;
    flags |= (1<<PRESSURE);
    data |= (1<<PRESSURE);
    SIGFOX_SEND(flags, data);
  //WAIT 5s BEFORE SENDING A MESSAGE AGAIN
    delay(1000);
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
    SIGFOX_SEND(sensorMask, data);
    //peopleCounter = 0;
    lastTimeMillis = millis();
  }
}
  


String SIGFOX_SEND(byte sensorMask, byte data){
  char* buf_str = (char*) malloc (10 + 1);
  String protocol="AT$SF=";
  
  sprintf(buf_str, "%02X\0", sensorMask);
  protocol += String(buf_str);

  sprintf(buf_str, "%02X\0", data);
  protocol += String(buf_str);
  
  sprintf(buf_str, "%02X\0", 0);
  protocol += String(buf_str);
  protocol += String(buf_str);
  protocol +="\r";
  sigfox.print(String(protocol));
  delay(1000);
  return "OK";
}
