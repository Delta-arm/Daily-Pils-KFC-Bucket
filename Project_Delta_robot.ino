#include <U8g2lib.h>
#include "RTClib.h"
#include <RCSwitch.h>
#include <Wire.h>
#define SW_pin_1 10
#define Y_pin_1 A0
#define X_pin_1 A1
#define Wacht_tijd 200
#define Instel_Mogelijkheden 5
#define Hoeveelheid_Pillen  4  // dit is een variable die aangeeft hoeveel pillen er in het systeem zitten.
unsigned long Tijd_Nu = 0;
int pil = 0;
int pil_oud = 0;
int tijd = 0;
int tijd_oud = 0;
int d = 0;
unsigned long HAHA = 0;
int Welke_Pil[4][6] = {           // Dit is voor het instellen van de datum dat de bepaalde pil moet worden ingenomen
  {2020, 7, 5, 11, 4, 3},         // De getallen in de array staan voor:
  {2020, 4, 6, 12, 0, 4},         // {Jaar, Maand, Dag, Uur, Minuut, Manier van innemen voor de patient}
  {2020, 4, 6, 12, 0, 5},
  {2020, 4, 6, 12, 0, 6}
};
int Welke_Pil_Oud;

char tijd_alarm = 20;
int Zendt_Tijd = 300;
char a = 0;
char j;
int buf;
RTC_DS3231 rtc;   // tijd clock module die er gebruikt wordt.
RCSwitch mySwitch = RCSwitch();  // Voor de RF module
DateTime now = rtc.now() ;
U8G2_ST7920_128X64_F_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* CS=*/ 10, /* reset=*/ 8);    // De pins waar de display op aangesloten moet.
void setup()
{
  Serial.begin(9600);
  pinMode(SW_pin_1, INPUT);
  digitalWrite(SW_pin_1, HIGH);
  pinMode (2, INPUT);
  pinMode(3, INPUT);
  rtc.begin();
  Wire.begin();
  u8g2.begin();
  mySwitch.enableTransmit(10); // De zender moet verbonden worden op pin 10.
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
}
void Wat_Veranderd()
{
  if ( millis() > Tijd_Nu + Wacht_tijd  )
  {
    if (digitalRead (2) == HIGH) // Wanneer welke pillenbak verandert moet worden moet pin 2 aan 5volt worden aangesloten.
    {
      if (analogRead(X_pin_1 ) >= 0 && analogRead(X_pin_1 ) <= 50 && analogRead(Y_pin_1) >= 300 && analogRead(Y_pin_1) <= 700)// Joystick gaat naar rechts.)
      {
        pil = pil + 1;            // Wat er wordt ingesteld gaat een waarde naar rechts
        Tijd_Nu = millis();


      }
      if (analogRead(X_pin_1) <= 1023 && analogRead(X_pin_1 ) >= 950 && analogRead(Y_pin_1) >= 300 && analogRead(Y_pin_1) <= 700) // Joystick gaat naar links.
      {
        pil = pil - 1;          
        Tijd_Nu = millis();
      }
    }
    if (digitalRead (2) == LOW) // Wanneer wat er bij een pillenbak moet worden verandert, moet pin 2 aan de ground worden aangesloten.
    {
      if (analogRead(X_pin_1 ) >= 0 && analogRead(X_pin_1 ) <= 50 && analogRead(Y_pin_1) >= 300 && analogRead(Y_pin_1) <= 700) // Joystick gaat naar rechts.
      {
        tijd = tijd + 1;        // Voor het instellen van welke datum soort (jaar, maand enz...) of hoe de pil moet worden ingenomen.
        Tijd_Nu = millis();     // Is ervoor om een delay intestellen zonder dat het hele programma wordt delayed.
      }
      if (analogRead(X_pin_1) <= 1023 && analogRead(X_pin_1 ) >= 950 && analogRead(Y_pin_1) >= 300 && analogRead(Y_pin_1) <= 700) // Joystick gaat naar links.
      {
        tijd  = tijd - 1;        // Voor het instellen van welke datum soort (jaar, maand enz...) of hoe de pil moet worden ingenomen.
        Tijd_Nu = millis();      // Is ervoor om een delay intestellen zonder dat het hele programma wordt delayed.
      }
    }
  }
}
void Tijd_Veranderen ()
{
  if ( millis() > Tijd_Nu + Wacht_tijd  )  // Dit zorgt ervoor dat de waarde  maar met 1 kan veranderen per 0.2 seconden
  {
    if (analogRead(Y_pin_1 ) >= 0 && analogRead(Y_pin_1 ) <= 50 && analogRead(X_pin_1) >= 300 && analogRead(X_pin_1) <= 700)// Joystick gaat naar beneden.
    {
      Welke_Pil[pil][tijd] = Welke_Pil[pil][tijd] + 1; // De datum of de manier van hoe de pillenbak moet worden ingenomen wordt verandert.
      Tijd_Nu = millis();     // Is ervoor om een delay intestellen zonder dat het hele programma wordt delayed.

    }
    if (analogRead(Y_pin_1) <= 1023 && analogRead(Y_pin_1 ) >= 950 && analogRead(X_pin_1) >= 300 && analogRead(X_pin_1) <= 700) // Joystick gaat naar boven.
    {
      Welke_Pil[pil][tijd] = Welke_Pil[pil][tijd] - 1; // De datum of de manier van hoe de pillenbak moet worden ingenomen wordt veranderd.
      Tijd_Nu = millis();     // Is ervoor om een delay intestellen zonder dat het hele programma wordt delayed.

    }
  }
}
void Het_Klaar_Zetten_Pil()
{
  for (char i = 0; i < sizeof pil; i++)
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
  for (char  i = 0; i < sizeof pil; i++)
  {
    if (now.year() == Welke_Pil[i][0] && now.month() == Welke_Pil[i][1 ] && now.day() == Welke_Pil[i][2] && now.hour() == Welke_Pil[i][3] && now.minute() == Welke_Pil[i][4]) // De pil moet nu worden ingenomen, de gebruiker krijgt een seintje door.
    {
      mySwitch.switchOn("11111", "00010");   // Er wordt een bepaalde seintje verzonden aan de receiver dat de trilmotor aan moet.
    }
    if ( HAHA - Tijd == Zendt_Tijd)  // Na 3 seconden zenden stopt de RF module met zenden
    {
      mySwitch.switchOff("11111", "00010");  // Er wordt een bepaalde seintje verzonden aan de receiver dat de trilmotor uit moet
      Tijd = millis();   // Is ervoor om een delay intestellen zonder dat het hele programma wordt delayed.
    }
  }
}


void Display_zonder_Display ()
{

  if (pil >  Hoeveelheid_Pillen)  // Deze if statements zijn ervoor dat er niet meer dan pillen of instel mogelijkheden worden ingevoerd dan het maximum.
  {
    pil = 0;
  }
  if (pil < 0)
  {
    pil =   Hoeveelheid_Pillen;
  }
  if (tijd >  Instel_Mogelijkheden)
  {
    tijd = 0;
  }
  if ( tijd < 0)
  {
    tijd = Instel_Mogelijkheden ;
  }
  if   (digitalRead (3) == HIGH)   // Dit is voor wanneer de verpleegkundige iets wilt invoeren.
  {
    if (d == 0)    // Wanneer het instellen van de display voor het eerst aangaat wordt aangegeven welke pillenbak er geselecteerd is.
    {
      u8g2.clearBuffer();          // internal memory wordt gewist van de display
      u8g2.setFont(u8g2_font_ncenB08_tr); // Bepaalde font die gekozen is voor de display
      u8g2.drawStr(0, 10, "U heeft pillenbak "); // drawSTR betekend dat wat er staat wordt geschreven op de display. (x-as,y-as, "tekst" ).
      u8g2.drawStr(0, 10, (int)pil);   // (x-as,y-as, variable)
      u8g2.drawStr(30, 10, "geselecteerd "); // write something to the internal memory
      u8g2.sendBuffer();          //  De interne memory wordt gestuurd naar de display.

    }
    if ((pil_oud - pil  >= 1) || (pil_oud - pil <= -1) )  // Wanneer er na het opstarten een andere pillenbak is geselecteerd wordt dat met dit stukje code aangegeven.
    {
      u8g2.clearBuffer();
      u8g2.setFont(u8g2_font_ncenB08_tr); // Uitleg wat de commands beteken staan aan de bovenkant van deze functie.
      u8g2.drawStr(0, 10, "U heeft pillenbak ");
      u8g2.drawStr(25, 10, ( int )pil);
      u8g2.drawStr(30, 10, "geselecteerd ");
      u8g2.sendBuffer();
    }
    if (tijd_oud - tijd >= 1 || tijd_oud - tijd <= -1  )  // Deze if statement is ervoor om het gene wat ze willen veranderen te laten zien.
    {
      switch  ( tijd)
      {
        case 0:
          u8g2.clearBuffer();  // Uitleg wat de commands beteken staan aan de bovenkant van deze functie.
          u8g2.setFont(u8g2_font_ncenB08_tr);
          u8g2.drawStr(0, 10, "U heeft datum type jaar geselecteerd\n");
          u8g2.sendBuffer();
          break;
        case 1:
          u8g2.clearBuffer();   // Uitleg wat de commands beteken staan aan de bovenkant van deze functie.
          u8g2.setFont(u8g2_font_ncenB08_tr);
          u8g2.drawStr(0, 10, "U heeft datum type maand geselecteerd\n");
          u8g2.sendBuffer();
          break;
        case 2:
          u8g2.clearBuffer();   // Uitleg wat de commands beteken staan aan de bovenkant van deze functie.
          u8g2.setFont(u8g2_font_ncenB08_tr);
          u8g2.drawStr(0, 10, "U heeft datum type dag geselecteerd\n");
          u8g2.sendBuffer();
          break;
        case 3:
          u8g2.clearBuffer();  // Uitleg wat de commands beteken staan aan de bovenkant van deze functie.
          u8g2.setFont(u8g2_font_ncenB08_tr);
          u8g2.drawStr(0, 10, "U heeft datum type uur geselecteerd\n");
          u8g2.sendBuffer();
          break;
        case 4:
          u8g2.clearBuffer(); // Uitleg wat de commands beteken staan aan de bovenkant van deze functie.
          u8g2.setFont(u8g2_font_ncenB08_tr);
          u8g2.drawStr(0, 10, "U heeft datum type minuut geselecteerd\n");
          u8g2.sendBuffer();
          break;
        case 5:
          u8g2.clearBuffer(); // Uitleg wat de commands beteken staan aan de bovenkant van deze functie.
          u8g2.setFont(u8g2_font_ncenB08_tr);
          u8g2.drawStr(0, 10, "U  heeft hoe de patient de pil moet nemen geselecteerd");
          u8g2.sendBuffer();
          break;
      }
    }
    else if (Welke_Pil_Oud - Welke_Pil[pil][tijd] == 1 || Welke_Pil_Oud - Welke_Pil[pil][tijd] == -1 && a == 0)  // Deze if statement is ervoor om op de dislay te laten zien de hoeveelheid van wat de persoon bij welke pillenbak aan het instellen is.
    {
      if (tijd == 0)
      {
        u8g2.clearBuffer(); // Uitleg wat de commands beteken staan aan de bovenkant van deze functie.
        u8g2.setFont(u8g2_font_ncenB08_tr);
        u8g2.drawStr(0, 10, "Bij pil ");
        u8g2.drawStr(25, 10, ( int )pil);
        u8g2.drawStr(30, 10, "is het jaar ");
        u8g2.drawStr(25, 10, (int)Welke_Pil[pil][tijd]);
        u8g2.drawStr(0, 10, "Ingesteld ");
        u8g2.sendBuffer();
      }
      if (tijd == 1)
      {
        u8g2.clearBuffer();  // Uitleg wat de commands beteken staan aan de bovenkant van deze functie.
        u8g2.setFont(u8g2_font_ncenB08_tr);
        u8g2.drawStr(0, 10, "Bij pil ");
        u8g2.drawStr(25, 10, ( int )pil);
        u8g2.drawStr(30, 10, "is de maand");
        u8g2.drawStr(25, 10, (int)Welke_Pil[pil][tijd]);
        u8g2.drawStr(0, 10, "Ingesteld ");
        u8g2.sendBuffer();
      }
      if (tijd == 2)
      {
        u8g2.clearBuffer();  // Uitleg wat de commands beteken staan aan de bovenkant van deze functie.
        u8g2.setFont(u8g2_font_ncenB08_tr);
        u8g2.drawStr(0, 10, "Bij pil ");
        u8g2.drawStr(25, 10, ( int )pil);
        u8g2.drawStr(30, 10, "is de dag");
        u8g2.drawStr(25, 10, (int)Welke_Pil[pil][tijd]);
        u8g2.drawStr(0, 10, "Ingesteld ");
        u8g2.sendBuffer();
      }
      if (tijd == 3)
      {
        u8g2.clearBuffer();  // Uitleg wat de commands beteken staan aan de bovenkant van deze functie.
        u8g2.setFont(u8g2_font_ncenB08_tr);
        u8g2.drawStr(0, 10, "Bij pil ");
        u8g2.drawStr(25, 10, ( int )pil);
        u8g2.drawStr(30, 10, "is het uur");
        u8g2.drawStr(25, 10, (int)Welke_Pil[pil][tijd]);
        u8g2.drawStr(0, 10, "Ingesteld ");
        u8g2.sendBuffer();
      }
      if (tijd == 4)
      {
        u8g2.clearBuffer();  // Uitleg wat de commands beteken staan aan de bovenkant van deze functie.
        u8g2.setFont(u8g2_font_ncenB08_tr);
        u8g2.drawStr(0, 10, "Bij pil ");
        u8g2.drawStr(25, 10, ( int )pil);
        u8g2.drawStr(30, 10, "is het minuut");
        u8g2.drawStr(25, 10, (int)Welke_Pil[pil][tijd]);
        u8g2.drawStr(0, 10, "Ingesteld ");
        u8g2.sendBuffer();
      }
      if (tijd == 5)
      {
        if (( int)Welke_Pil[pil][5] == 0)   // Deze if statement is ervoor om op de display bij het instellen te laten zien hoe de bepaalde pil ingenomen wordt.
        {
          u8g2.clearBuffer();  // Uitleg wat de commands beteken staan aan de bovenkant van deze functie.
          u8g2.setFont(u8g2_font_ncenB08_tr);
          u8g2.drawStr(0, 10, "Bij pil ");
          u8g2.drawStr(25, 10, ( int )pil);
          u8g2.drawStr(30, 10, "Moet het geprakt worden");
          u8g2.sendBuffer();
        }
        if (( int)Welke_Pil[pil][tijd] == 1)
        {
          u8g2.clearBuffer();   // Uitleg wat de commands beteken staan aan de bovenkant van deze functie.
          u8g2.setFont(u8g2_font_ncenB08_tr);
          u8g2.drawStr(0, 10, "Bij pil ");
          u8g2.drawStr(25, 10, ( int )pil);
          u8g2.drawStr(30, 10, "Moet het met water genomen worden");
          u8g2.sendBuffer();

        }
        if (( int)Welke_Pil[pil][tijd] == 2)
        {
          u8g2.clearBuffer();   // Uitleg wat de commands beteken staan aan de bovenkant van deze functie.
          u8g2.setFont(u8g2_font_ncenB08_tr);
          u8g2.drawStr(0, 10, "Bij pil ");
          u8g2.drawStr(25, 10, ( int )pil);
          u8g2.drawStr(30, 10, "Moet het met vla genomen worden");
          u8g2.sendBuffer();
        }
      }
    }
  }
  for (char i = 0; i < sizeof Hoeveelheid_Pillen ; i++ )
  {
    if (now.year() == Welke_Pil[i][0] && now.month() == Welke_Pil[i][1 ] && now.day() == Welke_Pil[i][2] && now.hour() == Welke_Pil[i][3] && now.minute() == Welke_Pil[i][4]  )  // dit is voor de display met het innemen van de pllen.
    {
      if (Welke_Pil[i][5] == 1)
      {
        u8g2.clearBuffer();
        u8g2.setFont(u8g2_font_ncenB08_tr);
        u8g2.drawStr(0, 10, "Neem de pil geprakt ");
        u8g2.sendBuffer();
      }
      if (Welke_Pil[i][5] == 2)
      {
        u8g2.clearBuffer();
        u8g2.setFont(u8g2_font_ncenB08_tr);
        u8g2.drawStr(0, 10, "Neem de pil met water ");
        u8g2.sendBuffer();
      }
      if  (Welke_Pil[i][5] == 3)
      {
        u8g2.clearBuffer();
        u8g2.setFont(u8g2_font_ncenB08_tr);
        u8g2.drawStr(0, 10, "Neem de pil met water ");
        u8g2.sendBuffer();
      }
    }
  }
  pil_oud = pil;  // De variable naam_oud is ervoor om een vergelijking te maken waarbij te zien is of de persoon die aan het instellen is iets aan het veranderen is.
  tijd_oud = tijd  ;
  Welke_Pil_Oud = Welke_Pil[pil][tijd];
}
void  Limiet_Array_Datum()
{
  for (char i = 0; i < Hoeveelheid_Pillen; i++) // De for statement is ervoor om elke pillenbak langs te gaan.
  {

    if (Welke_Pil[i][0] > 2050 || Welke_Pil[i][0] < now.year())  // Deze if statement is ervoor dat het jaar niet te ver in de toekomst kan worden ingesteld en niet in het verleden
    {
      Welke_Pil[i][0] = now.year();
    }
    if (Welke_Pil[i][1] > 12 || Welke_Pil[i][1] < 0) // Maanden kunnen niet onder de nul worden insteld en niet boven de 12
    {
      Welke_Pil[i][1] = now.month();
    }
    if (Welke_Pil[i][2] > 31 || Welke_Pil[i][2] < 0) // Dagen kunnen niet onder de nul worden insteld en niet boven de 31
    {
      Welke_Pil[i][2] = now.day();
    }
    if (Welke_Pil[i][3] > 24 || Welke_Pil[i][3] < 0)  // Uren kunnen niet onder de nul worden insteld en niet boven de 24
    {
      Welke_Pil[i][3] = now.hour();
    }
    if (Welke_Pil[i][4] > 60 || Welke_Pil[i][4] < 0) // Minuten kunnen niet onder de nul worden insteld en niet boven de 60
    {
      Welke_Pil[i][4] = now.minute();
    }
  }
}
