#include <math.h>
#include <Servo.h>

Servo MyServo[3];

float Zdoel = 150;
float Xdoel = 0;
float Ydoel = 0 ;
float Ltop = 20;

float afstands1;
float afstands2;
float afstands3;
float hoek;
double T;
double T1;
float i;

float ZPelleboog1 ;
float XPelleboog1 ;
float YPelleboog1 = 0;

float ZPelleboog2 ;
float XPelleboog2 ;
float YPelleboog2 = 0;

float ZPelleboog3 ;
float XPelleboog3 ;
float YPelleboog3 = 0;


float ZPpols1 = Zdoel;
float XPpols1;
float YPpols1 = Ydoel;

float ZPpols2 = Zdoel;
float XPpols2 = Xdoel - cos(1/3*M_PI)*Ltop;
float YPpols2 = Ydoel + sin(1/3*M_PI)*Ltop;

float ZPpols3 = Zdoel;
float XPpols3 = Xdoel - cos(1/3*M_PI)*Ltop;
float YPpols3 = Ydoel - sin(1/3*M_PI)*Ltop;

char afstand1waarde;
float servo1hoek = 0;
float servo2hoek = 0;
float servo3hoek = 0;

double radialen;
//double pi = 3.141;

void hoekberekenen(float Xbepalen);

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  MyServo[0].attach(5);
  MyServo[1].attach(6);
  MyServo[2].attach(7);
  hoekberekenen(Xdoel);
  MyServo[0].write(servo1hoek);
  MyServo[1].write(servo2hoek);
  MyServo[2].write(servo3hoek);
}

void loop() {


}


void hoekberekenen(float Xbepalen)
{
  servo1hoek = 0;
  servo2hoek = 0;
  servo3hoek = 0;
  
  for (hoek = 0; hoek<180; hoek = hoek + 0.1)
  {
    T = (float)hoek/(float)360;
    T1 = (float)T * (float)2;

    radialen = (float)T1 * (float)M_PI;

    ZPelleboog1 = 60 * (sin(radialen));
    XPelleboog1 = 100 + 60 * cos(radialen);
    XPpols1 = Xbepalen + Ltop;

    ZPelleboog2 = 60 * (sin(radialen));
    XPelleboog2 = 100 - sin(1/3*M_PI)*60*cos(radialen);
    YPelleboog2 = sin(1/3*M_PI)*60*cos(radialen);

    ZPelleboog3 = 60 * (sin(radialen));
    XPelleboog3 = 100 - cos(1/3*M_PI)*60*cos(radialen);
    YPelleboog3 = 0 - sin(1/3*M_PI)*60*cos(radialen);

    afstands1 = sqrt(sq((ZPelleboog1-ZPpols1))+sq((XPelleboog1-XPpols1))+sq((YPelleboog1-YPpols1)));
    afstands2 = sqrt(sq((ZPelleboog2-ZPpols2))+sq((XPelleboog2-XPpols2))+sq((YPelleboog2-YPpols2)));
    afstands3 = sqrt(sq((ZPelleboog3-ZPpols3))+sq((XPelleboog3-XPpols3))+sq((YPelleboog3-YPpols3)));

    if (afstands1>199.9 && afstands1<200.1)
      {
        if(servo1hoek == 0)
        {
        Serial.print("   Hoek = ");
        Serial.print(hoek);
        Serial.print("   Afstands1 = ");
        Serial.println(afstands1);
        servo1hoek = hoek;
        }
        
      } 

    if (afstands2>199.9 && afstands2<200.1)
      {
        if(servo2hoek == 0)
        {
        Serial.print("   Hoek = ");
        Serial.print(hoek);
        Serial.print("   Afstands2 = ");
        Serial.println(afstands2);
        servo2hoek = hoek;
        }
      } 
    if (afstands3>199.9 && afstands3<200.1)
      {
        if(servo3hoek == 0)
        {
        Serial.print("   Hoek = ");
        Serial.print(hoek);
        Serial.print("   Afstands3 = ");
        Serial.println(afstands3);
        servo3hoek = hoek;
        }
      } 
  }
}
