#include <Adafruit_MotorShield.h>

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
//#include <AFMotor.h> //Deprecated Library

int msg[3];
RF24 radio(9, 10);
const uint64_t RFAddress = 0xE8E8F0F0E1LL;
int pipe = 1;

//Unused PWM Pins: 5, 6
//Note for Addressing: Do Not Use 0x60-0x80, this is
//reserved for the shield communication.
//Adafruit shield V2+ use i2C pins: A4 and A5 on Uno/Nano

//AF_DCMotor Left_Motor(1, MOTOR12_64KHZ);
//AF_DCMotor Right_Motor(2, MOTOR12_64KHZ);
Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x60);

Adafruit_DCMotor *Left_Motor = AFMS.getMotor(3);
Adafruit_DCMotor *Right_Motor = AFMS.getMotor(4);

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(pipe, RFAddress);
  radio.startListening();
  AFMS.begin();
  Left_Motor->setSpeed(150);
  Right_Motor->setSpeed(150);
  
  //Left_Motor->run(RELEASE);
  //Right_Motor->run(RELEASE);
  //delay(5000);
  Serial.println("Start");
}

void loop() {
  Serial.println("Test");
  //Left_Motor->run(FORWARD);
  
  if (radio.available()) {
    Serial.println("Available");
    bool done = false;
    while (!done) {
      done = radio.available();
      radio.read(&msg[0],6);
      Serial.print("Type: ");Serial.println(msg[0]);
      Serial.print("MSG1: ");Serial.println(msg[1]);
      Serial.print("MSG2: ");Serial.println(msg[2]);
      if (msg[0] == 111) {
        if( msg[1] > 500 && msg[1] < 515 && msg[2]>500 &&msg[2]<515 ){
          //stop
          Serial.println("STOPA!!!");
          Left_Motor->run(RELEASE);
          Right_Motor->run(RELEASE);
        }
        if(msg[2] > 515){
          //forward
          if( msg[1] > 530){
            //f_right
            Left_Motor->run(FORWARD);
            Right_Motor->run(BACKWARD);
          }
          else if( msg[1] < 485){
            //f_left
            Left_Motor->run(BACKWARD);
            Right_Motor->run(FORWARD);
          }
          else{
            Left_Motor->run(FORWARD);
            Right_Motor->run(FORWARD);
          }
        }
        
        if( msg[2] < 500 ){
          //back
          if(  msg[1] > 530 ){
            //b_right
            Left_Motor->run(BACKWARD);
            Right_Motor->run(FORWARD);
          }
          else if( msg[1] < 485){
            //b_left
            Left_Motor->run(FORWARD);
            Right_Motor->run(BACKWARD);
          }
          else{
            Left_Motor->run(BACKWARD);
            Right_Motor->run(BACKWARD);
          }
        }        
      }
    }
  }else{
    Serial.println("No Radio Available");
  }
}
