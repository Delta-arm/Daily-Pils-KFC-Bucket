/*
  Example for receiving
  
  https://github.com/sui77/rc-switch/
  
  If you want to visualize a telegram copy the raw data and 
  paste it into http://test.sui.li/oszi/
*/
#include  <SPI.h>
#include <RCSwitch.h>
#define Tril_Motor 5 
int Tijd_Trillingen = 0;
int Tijd=0;
int Wachttijd=20000;
RCSwitch mySwitch = RCSwitch();

void setup() {
  Serial.begin(9600);
  mySwitch.enableReceive(0);  // Receiver on interrupt 0 => that is pin #2
  pinMode(Tril_Motor,OUTPUT);
}

void loop()
{
Tijd=millis();
  if (mySwitch.available())    // wanneer er iets ontvangen wordt 
  {
    digitalWrite(Tril_Motor,HIGH);
    Tijd_Trillingen=millis();
  }
  if (Tijd-Tijd_Trillingen == Wachttijd)
  {
    digitalWrite(Tril_Motor,LOW);
      
  }
}
