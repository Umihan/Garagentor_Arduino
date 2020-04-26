#ifndef ENTPRELLUNG_H
#define ENTPRELLUNG_H

#include <Arduino.h>

//**** Klasse, um einen GPIO-Pin, als INPUT definiert, zu entprellen
class Entprellung
{
    public:
        int buttonstate=LOW;
        //** Constructor
        Entprellung(int pin);

        //*** Method for detecting Button-state
        //*** returns State of Button
        int readState();

        //*** Method for detecting raising Edges
        //*** returns true if detected
        bool raisingedge();
        
        //*** Method for detecting falling Edges
        //*** returns true if detected
        bool fallingedge();
        
    private:
        int _pin;
        unsigned long _lastMillis=0;
        unsigned long _intervall=50;
        int _lastState;
        bool _raisingEdge, _fallingEdge;

};
#endif