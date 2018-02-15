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
    //int conversion_ADC_to_cm(int);
    void PIDThrottle();
    
    unsigned int Altitude, Throttle, AutoPilot, Grabber;
    unsigned int PulseLength, StartPulse;
  private:
    int LastError;
}
;
#endif
