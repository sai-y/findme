#include <SoftwareSerial.h>
SoftwareSerial sigfox(8, 7); // ARDUINO RX, TX
void setup() {
  // SETUP OF THE SOFTWARESERIAL
  pinMode(8, INPUT);
  pinMode(7, OUTPUT);
  sigfox.begin(9600);
}

void loop() {
  // data to send should be 12 bytes (or less - puts 0 on the MSB) hexadecimal transformed to a chain of character
  char* data_to_send="66";
  //SEND DATA
  if(analogRead(A0)>900){
    SIGFOX_SEND(data_to_send);
  //WAIT 5s BEFORE SENDING A MESSAGE AGAIN
    delay(1000);
  }
  
}

String SIGFOX_SEND(char* data){
  String protocol="AT$SF=";
  protocol+=(String)data;
  protocol+="\r";
  sigfox.print(String(protocol));
  delay(1000);
  return "OK";
}
