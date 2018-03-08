#include "QuadCopter.h"
#include "defines.h"
#include <Servo.h>
//#include <NewPing.h>

//NewPing S(UST,USE,150);
Servo T, G;


void QuadCopter::Initialize()  // initializing all the necesary outputs/inputs
{
    Serial.begin(9600);
    pinMode(ThO, OUTPUT);
    pinMode(ThS, OUTPUT);
    pinMode(UST, OUTPUT);
    pinMode(USE,INPUT);
    pinMode(SRV, OUTPUT);
    pinMode(RXIN, INPUT);
    Grabber = 1;
    T.attach(ThO);
    G.attach(SRV);
    G.write(CLOSE);
    Grabber = 0;
}


void QuadCopter::Encode()   // encodes and sends telemetry data via Bluetooth
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
    //Serial.print(str);
    //Serial.print("Altitude: "); Serial.println(Altitude);
}
void QuadCopter::SmoothAltitude()
{
    unsigned int lastAltitude = Altitude;
    unsigned int duration;
    digitalWrite(UST, LOW);  
    delayMicroseconds(2); 
    
    digitalWrite(UST, HIGH);
    delayMicroseconds(10);
    digitalWrite(UST, LOW);
    
    duration = pulseIn(USE, HIGH, 9000);
    TrueAltitude = duration * 0.034/2;
    lastDuration = W/100.0 * duration + (100-W)*lastDuration/100.0;
    Altitude = lastDuration * 0.034/2;
    if(Altitude == 0) Altitude = lastAltitude;
    //Serial.print(Altitude);Serial.print("  ");
    //Serial.println(int(duration * 0.034/2));
}

void QuadCopter::ReadAltitude()   // reads altitude of the US sensor
{ 
  unsigned int lastAltitude = Altitude;
  unsigned int i, no = 0, a[3], sum = 0;
  unsigned int duration;
  
  for(i=0;i<3;i++){
    digitalWrite(UST, LOW);  
    delayMicroseconds(2); 
    
    digitalWrite(UST, HIGH);
    delayMicroseconds(10);
    digitalWrite(UST, LOW);
    
    duration = pulseIn(USE, HIGH, 9000);
    
    if(duration != 0){
      no++;
      sum = sum + duration;
    }
  }
  if(no == 0) Altitude = lastAltitude;
  else Altitude = sum/no * 0.034/2;
  
}


void QuadCopter::PIDThrottle()
{
  int BaseValue, MaxValue;
  
  if(Grabber == 1){BaseValue = BaseValue2; MaxValue = MaxValue2;}   // if the Grabber is open, it needs more power in order to fly
  else{BaseValue = BaseValue1; MaxValue = MaxValue1;}     // checks the position of the Grabber in order to send the required power to the motors
  
  int Error = Target - Altitude;
  float Correction = Kp * Error + Kd * (Error - LastError);
  LastError = Error;
  Throttle = BaseValue + Correction;    // standard PID controller, that doesnt use the Integral part
  
  if(Throttle > MaxValue) Throttle = MaxValue;
  if(Throttle < 0)   Throttle = 0;      // caps the values of the throttle, in order to make sure we dont send to much or too little power to the flight controller
  
  int Th = map(Throttle, 0, 999, MinThrottlePulse, MaxThrottlePulse); // remaps the throttle signal from (0,999) to (1000,2000), to be sent as a pulse lenght in microseconds to the flight controller
  T.writeMicroseconds(Th);
}


void QuadCopter::drop()
{
  G.write(OPEN);
  Grabber = 0;
}

