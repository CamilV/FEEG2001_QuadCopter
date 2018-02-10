#include "QuadCopter.h"


QuadCopter Q;

void setup() {
  Q.Initialize();

}

void loop() {
  Q.ReadAltitude();
  Serial.print(Q.Altitude); Serial.println(" cm");
}
