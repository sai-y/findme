#include <SoftwareSerial.h>

#define PRESSURE 0
#define BUTTON 1
#define BUTTON_PIN 2
#define TEMPERATURE 3
#define PRESSURE_SENSE A0
#define TEMPERATURE_SENSE A1
#define HEART_BEAT 300000
#define RX 3
#define TX 2

SoftwareSerial sigfox(RX, TX); // ARDUINO RX, TX
long lastTimeMillis = 0;
void setup() {
  // SETUP OF THE SOFTWARESERIAL
  pinMode(RX, INPUT);
  pinMode(TX, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);
  sigfox.begin(9600);
  Serial.begin(9600);
  
}

void loop() {

  float temp_value = analogRead(TEMPERATURE_SENSE) * 5.0;
  temp_value /= 1024.0;
  
  byte temperature = (temp_value - 0.5) * 100;
  
  if(temperature > 50){
    byte sensorMask =0;
    sensorMask |= (1<<TEMPERATURE);
    Serial.println(temperature);
    SIGFOX_SEND(sensorMask, temperature);
  }
  // send every 5 mins
  if (millis() - lastTimeMillis >= HEART_BEAT) {
    byte sensorMask =0, data=0;
    sensorMask |= (1<<TEMPERATURE);
    
    SIGFOX_SEND(sensorMask, temperature);
    //peopleCounter = 0;
    lastTimeMillis = millis();
  }
}
  


String SIGFOX_SEND(byte sensorMask, byte data){
  char* buf_str = (char*) malloc (10 + 1);
  String protocol="AT$SF=";
  
  sprintf(buf_str, "%02X\0", sensorMask);
  protocol += String(buf_str);
  
  sprintf(buf_str, "%02X\0", 0);
  protocol += String(buf_str);

  sprintf(buf_str, "%02X\0", data);
  protocol += String(buf_str);

  sprintf(buf_str, "%02X\0", 0);
  protocol += String(buf_str);
  protocol +="\r";
  sigfox.print(String(protocol));
  delay(1000);
  return "OK";
}
