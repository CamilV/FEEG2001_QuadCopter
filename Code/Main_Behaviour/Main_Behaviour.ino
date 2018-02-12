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
  Q.ReadAltitude();
  //Serial.print("altitude: ");Serial.println(Q.Altitude);
  delay(500);
}
