#include <SPI.h>                            // инклюдим библиотеку для аппаратного SPI

#define CS 10                               // нога чипслекет        

 
void setup() { 
  Serial.begin(115200);
  SPI.begin();    
  pinMode (CS, OUTPUT);   
}
 
void loop() {
  int val=0;
  for (int val=0; val <= 255; val++){
    val=val+1; 
    //val=val+255;
    MCP4xxxxWrite(val);                         // пишем значение переменной в потенциометр
    Serial.println(val, DEC);
    delay(50);                                 // ждем до следующего раза
 
}
}
 
void MCP4xxxxWrite(byte val) {              // отправляет в потенциометр значение ползунка
    digitalWrite(CS, LOW);                    // включаем прием данных микросхемой
    SPI.transfer(0b00010001);                 // отправляем первый байт в регистр конфигурации
    SPI.transfer(val);                        // отправляем второй байт в "регистр ползунка"
    digitalWrite(CS,HIGH);                    // выключаем прием данных микросхемой
}
