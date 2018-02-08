#ifndef ROBOT_H_INCLUDED
#define ROBOT_H_INCLUDED
#include "Arduino.h"
#include "defines.h"

class QuadCopter
{
  public:
    void Initialize();
    void Encode();
    int Altitude, Throttle, AutoPilot, Grabber;
}
;
#endif
