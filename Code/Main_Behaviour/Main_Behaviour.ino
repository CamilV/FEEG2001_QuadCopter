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
    if(Q.PulseLength > ReceiverThrottle) {digitalWrite(ThS, HIGH); Q.AutoPilot = 0;}
    else {digitalWrite(ThS, LOW); Q.AutoPilot = 1;}
    interrupts();
  }
}

void setup() {
  Q.Initialize();
  enableInterrupt(RXIN, Switch, CHANGE);
}

void loop() {
  int t = millis();
  Q.ReadAltitude();
  
  //Serial.print("altitude: "); Serial.println(Q.Altitude);
  Serial.print("duration: "); Serial.println(millis()-t);
}
