#include <math.h>

float Zdoel = 180;
float Xdoel = 10;
float Ydoel = 15;
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

double radialen;
//double pi = 3.141;

void setup() {
  // put your setup code here, to run once:

Serial.begin(9600);

for (hoek = 0; hoek<181; hoek = hoek + 0.01)
{
T = (float)hoek/(float)360;
//Serial.print(T);
T1 = (float)T * (float)2;

radialen = (float)T1 * (float)M_PI;

ZPelleboog1 = 60 * (sin(radialen));
XPelleboog1 = 100 + 60 * cos(radialen);
XPpols1 = Xdoel + Ltop;

ZPelleboog2 = 60 * (sin(radialen));
  XPelleboog2 = 100 - sin(1/3*M_PI)*60*cos(radialen);
YPelleboog2 = sin(1/3*M_PI)*60*cos(radialen);

ZPelleboog3 = 60 * (sin(radialen));
XPelleboog3 = 100 - cos(1/3*M_PI)*60*cos(radialen);
YPelleboog3 = 0 - sin(1/3*M_PI)*60*cos(radialen);

afstands1 = sqrt(sq((ZPelleboog1-ZPpols1))+sq((XPelleboog1-XPpols1))+sq((YPelleboog1-YPpols1)));
afstands2 = sqrt(sq((ZPelleboog2-ZPpols2))+sq((XPelleboog2-XPpols2))+sq((YPelleboog2-YPpols2)));
afstands3 = sqrt(sq((ZPelleboog3-ZPpols3))+sq((XPelleboog3-XPpols3))+sq((YPelleboog3-YPpols3)));


if (afstands1 > 199.99 && afstands1 < 200.01){
  Serial.print("   Hoek = ");
  Serial.print(hoek);
  Serial.print("   Afstands1 = ");
  Serial.println(afstands1);
  servo1hoek = hoek;
}

//for (i = 200; i <201; i=i+0.01){
//if (afstands2 == i){

/*if (afstands2 > 199.99 && afstands2 < 200.01){
  //Serial.print("   i = ");
  //Serial.print(i);
  Serial.print("   Hoek = ");
  Serial.print(hoek);
  Serial.print("   Afstands2 = ");
  Serial.println(afstands2);
  }


if (afstands3 > 199.99 && afstands3 < 200.01){
  Serial.print("   Hoek = ");
  Serial.print(hoek);
  Serial.print("   Afstands3 = ");
    Serial.println(afstands3);
    }
    */
    
  }

Serial.println(servo1hoek);
  
}
//}

void loop() {
  // put your main code here, to run repeatedly:

}


void hoekberekenen(float Xbepalen,float Ybepalen,float Zbepalen)
{
   




  
}
