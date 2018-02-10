#include "QuadCopter.h"
#include "defines.h"
#include <Wire.h>
#include <Servo.h>

int QuadCopter::conversion_ADC_to_cm(int a)
{
  return (int)472280 * pow(a,-1.665);
}
void QuadCopter::a(int b)
{
  
}
void QuadCopter::Initialize()
{
    Serial.begin(9600);
    pinMode(ThO, OUTPUT);
    pinMode(ThS, OUTPUT);
    pinMode(UST, OUTPUT);
    pinMode(SRV, OUTPUT);
    pinMode(RXIN, INPUT);
    pinMode(USE, INPUT);
}

void QuadCopter::Encode()
{
    char str[8];
    str[0] = (char)('0' + Altitude/100);
    str[1] = (char)('0' + Altitude/10 - Altitude/100*10);
    str[2] = (char)('0' + Altitude%10);
    str[3] = (char)('0' + Throttle/100);
    str[4] = (char)('0' + Throttle/10 - Throttle/100*10);
    str[5] = (char)('0' + Throttle%10);
    str[6] = (char)('0' + AutoPilot);
    str[7] = (char)('0' + Grabber);
    Serial.print(str);
}

void QuadCopter::ReadAltitude()
{
  int IR[NO_ITERATIONS],i,AVG, ALT[NO_ITERATIONS];
  for(i=0;i<NO_ITERATIONS;i++)
  {
    IR[i] = analogRead(IRSensor);
    ALT[i] = conversion_ADC_to_cm(IR[i]);
    AVG = ALT[i] / NO_ITERATIONS;
  }
  int j=0;
  for(i=0;i<NO_ITERATIONS;i++)
    if(ALT[i] > (AVG + THRESHOLD) || ALT[i] < (AVG - THRESHOLD))
    {
        AVG = AVG - ALT[i];
        j++;
    }
  AVG = AVG * NO_ITERATIONS / (NO_ITERATIONS - j);
  
}

