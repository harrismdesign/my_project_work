#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define SERVOMIN  125 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  575 // this is the 'maximum' pulse length count (out of 4096)

uint8_t servonum = 0; //for PWMservo library

const int LFT = 0; // start 90 // rest 135 // step forward towards 90
const int LFB = 1; // start 0 // rest 90 // lift to 135 (q)// drop to 65 in forward step

const int RFT = 3; // start 90 // rest 45 // step forward towards 90
const int RFB = 4; // start 180 // rest 90 // lift to 45 (w) // drop to 114 in forward step

const int LBT = 6; // start 90 // rest 135 // step forward towards 90
const int LBB = 7; // start 0 // rest 90 // lift to 135 (e)// drop to 65 in forward step

const int RBT = 9; // start 90 // rest 45 // step forward towards 90
const int RBB = 10; // start 180 // rest 90 // lift to 45 (r)// drop to 114 in forward step

int set_control = 0; // if this is 1 the robot will not repeat phase_1
void setup() {
  Serial.begin(9600);
  Serial.println("Quad Robot 2.0");

  pwm.begin();
  
  pwm.setPWMFreq(60);

  //Position(LFT, 90); // these positions set the legs in a straight position, 
  //Position(LFB, 0);  // making the assembly easy
 //Position(RFT, 90);
  //Position(RFB, 180);
  //Position(LBT, 90);
  //Position(LBB, 0);
  //Position(RBT, 90);
  //Position(RBB, 180);
  
  }

void loop() {
   char incomingByte;
  if (Serial.available() > 0) {
   
incomingByte = (char)Serial.read(); // reads a character from the serial port 
Serial.println(incomingByte);
  }
 if (incomingByte == 'x') {
    Serial.println("Reset"); 
   Position(LFT, 90); // these positions set the legs in a straight position, 
  Position(LFB, 0);  // making the assembly easy
  Position(RFT, 90);
  Position(RFB, 180);
  Position(LBT, 90);
  Position(LBB, 0);
  Position(RBT, 90);
  Position(RBB, 180);
  set_control = 0;
    }  
 
 if (incomingByte == 'z') {
    Serial.println("Set"); 
  Position(LFT, 135); // these positions ready the robot 
  Position(LFB, 90);  
  Position(RFT, 45);
  Position(RFB, 90);
  Position(LBT, 155);
  Position(LBB, 80); //
  Position(RBT, 40);
  Position(RBB, 90);

  set_control = 0;
    } 

if (incomingByte == 'q') {
    Serial.println("LF lift"); 
    Position(LFB, 135);
    }

if (incomingByte == 'w') {
    Serial.println("RF lift"); 
    Position(RFB, 45);
    } 

if (incomingByte == 'e') {
    Serial.println("LB lift"); 
    Position(LBB, 135);
    } 

if (incomingByte == 'r') {
    Serial.println("RB lift"); 
    Position(RBB, 45);
}          

if (incomingByte == 'a') {
    Serial.println("Step"); 
  Phase_1();
  Phase_2();
  settogo();
  }
    
if (incomingByte == 's') {
    Serial.println("walk_1"); 
  Phase_1(); } 

if (incomingByte == 'd') {
    Serial.println("walk_2"); 
  Phase_2(); } 
       

}

void Position(int number, int degree) {      // takes a desired servo and angle 
int pospwm = map(degree, 0, 180, 125, 575);  //and uses the PWM servo library to execute
pwm.setPWM(number, 0, pospwm );
delay(5);
}

void Phase_1 () // step LF and RB forwards only, LFT-135-90, LFB-90-135-65
{               //RBT-45-90, RBB-90-45-114
int P1 = 0;
int P1_control = 0; //the function stops when this 
while(P1_control ==0){
for(P1 = 0; P1 < 9; P1 +=1){
Position(LFT,135-(2.5*P1));
Position(RBT,45+(2.5*P1));
Position(LFB,90+(3*P1));
Position(RBB,90-(3*P1));

delay(2);
}
for(P1 = 9; P1 > 0; P1 -=1){
Position(LFT,90+(2.5*P1));
Position(RBT,90-(2.5*P1));
Position(LFB,60+(6*P1));
Position(RBB,115-(6*P1));  
delay(2);
}
P1_control = 1;
}}  

void Phase_2 () 
{               
int P2 = 0;
int P2_control = 0; 
while(P2_control ==0){
for(P2 = 0; P2 < 9; P2 +=1){
Position(LBT,135-(2.5*P2));
Position(RFT,45+(2.5*P2));
Position(LBB,90+(3*P2));
Position(RFB,90-(3*P2));
Position(LFT,90+(2.5*P2));
Position(LFB,60+(1.6*P2));
Position(RBT,90-(2.5*P2));
Position(RBB,115-(1.4*P2));

delay(2);
}
for(P2 = 9; P2 > 0; P2 -=1){
Position(LBT,90+(2.5*P2));
Position(RFT,90-(2.5*P2));
Position(LBB,45+(7*P2));
Position(RFB,115-(6*P2)); 
Position(LFT,135-(2.5*P2));
Position(LFB,90-(1.6*P2));
Position(RBT,45+(2.5*P2));
Position(RBB,90+(1.4*P2)); 
delay(2);
}
P2_control = 1;
}}



void Phase_3 () //step LF and RB forwards, while pulling back RF and LB
                //LFT-135-90, LFB-90-135-65, RBT-45-90, RBB-90-45-114
{               //LBT-90-135,LBB-65-90,RFT-90-45,RFB-114-90.             
int P3 = 0;
int P3_control = 0; //the function stops when this 1
while(P3_control ==0){
for(P3 = 0; P3 < 9; P3 +=1){
Position(LFT,135-(2.5*P3));
Position(RBT,45+(2.5*P3));
Position(LFB,90+(3*P3));
Position(RBB,90-(3*P3));

delay(2);
}
for(P3 = 9; P3 > 0; P3 -=1){
Position(LFT,90+(2.5*P3));
Position(RBT,90-(2.5*P3));
Position(LFB,60+(6*P3));
Position(RBB,115-(6*P3));  
delay(2);
}
P3_control = 1;
}}  

void settogo(){
  Position(LFT, 135); 
  Position(LFB, 90);  
  Position(RFT, 45);
  Position(RFB, 90);
  Position(LBT, 155);
  Position(LBB, 80); 
  Position(RBT, 40);
  Position(RBB, 90);
  
  
}
