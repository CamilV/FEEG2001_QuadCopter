//  SerialIn_SerialOut_HM-10_01
//
//  Uses hardware serial to talk to the host computer and AltSoftSerial for communication with the bluetooth module
//
//  What ever is entered in the serial monitor is sent to the connected device
//  Anything received from the connected device is copied to the serial monitor
//  Does not send line endings to the HM-10
//
//  Pins
//  BT VCC to Arduino 5V out. 
//  BT GND to GND
//  Arduino D8 (SS RX) - BT TX no need voltage divider 
//  Arduino D9 (SS TX) - BT RX through a voltage divider (5v to 3.3v)
//
#include <AltSoftSerial.h>

AltSoftSerial BTserial;
// https://www.pjrc.com/teensy/td_libs_AltSoftSerial.html
 
 
char c=' ';
char word1[100];
boolean NL = true;
//SoftwareSerial BTserial(7,8);
#define BaudRate 9600
void setup() 
{
    Serial.begin(BaudRate);
    Serial.print("Sketch:   ");   Serial.println(__FILE__);
    Serial.print("Uploaded: ");   Serial.println(__DATE__);
    Serial.println(" ");
    pinMode(2,OUTPUT);
    BTserial.begin(BaudRate);  
    Serial.println("BTserial started at 9600");
}

void loop()
{
    // Read from the Bluetooth module and send to the Arduino Serial Monitor
    int index = 0;
    boolean p = 1;
    while(BTserial.available()>0)
    {
        c = BTserial.read();
        word1[index] = c;
        //if(c == '0') digitalWrite(2,LOW);
        //if(c == '1') digitalWrite(2,HIGH);
        p=0;
        index++;
        delay(2);
    }
    if(p==0){
      Serial.print(word1);
      //Serial.print(index);
      p=1;
    }
 
    // Read from the Serial Monitor and send to the Bluetooth module
    if (Serial.available())
    {
        c = Serial.read();
 
        // do not send line end characters to the HM-10
        
        
             BTserial.write(c);
        
 
        // Echo the user input to the main window. 
        // If there is a new line print the ">" character.
        if (NL) { Serial.print("\r\n>");  NL = false; }
        Serial.write(c);
        if (c==10) { NL = true; }
    }
}
