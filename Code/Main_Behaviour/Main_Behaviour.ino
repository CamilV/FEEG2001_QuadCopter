#include "QuadCopter.h"
#include <EnableInterrupt.h>

QuadCopter Q;
int noVotes, voteFor;


void Switch(){      // function that calculates the pulse lenght and switches the autopilot on or off
  if (digitalRead(RXIN) == HIGH) {
    Q.StartPulse = micros();      // starts timer as soon as the pin goes HIGH
  } 
  else {
    noInterrupts();
    int PulseLength = micros() - Q.StartPulse;    // stops timer when the pin goes LOW
    if(PulseLength > ReceiverThrottle){           // checks the lenght of the pulse
      digitalWrite(ThS, HIGH);                    // long pulse (1 on the remote) turns the autopilot on
      Q.AutoPilot = 1;
    }
    else {
      digitalWrite(ThS, LOW);           // short pulse (0 on the remote) turns the autopilot off
      Q.AutoPilot = 0;
    }
    interrupts();
  }
}


void setup() {
  Q.Initialize();
  enableInterrupt(RXIN, Switch, CHANGE);    // creates the event Switch() which is called whenever the RXIN pin CHANGEs state
  //delay(5000);
}

unsigned long t;
void loop() {  
  //Q.ReadAltitude();
  Q.SmoothAltitude();
  Q.PIDThrottle();
  
  if(Q.Altitude < (Target + aError / 2) && Q.Altitude > (Target - aError/2)) voteFor++;     // voting system to verify the altitude
  noVotes++;
  
  if(voteFor >= 800) Q.drop();                 // if we have enough votes that say we are at the target altitude, it opens the grabber
  
  if(noVotes == 1000){                          // if too many votes have passed, we reset the two counters and start all over again
    noVotes == 0; voteFor == 0;
  }
  if(millis() - t > 500){
    Q.Encode();  // every 500 ms we send data via bluetooth
    t = millis();
  }
}
