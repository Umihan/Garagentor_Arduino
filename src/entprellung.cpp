#include <Arduino.h>
#include <Entprellung.h>

//** Constructor
        Entprellung::Entprellung(int pin)
        {
            _pin=pin;
            _lastMillis=millis();
            buttonstate=_lastState=digitalRead(_pin);
        }

//*** Method for detecting Button-state
//*** returns State of Button       
        int Entprellung::readState()
        {
            int aktState=digitalRead(_pin);
            _fallingEdge=_raisingEdge=false;
            if(aktState!=_lastState)
            {
                _lastMillis=millis();
            }
            if(millis()-_lastMillis>_intervall)
            {
                if(aktState!=buttonstate)
                {
                    buttonstate=aktState;
                    if(buttonstate==LOW)
                        _fallingEdge=true;
                    else
                        _raisingEdge=true;             
                }
            }
            _lastState=aktState;
            return buttonstate;
        }

        //*** Method for detecting raising Edges
        //*** returns true if detected
        bool Entprellung::raisingedge()
        {
            readState();
            return _raisingEdge;
        }
        
        //*** Method for detecting falling Edges
        //*** returns true if detected
        bool Entprellung::fallingedge()
        {
            readState();
            return _fallingEdge;
        }

    