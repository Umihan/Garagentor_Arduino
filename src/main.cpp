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
//*** Serielle Kommunikation
bool _SerOpen = false, _SerClose=false;;

byte frame[4];

int Zustand;


unsigned long Startzeit=0;
unsigned long SchliessIntervall=10000;   // Zeit, bis zum automatischen Schließen, zum Testen auf 5 Sekunden gesetzt


//*** Methoden Header
void EingaengeLesen();
void Bedingungen();
void Aktionen();
void SeriellLesen();

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
    SeriellLesen();

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

  
  delay(500);
}

void SeriellLesen()
{
  if(Serial.available()>0)
  {
    frame[0]=Serial.read();
    if(frame[0]=='@')
    {
      delay(5);
      int counter=1;
      while(Serial.available()>0 && counter<4)
      {
        frame[counter++]=Serial.read();
      }
      if(frame[3]==';')
      {
        //** Fehlerfreies Frame erhalten ***//
        switch(frame[1])
        {
          case 'o':
            _SerOpen=true;
          break;
          case 'c':
            _SerClose=true;
          break;
        }
      }
      else
      {
        // ** Fehlerhaftes Frame erhalten
      }
      
    }
    else
    {
      //*** Fehlerhaftes Frame
    }
    
  }
}
void Bedingungen()
{

  //*** Von ZU auf OEFFNEN
  if(Zustand==ST_ZU && (Taster==true || _SerOpen))
  {
    Zustand=ST_OEFFNEN;
    _SerOpen=false;
  }

  //*** Von OEFFNEN auf OFFEN
  if(Zustand==ST_OEFFNEN && So==true)
  {
    Zustand=ST_OFFEN;
    Startzeit=millis();                     // Zum erkennen, ob das Tor länger als Intervall Sekunden offen ist.
  }
   //*** von OFFEN auf SCHLIESSEN
   if(Zustand==ST_OFFEN && (Taster==true || (millis()- Startzeit >= SchliessIntervall) || _SerClose))
   {
     Zustand=ST_SCHLIESSEN;
     _SerClose=false;
   }

   //*** Von SCHLIESSEN auf ZU
   if(Zustand==ST_SCHLIESSEN && Su==true)
   {
     Zustand=ST_ZU;
   }

   //*** von BEREIT auf OEFFNEN
   if(Zustand==ST_Bereit && (Taster==true || _SerOpen))
   {
     Zustand=ST_OEFFNEN;
     _SerOpen=false;
   }

   //*** BEREIT auf SCHLIESSEN weil die Zeit abgelaufen ist
   if(Zustand==ST_Bereit && ((millis()- Startzeit >= SchliessIntervall) || _SerClose))
   {
     Zustand=ST_SCHLIESSEN;
     _SerClose=false;
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