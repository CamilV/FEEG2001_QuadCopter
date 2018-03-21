#ifndef DEFINES_H_INCLUDED
#define DEFINES_H_INCLUDED

// pins
#define IRSensor A0   // InfraRed Sensor
#define ThO      12   // Arduino Throttle Output
#define ThS      11   // Throttle Switch
#define UST       8   // UltraSonic Sensor TRIGGER Pin
#define USE       7   // UltraSonic Sensor ECHO Pin
#define EXTRA     4   // extra pin if we ever need something like a button input
#define RXIN      6   // Receiver channel
#define SRV       3   // Servo Signal Pin
// reading
#define W        33   // sensor weighting, higher values favour new values
// pulse lenghts
#define ReceiverThrottle 1500  // pulse lenght above which the arduino changes to receiver throttle
#define MaxThrottlePulse 2000
#define MinThrottlePulse 1000
// frequencies
#define SamplingF        10     // this should be tested with different values for the sensor weighting
#define TelemetryF        5
// Throttle(0,999)
#define BaseValue1         300  // theoretic value to hover at the required altitude without payload, needs testing
#define BaseValue2         400  // theoretic value to hover at the required altitude with payload, needs testing
#define MaxValue1          600  // theoretic max climbing value without payload, needs testing
#define MaxValue2          700  // theoretic max climbing value with payload, needs testing

// Servo
#define OPEN   75
#define CLOSE   0

// PID constants
#define Kp       5
#define Kd       3
#define Ki     0.3
#define Target  80
#define aError  10

#endif
