//library's
#include <math.h>
#include <Servo.h>


Servo MyServo[3];
//globale variabele 
int Posities [22][3];   //twee-dementionale array waar de X,Y,Z waardes in komen van de verschillende posities 

float Ltop = 20;  // afstand tussen Pdoel en Ppols

float afstands1;
float afstands2;
float afstands3;


float ZPelleboog1 ;
float XPelleboog1 ;
float YPelleboog1 ;

float ZPelleboog2 ;
float XPelleboog2 ;
float YPelleboog2 ;

float ZPelleboog3 ;
float XPelleboog3 ;
float YPelleboog3 ;


float ZPpols1;
float XPpols1;
float YPpols1;


float ZPpols2;
float XPpols2;
float YPpols2;

float ZPpols3;
float XPpols3;
float YPpols3;

float servo1hoek;
float servo2hoek;
float servo3hoek;

double T;
double T1;
float i;
float hoek;
double radialen;

int j = 0;
char PositionPill = 1; // positie van de pillen beginen bij de 2de positie in de array 
char X = -10; // X positie van de pillen begint op X = -10
char Y;       
int Z = 150;  //de Z positie van de pillen zijn allemaal 150  

//prototypes
void hoekberekenen(float Xbepalen,float Ybepalen,float Zbepalen);

void setup() {

  Serial.begin(9600);

  //Servo's aan de aangewezen pinnen kopelen
  MyServo[0].attach(5);
  MyServo[1].attach(6);
  MyServo[2].attach(7);

  // positie van het bakje instellen 
  Posities[0][0] = 0;
  Posities[0][1] = 50;
  Posities[0][2] = 130;

  //positie van alle pillen instellen 
  for(i=0; i<3;i++) // loop voor de 3 kolomen van de pilen 
  {
    Y = -20;
    for(j=0; j<7; j++) // loop voor de 7 rijen van pillen elke dag 1  
    {
      Posities[PositionPill][0] = X;
      Posities[PositionPill][1] = Y;
      Posities[PositionPill][2] = Z;
      Y = Y - 10;//elke pil zit 1cm uit elkaar
      PositionPill++;
    }
    X = X + 10; //elke rij zit 1cm uit elkaar
  }
  
  //testen van elke positie met elke hoek
  for (j=0; j<22; j++)
  {
    Serial.print("positie");
    Serial.print(j);
    Serial.print(" X= ");
    Serial.print(Posities[j][0]);
    Serial.print(" Y= ");
    Serial.print(Posities[j][1]);  
    Serial.print(" Z= ");
    Serial.println(Posities[j][2]); 
    hoekberekenen(Posities[j][0],Posities[j][1],Posities[j][2]);
  }
}

void loop() 
{
  
}

//functie de van de X,Y,Z waardes de hoeken van de servo's berekent die daar bij horen 
void hoekberekenen(float Xbepalen, float Ybepalen, float Zbepalen)
{
  //alle voorafgaande hoeken weer op 0 zeten
  servo1hoek = 0;
  servo2hoek = 0;
  servo3hoek = 0;

  
  for (hoek = -30; hoek<160 ; hoek = hoek + 0.1) //voorloep die er voor zorgt dat elke hoek tussen -30 en 160 graden gecontroleerd word
  {
    //omrekenen van grades naar radialen zodat je met de math library de sinus en de cosinus uit kan rekenen
    T = (float)hoek/(float)360;
    T1 = (float)T * (float)2;
    radialen = (float)T1 * (float)M_PI;

    //positie berekenen van de elleboog van servo 1
    ZPelleboog1 = 60 * (sin(radialen));
    XPelleboog1 = 100 + 60 * cos(radialen);
    YPelleboog1 = 0;
    //positie berekenen van de pols van servo 1
    ZPpols1 = Zbepalen;
    XPpols1 = Xbepalen + Ltop;
    YPpols1 = Ybepalen;

    //positie berekenen van de elleboog van servo 2
    ZPelleboog2 = 60 * (sin(radialen));
    XPelleboog2 = 100 - sin(1/3*M_PI)*60*cos(radialen);
    YPelleboog2 = sin(1/3*M_PI)*60*cos(radialen);
    //positie berekenen van de pols van servo 2
    ZPpols2 = Zbepalen;
    XPpols2 = Xbepalen - cos(1/3*M_PI)*Ltop;
    YPpols2 = Ybepalen + sin(1/3*M_PI)*Ltop;

    //positie berekenen van de elleboog van servo 3
    ZPelleboog3 = 60 * (sin(radialen));
    XPelleboog3 = 100 - cos(1/3*M_PI)*60*cos(radialen);
    YPelleboog3 = 0 - sin(1/3*M_PI)*60*cos(radialen);
    //positie berekenen van de pols van servo 3
    ZPpols3 = Zbepalen;
    XPpols3 = Xbepalen - cos(1/3*M_PI)*Ltop;
    YPpols3 = Ybepalen - sin(1/3*M_PI)*Ltop;

    //afstand bereken tussen de elleboog en de pols voor elke servo 
    afstands1 = sqrt(sq((ZPelleboog1-ZPpols1))+sq((XPelleboog1-XPpols1))+sq((YPelleboog1-YPpols1)));
    afstands2 = sqrt(sq((ZPelleboog2-ZPpols2))+sq((XPelleboog2-XPpols2))+sq((YPelleboog2-YPpols2)));
    afstands3 = sqrt(sq((ZPelleboog3-ZPpols3))+sq((XPelleboog3-XPpols3))+sq((YPelleboog3-YPpols3)));

    //controleren of de afstand tussen de elleboog en de pols van servo 1 de juiste lengete heeft 
    if (afstands1>199.9 && afstands1<200.1)
    {
      if(servo1hoek == 0)
      {
        //de juiste hoek en afstand printen op de serial monitor
        Serial.print("   Hoek = ");
        Serial.print(hoek);
        Serial.print("   Afstands1 = ");
        Serial.println(afstands1);
        servo1hoek = hoek;
        MyServo[0].write(servo1hoek); //servo 1 naar de juiste hoek zetten 
      } 
    } 

    //controleren of de afstand tussen de elleboog en de pols van servo 2 de juiste lengete heeft
    if (afstands2>199.9 && afstands2<200.1)
    {
      if(servo2hoek == 0)
      {
        //de juiste hoek en afstand printen op de serial monitor
        Serial.print("   Hoek = ");
        Serial.print(hoek);
        Serial.print("   Afstands2 = ");
        Serial.println(afstands2);
        servo2hoek = hoek;  
        MyServo[1].write(servo2hoek); //servo 2 naar de juiste hoek zetten 
      }
    } 

    //controleren of de afstand tussen de elleboog en de pols van servo 3 de juiste lengete heeft  
    if (afstands3>199.9 && afstands3<200.1)
    {
      if(servo3hoek == 0)
      {
        //de juiste hoek en afstand printen op de serial monitor
        Serial.print("   Hoek = ");
        Serial.print(hoek);
        Serial.print("   Afstands3 = ");
        Serial.println(afstands3);
        servo3hoek = hoek;
        MyServo[2].write(servo3hoek); //servo 3 naar de juiste hoek zetten 
      }
    } 
  }
}
