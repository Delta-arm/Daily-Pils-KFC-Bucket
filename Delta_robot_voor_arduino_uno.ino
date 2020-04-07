
#include "RTClib.h"
#include <RCSwitch.h>
#include <Wire.h>

#define SW_pin_1 10
#define Y_pin_1 A0
#define X_pin_1 A1
#define Wacht_tijd 500
int Hoeveelheid_Pillen = 4;
unsigned long Tijd_Nu = 0;
int pil = 0;
int pil_oud = 0;
int tijd = 0;
int tijd_oud = 0;
int d = 0;
unsigned long HAHA = 0;
int Welke_Pil[4][6] = {           // Dit is voor het instellen van de datum dat de bepaalde pil moet worden ingenomen
  {2020, 7, 5, 11, 4, 3},
  {2020, 4, 6, 12, 0, 4},
  {2020, 4, 6, 12, 0, 5},
  {2020, 4, 6, 12, 0, 6}
};
int Welke_Pil_Oud;

char tijd_alarm = 20;
int Zendt_Tijd = 300;
char a = 0;
char j;
int buf;
RTC_DS1307 rtc;
RCSwitch mySwitch = RCSwitch();
DateTime now = (DateTime(2020, 7, 5, 11, 5, 0) ); // Wanneer er een RTC klok module is aangesloten moet het DateTime now =RTC.now(); worden

void setup()
{
  Serial.begin(9600);
  pinMode(SW_pin_1, INPUT);
  digitalWrite(SW_pin_1, HIGH);
  pinMode (2, INPUT); // Dit is voor als de pil moet worden veranderd moet pin 2 met de ground worden verbonden
  pinMode (3, INPUT);  // Dit is om de dislay aan te zetten voor het invoeren van de datums van de pillen, pin 3 moet dan met ground worden verbonden.
  pinMode (4, INPUT);
  rtc.begin();
  Wire.begin();
  mySwitch.enableTransmit(10); // De zender moet geconnect worden op pin 10.

  pinMode(8, OUTPUT);
}

void loop()
{

  HAHA = millis();
  Wat_Veranderd();
  Tijd_Veranderen ();
  Het_Klaar_Zetten_Pil();
  Alarm_Doorgeef();
  Display_zonder_Display ();
  Limiet_Array_Datum();
  if (pil >  4)
  {
    pil = 0;
  }
  if (pil < 0)
  {
    pil = sizeof Hoeveelheid_Pillen;
  }
  if (tijd >  5   )
  {
    tijd = 0;
  }
  if ( tijd < 0)
  {
    tijd = 5;
  }
}
void Wat_Veranderd() // Hier staat Wat er wordt veranderd (De pil, het jaar, de bepaalde dag, )
{
  if ( millis() > Tijd_Nu + Wacht_tijd  )
  {

    if (digitalRead (2) == HIGH) // wanneer pin 13 met de ground is verbonden kan de variable pil veranderen
    {
      if (analogRead(X_pin_1 ) >= 0 && analogRead(X_pin_1 ) <= 50 && analogRead(Y_pin_1) >= 300 && analogRead(Y_pin_1) <= 700) // GAAT NAAR RECHTS (JOYSTICK 1)
      {
        pil = pil + 1;            // Wat er wordt ingesteld gaat een waarde naar rechts
        Tijd_Nu = millis();


      }
      if (analogRead(X_pin_1) <= 1023 && analogRead(X_pin_1 ) >= 950 && analogRead(Y_pin_1) >= 300 && analogRead(Y_pin_1) <= 700) // GAAT NAAR LINKS (JOYSTICK 1)
      {
        pil = pil - 1;         // Wat er wordt ingesteld gaat een waarde naar links
        Tijd_Nu = millis();
      }
    }
    if (digitalRead (2) == LOW)  // wanneer pin 13 niet geconnect aan ground is kan de datum van de bepaalde pil worden veranderd
    {
      if (analogRead(X_pin_1 ) >= 0 && analogRead(X_pin_1 ) <= 50 && analogRead(Y_pin_1) >= 300 && analogRead(Y_pin_1) <= 700) // GAAT NAAR RECHTS (JOYSTICK 1)
      {
        tijd = tijd + 1;            // datum wordt ingesteld, wanneer de joystick naar rechts toe gaat kan er een ander type datum worden veranderd
        Tijd_Nu = millis();
      }
      if (analogRead(X_pin_1) <= 1023 && analogRead(X_pin_1 ) >= 950 && analogRead(Y_pin_1) >= 300 && analogRead(Y_pin_1) <= 700) // GAAT NAAR LINKS (JOYSTICK 1)
      {
        tijd  = tijd - 1;          // datum wordt ingesteld, wanneer de joystick naar rechts toe gaat kan er een ander type datum worden veranderd
        Tijd_Nu = millis();
      }
    }
  }
}
void Tijd_Veranderen ()
{
  if ( millis() > Tijd_Nu + Wacht_tijd  )  // Dit zorgt ervoor dat de waarde  maar met 1 kan veranderen per 0.2 seconden
  {
    if (analogRead(Y_pin_1 ) >= 0 && analogRead(Y_pin_1 ) <= 50 && analogRead(X_pin_1) >= 300 && analogRead(X_pin_1) <= 700) // GAAT NAAR RECHTS (JOYSTICK 1)
    {
      Welke_Pil[pil][tijd] = Welke_Pil[pil][tijd] + 1; // Hierbij wordt de bepaalde tijd bij een bepaalde pil bepaald
      Tijd_Nu = millis();

    }
    if (analogRead(Y_pin_1) <= 1023 && analogRead(Y_pin_1 ) >= 950 && analogRead(X_pin_1) >= 300 && analogRead(X_pin_1) <= 700) // GAAT NAAR LINKS (JOYSTICK 1)
    {
      Welke_Pil[pil][tijd] = Welke_Pil[pil][tijd] - 1; // Hierbij wordt de bepaalde tijd bij een bepaalde pil bepaald
      Tijd_Nu = millis();

    }
  }
}
void Het_Klaar_Zetten_Pil()
{

  for (char i = 0; i < sizeof Hoeveelheid_Pillen; i++ )
  {
    if (now.year() == Welke_Pil[i][0] && now.month() == Welke_Pil[i][1 ] && now.day() == Welke_Pil[i][2] && now.hour() == Welke_Pil[i][3] && now.minute() == Welke_Pil[i][4] - 3) // Dit is dat de machine de pil 3 minuten voordat de patient het moet innemen het klaar zet
    {
      // Hier moet de bepaalde pil worden klaar gemaakt
    }
  }
}
void  Alarm_Doorgeef()
{
  unsigned long Tijd = 0;
  for (char i = 0; i < sizeof Hoeveelheid_Pillen; i++ )
  {
    if (now.year() == Welke_Pil[i][0] && now.month() == Welke_Pil[i][1 ] && now.day() == Welke_Pil[i][2] && now.hour() == Welke_Pil[i][3] && now.minute() == Welke_Pil[i][4]) // De pil moet nu worden ingenomen, de gebruiker krijgt een seintje door.
    {
      mySwitch.switchOn("11111", "00010");
    }
    if ( HAHA - Tijd == Zendt_Tijd)  // Na 3 seconden zenden stopt de RF module met zenden
    {
      mySwitch.switchOff("11111", "00010");
      Tijd = millis();
    }
  }
}


void Display_zonder_Display ()
{



  if   (digitalRead (3) == HIGH)   // dit is voor het instellen
  {
    if (d == 0)
    {
      Serial.print ("U heeft pillenbakje ");
      Serial.print(( int )pil);
      Serial.print (" geselecteerd \n ");
      d = 1;

    }
    if ((pil_oud - pil  >= 1) || (pil_oud - pil <= -1) )
    {
      Serial.print ("U heeft pillenbakje ");
      Serial.print(( int )pil);
      Serial.print (" geselecteerd \n ");
    }
    if (tijd_oud - tijd >= 1 || tijd_oud - tijd <= -1  )
    {
      switch  ( tijd)
      {
        case 0:
          Serial.print ("U heeft datum type jaar geselecteerd\n");
       
          break;
        case 1:
          Serial.print ("U heeft datum type maand geselecteerd\n");
          
          break;
        case 2:
          Serial.print ("U heeft datum type dag geselecteerd\n");
          
          break;
        case 3:
          Serial.print ("U heeft datum type uur geselecteerd\n");
          
          break;
        case 4:
          Serial.print ("U heeft datum type minuut geselecteerd\n");
          
          break;
        case 5:
          Serial.print ("U  heeft hoe de patient de pil moet nemen geselecteerd\n");
          
          break;
      }
    }
    else if (Welke_Pil_Oud - Welke_Pil[pil][tijd] == 1 || Welke_Pil_Oud - Welke_Pil[pil][tijd] == -1 && a == 0)
    {
      if (tijd == 0)
      {
        Serial.print("Bij Pil ");
        Serial.print (( int)pil);
        Serial.print("Is het jaar: ");
        Serial.print (( int)Welke_Pil[pil][tijd]);
        Serial.print("Ingesteld \n");
      }
      if (tijd == 1)
      {
        Serial.print("Bij Pil ");
        Serial.print (( int)pil);
        Serial.print("Is de maand: ");
        Serial.print (( int)Welke_Pil[pil][tijd]);
        Serial.print("Ingesteld \n");
      }
      if (tijd == 2)
      {
        Serial.print("Bij Pil ");
        Serial.print (( int)pil);
        Serial.print("Is de dag: ");
        Serial.print (( int)Welke_Pil[pil][tijd]);
        Serial.print("Ingesteld \n ");
      }
      if (tijd == 3)
      {
        Serial.print("Bij Pil ");
        Serial.print (( int)pil);
        Serial.print("Is het uur: ");
        Serial.print (( int)Welke_Pil[pil][tijd]);
        Serial.print("Ingesteld \n");
      }
      if (tijd == 4)
      {
        Serial.print("Bij Pil ");
        Serial.print (( int)pil);
        Serial.print("Is het minuut: ");
        Serial.print (( int)Welke_Pil[pil][tijd]);
        Serial.print("Ingesteld \n");
      }
      if (tijd == 5)
      {
        if (( int)Welke_Pil[pil][5] == 0)
        {
          Serial.print("Bij Pil ");
          Serial.print (( int)pil);
          Serial.print("Moet het geprakt worden \n");
        }
        if (( int)Welke_Pil[pil][tijd] == 1)
        {
          Serial.print("Bij Pil ");
          Serial.print (( int)pil);
          Serial.print("Moet het met water genomen worden \n ");
        }
        if (( int)Welke_Pil[pil][tijd] == 2)
        {
          Serial.print("Bij Pil ");
          Serial.print (( int)pil);
          Serial.print("Moet het met vla genomen worden \n");
        }
      }
    }
  }
  for (char i = 0; i < sizeof Hoeveelheid_Pillen ; i++ )
  {
    if (now.year() == Welke_Pil[i][0] && now.month() == Welke_Pil[i][1 ] && now.day() == Welke_Pil[i][2] && now.hour() == Welke_Pil[i][3] && now.minute() == Welke_Pil[i][4]  )  // dit is voor de display met het innemen.
    {
      if (Welke_Pil[i][5] == 1)
      {
        Serial.print("Neem de pil geprakt");
      }
      if (Welke_Pil[i][5] == 2)
      {
        Serial.print("Neem de pil met water");
      }
      if  (Welke_Pil[i][5] == 3)
      {
        Serial.print("neem de pil met vla");
      }
    }
  }
  pil_oud = pil;
  tijd_oud = tijd  ;
  Welke_Pil_Oud = Welke_Pil[pil][tijd];
}
void  Limiet_Array_Datum()
{
  for (char i = 0; i < 4; i++)
  {

    if (Welke_Pil[i][0] > 2050 || Welke_Pil[i][0] < 1950)
    {
      Welke_Pil[i][0] = now.year();
    }
    if (Welke_Pil[i][1] > 12 || Welke_Pil[i][1] < 0)
    {
      Welke_Pil[i][1] = now.month();
    }
    if (Welke_Pil[i][2] > 31 || Welke_Pil[i][2] < 0)
    {
      Welke_Pil[i][2] = now.day();
    }
    if (Welke_Pil[i][3] > 24 || Welke_Pil[i][3] < 0)
    {
      Welke_Pil[i][3] = now.hour();
    }
    if (Welke_Pil[i][4] > 60 || Welke_Pil[i][4] < 0)
    {
      Welke_Pil[i][4] = now.minute();
    }
  }
}
