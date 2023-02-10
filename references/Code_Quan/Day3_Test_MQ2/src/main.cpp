#include <Arduino.h>

int LED = 32;            /*LED pin defined*/
int Sensor_input =15;    /*Digital pin 5 for sensor input*/

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);  /*baud rate for serial communication*/
  pinMode(LED, OUTPUT);  /*LED set as Output*/
}

void loop() {
  // put your main code here, to run repeatedly:
  int sensor_Aout = analogRead(Sensor_input);  /*Analog value read function*/
  Serial.print("Gas Sensor: ");  
  Serial.print(sensor_Aout);   /*Read value printed*/
  Serial.print("\t");
  Serial.print("\t");
  if (sensor_Aout > 1800) {    /*if condition with threshold 1800*/
    Serial.println("Gas");  
    digitalWrite (LED, HIGH) ; /*LED set HIGH if Gas detected */
  }
  else {
    Serial.println("No Gas");
    digitalWrite (LED, LOW) ;  /*LED set LOW if NO Gas detected */
  }
  delay(1000);                 /*DELAY of 1 sec*/
}