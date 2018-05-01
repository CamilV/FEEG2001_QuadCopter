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
    unsigned long int StartPulse;
    bool pAutopilot,Takeoff;
  private:
    int LastError;
    long int lastDuration;
    float dt, Error_i = 0;
    bool State;
    int MaxValue1,MaxValue2;
}
;
#endif
