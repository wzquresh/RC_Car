#include <SPI.h>

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

//#include <nRF24L01.h>
//#include <printf.h>
//#include <RF24.h>
//#include <RF24_config.h>

int up = 2;
int right = 3;
int down = 4;
int left = 5;
int E = 6;
int F = 7;
int joyPush = 8;
int X_Axis = 0;
int Y_Axis = 1;
int buttons[] = {up, right, down, left, E, F, joyPush};

int msg[3];
RF24 radio(9, 10);
const uint64_t RFAddress = 0xE8E8F0F0E1LL;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  for(int i = 0; i<7; ++i){
    pinMode(buttons[i], INPUT);
    digitalWrite(buttons[i], HIGH);
  }
  radio.begin();
  radio.openWritingPipe(RFAddress);
}

void loop() {
  // put your main code here, to run repeatedly:
  //Serial.println(analogRead(Y_Axis));
  //Serial.println(analogRead(X_Axis));

  msg[0] = 111; //New x_axis data is coming
  Serial.print("Data :");
  //Serial.println(analogRead(X_Axis));
  msg[1] = analogRead(X_Axis);
  Serial.println(msg[1]);
  //msg[1] = map(analogRead(X_Axis),0,1000,-5,5);
  //Serial.println("Data :" + msg[1]);
  msg[2] = analogRead(Y_Axis);
  Serial.print("Speed: ");Serial.println(msg[2]);
  radio.write(&msg[0], 6); //sends both type and data
}
