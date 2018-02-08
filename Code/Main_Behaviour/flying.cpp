#include <Wire.h>
#include "QuadCopter.h"
#include "defines.h"
#include <Servo.h>


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

