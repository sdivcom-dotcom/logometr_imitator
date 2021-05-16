// inslude the SPI library:
#include <SPI.h>
 
// set pin 10 as the slave select for the digital pot:
const int slave_Select_Pin  = 10;
const int analogInPin0      = A0; 
const int analogInPin1      = A1; 
String    inputString       = "";         // a string to hold incoming data
boolean   stringComplete    = false;      // whether the string is complete
int       level1            = 0;
int       level2            = 0;
 
void setup() {
     inputString.reserve(100);
     // set the slaveSelectPin as an output:
     pinMode (slave_Select_Pin, OUTPUT);
     Serial.begin(115200);
 
     // initialize SPI:
     SPI.begin();
     MSP42010PotWrite(slave_Select_Pin, B00010001, level1);
     MSP42010PotWrite(slave_Select_Pin, B00010010, level2);
}
 
void loop() {
  //Serial.print("!!!!!!!!");
  if (stringComplete) {
    //check ob R1:
    if (inputString.substring(0, 2) == "1:") {
      level1 = inputString.substring(2).toInt();
      MSP42010PotWrite(slave_Select_Pin, B00010001, level1);     
      printValues(level1, analogInPin0);
    }
    //check ob R2:
    if (inputString.substring(0, 2) == "2:") {
      level2 = inputString.substring(2).toInt();
      MSP42010PotWrite(slave_Select_Pin, B00010010, level2); //Datasheet Page 18
      printValues(level2, analogInPin1);
    }
    //check ob s
    if (inputString.substring(0, 1) == "s") {
      printValues(level1, analogInPin0);
      printValues(level2, analogInPin1);
    }
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
}
 
void MSP42010PotWrite(int slaveSelectPin, byte address, int value) {
     // take the SS pin low to select the chip:
     digitalWrite(slaveSelectPin,LOW);
     //  send in the address and value via SPI:
     SPI.transfer(address);
     SPI.transfer(value);
     // take the SS pin high to de-select the chip:
     digitalWrite(slaveSelectPin,HIGH);
}
void printValues(int level, int aPin) {
      delay(5);
      int pot = 0;
      if (aPin == 15) {
        pot = 1;
      }
      Serial.print("level Pot");
      Serial.print(pot);
      Serial.print(": ");
      Serial.print(level);
      Serial.print(" Spannung an A");
      Serial.print(pot);
      Serial.print(": ");
      double sl = analogRead(aPin);
      sl = sl * 5 / 1024; 
      Serial.print(sl);
      Serial.println(" Volt");  
}
/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    } else {
      inputString += inChar;
    }
  }
}
