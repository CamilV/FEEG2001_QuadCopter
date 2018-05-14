#include "QuadCopter.h"
#include <EnableInterrupt.h>

QuadCopter Q;
int noVotes, voteFor;
int PulseLength[3];

void Switch(){      // function that calculates the pulse lenght and switches the autopilot on or off
  if (digitalRead(RXIN) == HIGH) {
    Q.StartPulse = micros();      // starts timer as soon as the pin goes HIGH
  } 
  else {
    noInterrupts();
    Q.pAutopilot = Q.AutoPilot;
    PulseLength[0] = PulseLength[1];
    PulseLength[1] = PulseLength[2];
    PulseLength[2] = micros() - Q.StartPulse;    // stops timer when the pin goes LOW
    int i;
    bool on=0,off=0;
    for(i=0;i<=2;i++)
    {
      if(PulseLength[i] > ReceiverThrottle) on=1;
      if(PulseLength[i] < ReceiverThrottle) off=1;
    }
    if(on && !off){           // checks the lenght of the pulse
      if (!Q.pAutopilot) {Q.Takeoff = 1; Q.Throttle = 0;}
      digitalWrite(ThS, HIGH);                    // long pulse (0 on the remote) turns the autopilot on
      Q.AutoPilot = 1;
    }
    if(!on && off) {
      digitalWrite(ThS, LOW);           // short pulse (1 on the remote) turns the autopilot off
      Q.AutoPilot = 0;
    }
    interrupts();
  }
}


void setup() {
  Q.Initialize();
  enableInterrupt(RXIN, Switch, CHANGE);    // creates the event Switch() which is called whenever the RXIN pin CHANGEs state
  delay(5000);
}

unsigned long PreviousMillisT=0, PreviousMillisS=0;
void loop() {  
  unsigned long t = millis();
  if(t - PreviousMillisS > 1000/SamplingF){
    if(Q.Altitude > 30) Q.SmoothAltitude();
    else Q.ReadAltitude();
    Q.PIDThrottle();
    //Serial.println(Q.Altitude); 
    if(Q.Altitude < (Target + aError) && Q.Altitude > (Target - aError)) voteFor++;     // voting system to verify the altitude
    noVotes++;
  
    if(voteFor >= 10) Q.drop();                 // if we have enough votes that say we are at the target altitude, it opens the grabber
  
    if(noVotes == 20){                          // if too many votes have passed, we reset the two counters and start all over again
      noVotes == 0; voteFor == 0;
    }
  PreviousMillisS = millis();  
  }
  if(t - PreviousMillisT > 1000/TelemetryF){
    Q.Encode();  // every 500 ms we send data via bluetooth
    PreviousMillisT = millis();
  }
}
