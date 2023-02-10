#include <Arduino.h>
int cambien = 25;// khai báo chân digital 10 cho cảm biến

int Led = 4;//kháo báo chân digital 8 cho đèn LED
void setup() {
  // put your setup code here, to run once:
  pinMode(Led,OUTPUT);//pinMode xuất tín hiệu đầu ra cho led

  pinMode(cambien,INPUT);//pinMode nhận tín hiệu đầu vào cho cảm biê
}

void loop() {
  // put your main code here, to run repeatedly:
  int value = digitalRead(cambien);//lưu giá trị cảm biến vào biến value

  digitalWrite(Led,value);//xuất giá trị ra đèn LED
}