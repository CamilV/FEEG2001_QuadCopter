#ifndef ROBOT_H_INCLUDED
#define ROBOT_H_INCLUDED
#include "Arduino.h"
#include "defines.h"


class QuadCopter
{
  public:
    void Initialize();
    void Encode();
    void ReadAltitude();
    void PIDThrottle();
    void drop();
    void SmoothAltitude();
    unsigned int Altitude, Throttle, AutoPilot, Grabber;
    unsigned int StartPulse;
  private:
    int LastError;
    long int lastDuration;
    float dt, Error_i = 0;
}
;
#endif
