int FSR = A0; //the FSR is connected to analog pin 0 (A0)

void setup(){
Serial.begin(9600);
}

void loop(){
int FSRValue = analogRead(FSR);//this takes a reading of the analog value of the FSR

Serial.println(FSRValue);//this gives a printout of the reading of the analog value that is measured
delay(5000); //this slows down the readings so that they're given every 5 seconds
}

