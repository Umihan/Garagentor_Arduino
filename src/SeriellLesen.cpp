#include <SeriellLesen.h>
#include <GLOBALS.h>

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
          case 't':
            //*** Neues WarteIntervall festgelegt
            SchliessIntervall=frame[2];
            Serial.print("Neue Wartezeit :");
            Serial.println(SchliessIntervall);
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