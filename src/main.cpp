#include <Arduino.h>
//*** I/O des Arduino
#define SensorOben    3               // Sensor oben, Schließer -> Tor geöffnet
#define SensorUnten   2               // Sensor unten, Schließer -> Tor geschlossen
#define TasterManuell 4               // Taster zum Steuern des Tores, Schließer
#define MotorAuf      9               // H-Brücke, Anschluss zum Öffnen des Tores
#define MotorZu       10              // H-Brücke, Anschluss zum Schließen des Tores

#define ST_Bereit     1
#define ST_ZU         2
#define ST_OFFEN      3
#define ST_OEFFNEN    4
#define ST_SCHLIESSEN 5


bool Su, So, Taster, Mauf, Mzu;

int Zustand;

unsigned long Startzeit=0;
unsigned long SchliessIntervall=5000;   // Zeit, bis zum automatischen Schließen, zum Testen auf 5 Sekunden gesetzt


//*** Methoden Header
void EingaengeLesen();
void Bedingungen();
void Aktionen();

void setup() {
  
  Serial.begin(115200);

  pinMode(SensorOben,INPUT_PULLUP);
  pinMode(SensorUnten,INPUT);
  pinMode(TasterManuell,INPUT);
  pinMode(MotorAuf,OUTPUT);
  pinMode(MotorZu,OUTPUT);

  //*** Sensoren auslesen, um den Zustand des Tores bestimmen zu können
  EingaengeLesen();

  if(!Su && !So)
    Zustand=ST_Bereit;
  if(So)
    Zustand=ST_OFFEN;
  if(Su)
    Zustand=ST_ZU;

  if(Zustand==ST_Bereit || Zustand==ST_OFFEN)
    Startzeit=millis();

  //*** Fehler muss noch kontrolliert werden (beide Sensoren aktiv!!)

}

void loop() 
{
  //*************************/
  //*** Eingänge einlesen ***/
  //*************************/
    EingaengeLesen();

  //******************************************************************************************************/
  //*** Bedingungen und Übergänge kontrollieren, um zu wissen, welcher Zustand gerade aktiv sein muss ****/
  //******************************************************************************************************/
    Bedingungen();

  //*****************/
  //*** AKTIONEN ****/
  //*****************/
    Aktionen();

}

void EingaengeLesen()
{
  So=digitalRead(SensorOben)==HIGH;
  Su=digitalRead(SensorUnten)==HIGH;
  Taster=digitalRead(TasterManuell)==HIGH;

  //** Nur zum Debuggen
  // while(Serial.available()>0)
  // {
  //   char k=Serial.read();
  //   switch(k)
  //   {
  //     case 'o':
  //       So=true;
  //       Su=false;
  //       Taster=false;
  //       break;
  //      case 'u':
  //       Su=true;
  //       So=false;
  //       Taster=false;
  //       break;
  //       case 't':
  //       Taster=true;
  //       break;
  //   }
  //   Serial.println(k);
  // }
  delay(500);
}

void Bedingungen()
{

  //*** Von ZU auf OEFFNEN
  if(Zustand==ST_ZU && Taster==true)
  {
    Zustand=ST_OEFFNEN;
  }

  //*** Von OEFFNEN auf OFFEN
  if(Zustand==ST_OEFFNEN && So==true)
  {
    Zustand=ST_OFFEN;
    Startzeit=millis();                     // Zum erkennen, ob das Tor länger als Intervall Sekunden offen ist.
  }
   //*** von OFFEN auf SCHLIESSEN
   if(Zustand==ST_OFFEN && (Taster==true || (millis()- Startzeit >= SchliessIntervall)))
   {
     Zustand=ST_SCHLIESSEN;
   }

   //*** Von SCHLIESSEN auf ZU
   if(Zustand==ST_SCHLIESSEN && Su==true)
   {
     Zustand=ST_ZU;
   }

   //*** von BEREIT auf OEFFNEN
   if(Zustand==ST_Bereit && Taster==true)
   {
     Zustand=ST_OEFFNEN;
   }

   //*** BEREIT auf SCHLIESSEN weil die Zeit abgelaufen ist
   if(Zustand==ST_Bereit &&  (millis()- Startzeit >= SchliessIntervall))
   {
     Zustand==ST_SCHLIESSEN;
   }
}

void Aktionen()
{
  switch (Zustand)
  {
  case ST_OFFEN:
      digitalWrite(MotorAuf,LOW);
      digitalWrite(MotorZu,LOW);
      Serial.println("OFFEN");    //*** 
    break;
  case ST_OEFFNEN:
      digitalWrite(MotorAuf,HIGH);
      digitalWrite(MotorZu,LOW);
      Serial.println("OEFFNEN!!");
    break;
  case ST_ZU:
      digitalWrite(MotorAuf,LOW);
      digitalWrite(MotorZu,LOW);
      Serial.println("ZU");
    break;
  case ST_SCHLIESSEN:
      digitalWrite(MotorZu,HIGH);
      digitalWrite(MotorAuf,LOW);
      Serial.println("SCHLIESSEN");
    break;
  case ST_Bereit:
      digitalWrite(MotorAuf,LOW);
      digitalWrite(MotorZu,LOW);
      Serial.println("B E R E I T ");
    break;
  default:
    break;
  }
}