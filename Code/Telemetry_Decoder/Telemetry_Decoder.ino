#include <SoftwareSerial.h>
#include <rExcel.h>
rExcel myExcel;
SoftwareSerial BTSerial(9,8); //rx tx
char str[9];
int idx = 0;
char* SpreadSheet = "Quad";
void setup() {
  Serial.begin(9600);
  BTSerial.begin(9600);
  myExcel.clearInput();
  myExcel.newss(SpreadSheet);
  delay(100);
  myExcel.writeIndexed(SpreadSheet, idx+1, 1, "Time");
  myExcel.writeIndexed(SpreadSheet, idx+1, 2, "Altitude");
  myExcel.writeIndexed(SpreadSheet, idx+1, 3, "Throttle");
  myExcel.writeIndexed(SpreadSheet, idx+1, 4, "AutoPilot");
  myExcel.writeIndexed(SpreadSheet, idx+1, 5, "Grabber");
}

void loop() {
  int index = 0;
  if(BTSerial.available()>0){
    while(BTSerial.available()>0){
    char c = BTSerial.read();
    str[index] = c;
    index++;
    delay(10);
    }
    
    int alt = 100 * ((int)str[0]-'0') + 10 * ((int)str[1]-'0') + ((int)str[2]-'0');
    int th = 100 * ((int)str[3]-'0') + 10 * ((int)str[4]-'0') + ((int)str[5]-'0');
    int AutoPilot = (int)str[6]-'0';
    int Grabber = (int)str[7]-'0';
    myExcel.writeIndexed(SpreadSheet, idx+2, 1, "%time%");
    myExcel.writeIndexed(SpreadSheet, idx+2, 2, alt);
    myExcel.writeIndexed(SpreadSheet, idx+2, 3, th);
    myExcel.writeIndexed(SpreadSheet, idx+2, 4, AutoPilot);
    myExcel.writeIndexed(SpreadSheet, idx+2, 5, Grabber);
    idx++;
    /*
    Serial.print("Altitude: ");Serial.println(alt);
    Serial.print("Throttle: ");Serial.println(th);
    Serial.print("AutoPilot: ");Serial.println(AutoPilot);
    Serial.print("Servo: ");Serial.println(Grabber);
    */
  }
  delay(500);
}
