#include "QuadCopter.h"
#include "defines.h"
#include <Wire.h>
#include <Servo.h>


//int QuadCopter::conversion_ADC_to_cm(int a) // converting ADC reading into cm
//{
//  return 472280 * pow(a,-1.665);
//}

void QuadCopter::Initialize()  // initializing all the necesary outputs/inputs
{
    Serial.begin(9600);
    pinMode(ThO, OUTPUT);
    pinMode(ThS, OUTPUT);
    pinMode(UST, OUTPUT);
    pinMode(SRV, OUTPUT);
    pinMode(RXIN, INPUT);
    pinMode(USE, INPUT);
    
}

void QuadCopter::Encode()   // encodes telemetry data, in order to be sent via Bluetooth
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

void QuadCopter::ReadAltitude()   // reads altitude of the IR sensor
{
  // ir sensor is not accurate(consistent) enough, ultrasonic sensor runs fast enough for us and gives us more accurate(consistent) readings
  /*
  int IR[NO_ITERATIONS], i, AVG=0, ALT[NO_ITERATIONS];
  for(i=0; i<NO_ITERATIONS; i++)              // calculates the average of NO_ITERATIONS readings
  {
    IR[i] = analogRead(IRSensor);
    ALT[i] = conversion_ADC_to_cm(IR[i]);
    AVG = AVG + ALT[i] / NO_ITERATIONS;
    
  }
  int j=0;
  int BAVG = AVG;
  for(i=0;i<NO_ITERATIONS;i++)          // checks if any values are far from the calculated average and eliminates those
    if(ALT[i] > (AVG + THRESHOLD) || ALT[i] < (AVG - THRESHOLD))
    {
        AVG = AVG - ALT[i]/NO_ITERATIONS;
        j++;
    }
  if(j != NO_ITERATIONS)
    AVG = AVG * NO_ITERATIONS / (NO_ITERATIONS - j);
  else
    AVG = BAVG;                   // fool-proof so it doesn't eliminate all the readings from the average
   // IR reading (all 5) takes about 2-3 ms
  */
  int dist, dur;
  digitalWrite(UST, LOW);
  delayMicroseconds(2);

  digitalWrite(UST, HIGH);
  delayMicroseconds(10);
  digitalWrite(UST, LOW);

  dur = pulseIn(USE, HIGH, 9000);

  Altitude = dur*0.034/2;   // UltraSonic sensor takes 6-7 ms to run
  //Altitude = (AVG+dist)/2;  // eventually after the US sensor implementation
               // just for debugging as the US sensor isn't yet connected
}

void QuadCopter::PIDThrottle()
{
  int Error = Target - Altitude;
  float Correction = Kp * Error + Kd * (Error - LastError);
  LastError = Error;
  Throttle = BaseValue + Correction;
  if(Throttle > 999) Throttle = 999;
  if(Throttle < 0)   Throttle =   0;
}

