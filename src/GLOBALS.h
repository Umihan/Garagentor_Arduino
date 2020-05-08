#ifndef GLOBALS_H
#define GLOBALS_H
#include <Arduino.h>

extern byte frame[4];
//*** Serielle Kommunikation
extern bool _SerOpen , _SerClose;
extern unsigned long SchliessIntervall;   // Zeit, bis zum automatischen Schließen, zum Testen auf 5 Sekunden gesetzt


#endif