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
      Q.drop(CLOSE);                   // long pulse (0 on the remote) turns the autopilot on
      //Q.AutoPilot = 1;
      Serial.println(Q.Grabber);
    }
    else {
      Q.drop(OPEN);          // short pulse (1 on the remote) turns the autopilot off
      //Q.AutoPilot = 0;
      Serial.println(Q.Grabber);
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

//void loop() {  
//  unsigned long t = millis();
//  if(t - PreviousMillisS >= 1000/SamplingF){
//    Q.SmoothAltitude();
//    Q.PIDThrottle();
//  
//    if(Q.Altitude < (Target + aError) && Q.Altitude > (Target - aError)) voteFor++;     // voting system to verify the altitude
//    noVotes++;
//  
//    if(voteFor >= 30) Q.drop(OPEN);                 // if we have enough votes that say we are at the target altitude, it opens the grabber
//  
//    if(noVotes == 50){                          // if too many votes have passed, we reset the two counters and start all over again
//      noVotes == 0; voteFor == 0;
//    }
//  PreviousMillisS = t;  
//  }
//  if(t - PreviousMillisT >= 1000/TelemetryF){
//    Q.Encode();  // every 500 ms we send data via bluetooth
//    PreviousMillisT = t;
//  }
//}
void loop(){
  Q.drop(80);
  delay(2000);
  Q.drop(100);
  delay(2000);
}

