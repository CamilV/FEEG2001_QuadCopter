#ifndef DEFINES_H_INCLUDED
#define DEFINES_H_INCLUDED

// pins
#define IRSensor A0   // InfraRed Sensor
#define ThO      12   // Arduino Throttle Output
#define ThS      11   // Throttle Switch
#define UST       8   // UltraSonic Sensor TRIGGER Pin
#define USE       7   // UltraSonic Sensor ECHO Pin
#define RXIN      4   // Receiver channel
#define SRV       3   // Servo Signal Pin

// reading IR
#define NO_ITERATIONS 5 // number of reading of IR sensor before calculating an average
#define THRESHOLD     7 // cm drift from the average after the reading is considered an error

// pulse lenghts
#define ReceiverThrottle 1500  // pulse lenght above which the arduino changes to receiver throttle
#define MaxThrottlePulse 2000
#define MinThrottlePulse 1000

// Throttle
#define BaseValue         300  // max throttle is 999, theoretic value to hover at the required altitude, needs testing

// PID constants
#define Kp      7
#define Kd      8
#define Target 80

#endif
