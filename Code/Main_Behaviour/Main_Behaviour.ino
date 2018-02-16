#include "QuadCopter.h"
#include <EnableInterrupt.h>

QuadCopter Q;

void Switch(){
  if (digitalRead(RXIN) == HIGH) {
    Q.StartPulse = micros();
  } 
  else {
    noInterrupts();
    Q.PulseLength = micros() - Q.StartPulse;
    if(Q.PulseLength > ReceiverThrottle) {digitalWrite(ThS, HIGH); Q.AutoPilot = 1;}
    else {digitalWrite(ThS, LOW); Q.AutoPilot = 0;}
    interrupts();
  }
}

void setup() {
  Q.Initialize();
  enableInterrupt(RXIN, Switch, CHANGE);
}

void loop() {
  unsigned long t = millis();
  while(millis() - t < 500){
    Q.ReadAltitude();
  }
  Q.Encode();
  
  
}
