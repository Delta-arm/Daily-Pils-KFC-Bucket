/*
 * Naam Code: Pressure uitlezen Delta-Arm
 * Datum: 30-03-2020
 * Autheur: Max Besseling
 */
 
const int pinVplus = A0;                                           //hier wordt de Vout+ pin van de druksesnor op aangesloten
const int pinVmin = A1;                                            //hier wordt de Vout- pin van de druksesnor op aangesloten
const float offset = 0.0;                                          //het verschil tussen Vout+ en Vout-
float pressure;                                                    //perameter voor de gemeten druk
int setpoint[10];                                                  //dit zijn de verschillende xyz locaties waar de delta-arm naar toe moet
int setpointEind;                                                  //dit is de loctaie waar de medicijn wordt gedosseert
int motorOutput = 10;                                              //vacuümpomp wordt aangesloten op pin 10

float VoltageToPressure();                                         //functie om de uitgaande spanning van de sensor om te rekenen naar druk 
void PressureRead(float pressure);                                 //functie om de met de gemeten druk t bepalen of er een medicijn is opgepakt

void setup() 
{
  Serial.begin(9600);                                              //begin de seriële monitor
  pinMode(motorOutput, OUTPUT);                                    //defineer pin 10 als output
}

void loop() 
{
  PressureRead(VoltageToPressure());                               //voer de functie "PressureRead uit en passeer de perameter van de functie "VoltageToPressure"
}

float VoltageToPressure()
{
  float vPlus = (float) analogRead(pinVplus) / 1023.0 * 5.0;       //lees de spanning uit op A0
  float vMin = (float) analogRead(pinVmin) / 1023.0 * 5.0;         //lees de spanning uit op A1
  Serial.print(F("vPlus[V]="));
  Serial.println(vPlus, 4);
  Serial.print(F("vMin[V]="));
  Serial.println(vMin, 4);

  float diff = vPlus - vMin;                                       //bereken het verschil tussen Vout+ en Vout-
  diff -= offset;                                                  //het verschil wordt de nieuwe offset
  Serial.print(F("diff[V]="));
  Serial.println(diff, 4);
 
  Serial.print(F("pressure[kPa]="));
  Serial.println(pressure, 4);

  return pressure;
}

void PressureRead(float pressure)
{
  if(pressure >= 100000 && digitalRead(motorOutput == LOW))       //als de druk gelijk is aan de omgevingsdruk en de pomp staat uit, ga naar setpoint[x] en zet de pomp aan
  {
    if(setpoint[1])
    {
      digitalWrite(motorOutput ,HIGH);
      Serial.println("Pil wordt opgepakt");
    }
  }
  else if(pressure <= 100000 && digitalRead(motorOutput == HIGH)) //als de druk lager is aan de omgevingsdruk en de pomp staat aan, ga naar de dosseerplek en zet de pomp uit
  {
    if(setpointEind)
    {
      Serial.println("Pil wordt door gevoerd");
      digitalWrite(motorOutput ,LOW);
      Serial.println("pil wordt gedosseerd");
    }
  }
}
