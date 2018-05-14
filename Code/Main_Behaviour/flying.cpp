#include "QuadCopter.h"
#include "defines.h"
#include <Servo.h>
#include "Maxbotix.h"
//#include <NewPing.h>

//NewPing S(UST,USE,150);
Servo T, G;
Maxbotix US(EXTRA, Maxbotix::PW, Maxbotix::LV);

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
    G.writeMicroseconds(OPEN);
    delay(4000);
    G.writeMicroseconds(CLOSE);
    Grabber = 0;
    dt = 1/SamplingF;
}


void QuadCopter::Encode()   // encodes and sends telemetry data via Bluetooth
{

    //unsigned long ti = millis();
    char str[14];
    str[5] = (char)('0' + State);
    str[6] = (char)('0' + Altitude/100);
    str[7] = (char)('0' + Altitude/10 - Altitude/100*10);
    str[8] = (char)('0' + Altitude%10);
    str[9] = (char)('0' + Throttle/100);
    str[10] = (char)('0' + Throttle/10 - Throttle/100*10);
    str[11] = (char)('0' + Throttle%10);
    str[12] = (char)('0' + AutoPilot);
    str[13] = (char)('0' + Grabber);
    Serial.print(str);
}
void QuadCopter::SmoothAltitude()
{
    unsigned int lastAltitude = Altitude;
    
    Altitude = US.readSensor();
 //   if(Altitude < 14) Altitude = lastAltitude;
 //   if(Altitude > 600) Altitude = lastAltitude;

}

void QuadCopter::ReadAltitude()   // reads altitude of the US sensor
{ 
  unsigned int lastAltitude = Altitude;

  unsigned int duration;
  digitalWrite(UST, LOW);  
  delayMicroseconds(2); 
    
  digitalWrite(UST, HIGH);
  delayMicroseconds(10);
  digitalWrite(UST, LOW);
    
  duration = pulseIn(USE, HIGH, 9000);
  Altitude = duration * 0.034/2;
}


void QuadCopter::PIDThrottle()
{
  int BaseValue, MaxValue;
  if(Takeoff || State)
  {
    if(Takeoff) Throttle = 0;
    Takeoff = 0;
    State = 1;
    Throttle = Throttle + 10;
    if(Altitude > 6 || Throttle > 580) {
      State = 0; 
      MaxValue2 = Throttle + 15; 
      MaxValue1 = Throttle + 10;
      BaseValue2 = Throttle - 12;
      BaseValue1 = Throttle - 32;
    }
  }
  else{
    if(Grabber == 0){BaseValue = BaseValue2;MaxValue = MaxValue2;}   // if the Grabber is closed, it needs more power in order to fly
    else{BaseValue = BaseValue1; MaxValue = MaxValue1;}     // checks the position of the Grabber in order to send the required power to the motors
  
    int Error = Target - Altitude;
    int Error_d = (Error - LastError)/dt;
  
    float Correction = Kp * Error + Kd * Error_d + Ki * Error_i;
  
    if(Error_i > 450) Error_i = 450;
    if(Error_i < -450) Error_i = -450;
  
    if(abs(Error) >= 20) Error_i = Error_i;
    else Error_i = Error_i + dt*Error;
    LastError = Error;
    Throttle = BaseValue + Correction;    // standard PID controller
    if(Throttle > MaxValue) Throttle = MaxValue;
  }
  if(Throttle < 0)   Throttle = 0;      // caps the values of the throttle, in order to make sure we dont send to much or too little power to the flight controller

  int Th = map(Throttle, 0, 999, MinThrottlePulse, MaxThrottlePulse); // remaps the throttle signal from (0,999) to (1000,2000), to be sent as a pulse lenght in microseconds to the flight controller
  T.writeMicroseconds(Th);
}


void QuadCopter::drop()
{
  G.writeMicroseconds(OPEN);
  Grabber = 1;
}

