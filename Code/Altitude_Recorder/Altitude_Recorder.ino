#include <SPI.h>
#include<SD.h>
#include "defines.h"
#include "QuadCopter.h"

QuadCopter Q;
const int chipSelect = EXTRA;
int noVotes, voteFor;
unsigned long t;

void setup() {
  Serial.begin(9600);
  SD.begin(chipSelect);
  Q.Initialize();
  digitalWrite(ThS,LOW);
}

void loop() {
  delay(86);
  String data = "";
  Q.SmoothAltitude();
  Q.PIDThrottle();
  unsigned long ti = millis();
  Serial.println(ti);
  data += String(ti);
  data += ",";
  data += String(Q.TrueAltitude);
  data += ",";
  data += String(Q.Altitude);
  data += ",";
  data += String(Q.Throttle);

  File dataFile = SD.open("datalog.txt",FILE_WRITE);
  dataFile.println(data);
  dataFile.close();
    
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
