#include <AltSoftSerial.h>
#include <rExcel.h>
rExcel myExcel;

AltSoftSerial BTSerial;
char str[100];
int idx = 0;
char* SpreadSheet = "Quad";
void setup() {
  Serial.begin(9600);
  BTSerial.begin(9600);
  BTSerial.println("AT+INQ");
  delay(5000);
  BTSerial.println("AT+CONN1");
  myExcel.clearInput();
  //myExcel.newss(SpreadSheet);
  //delay(1000);
  myExcel.writeIndexed(SpreadSheet, idx+1, 1, "Time");
  myExcel.writeIndexed(SpreadSheet, idx+1, 2, "Altitude");
  myExcel.writeIndexed(SpreadSheet, idx+1, 3, "Throttle");
  myExcel.writeIndexed(SpreadSheet, idx+1, 4, "AutoPilot");
  myExcel.writeIndexed(SpreadSheet, idx+1, 5, "Grabber");
}
char c=' ';
boolean p = 1;
void loop() {
  int index = 0;
  while(BTSerial.available()>0){
    c = BTSerial.read();
    str[index] = c;
    index++;
    delay(2);
    p=0;
   
  }
    if(p==0){
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
    
    Serial.print("Altitude: ");Serial.println(alt);
    Serial.print("Throttle: ");Serial.println(th);
    Serial.print("AutoPilot: ");Serial.println(AutoPilot);
    Serial.print("Servo: ");Serial.println(Grabber);
    p=1;
  }

}
