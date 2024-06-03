#include <EEPROM.h>
const int motorAdir = 7; 
const int motorApwm = 6; 
const int motorBdir = 4; 
const int motorBpwm = 5; 



void setup() {
 pinMode(motorAdir, OUTPUT);
  pinMode(motorApwm, OUTPUT);
  pinMode(motorBdir, OUTPUT);
  pinMode(motorBpwm, OUTPUT);
  analogWrite(motorApwm, 255);
  analogWrite(motorBpwm, 255);
}

int geneLength = 42;
int maxim = 255;
void controlMotors(int eep_num, int i) {
      int rem = i%6;
      switch(rem){
        case 0:
          if(eep_num > maxim/2.0){
            analogWrite(motorApwm, 255);
          }else{
            analogWrite(motorApwm, 0);
          }
          break;
        case 1:
          if(eep_num > maxim/2.0){
            digitalWrite(motorAdir, HIGH);
          }else{
            digitalWrite(motorAdir, LOW);
          }
          break;
        case 2:
          delay(eep_num);
          analogWrite(motorApwm, 0);
          delay(maxim - eep_num);
          break;
        case 3:
          if(eep_num > maxim/2.0){
            analogWrite(motorBpwm, 255);
          }else{
            analogWrite(motorBpwm, 0);
          }
          break;
        case 4:
          if(eep_num > maxim/2.0){
            digitalWrite(motorBdir, HIGH);
          }else{
            digitalWrite(motorBdir, LOW);
          }
          break;
        case 5:
          delay(eep_num);
          analogWrite(motorBpwm, 0);
          delay(maxim - eep_num);
          break;
      }
}
void antiControlMotors(int eep_num, int i){
  int rem = i%6;
      switch(rem){
        case 0:
          if(eep_num > maxim/2.0){
            analogWrite(motorBpwm, 255);
          }else{
            analogWrite(motorBpwm, 0);
          }
          break;
        case 1:
          if(eep_num > maxim/2.0){
            digitalWrite(motorBdir, HIGH);
          }else{
            digitalWrite(motorBdir, LOW);
          }
          break;
        case 2:
          delay(eep_num);
          analogWrite(motorBpwm, 0);
          delay(maxim - eep_num);
          break;
        case 3:
          if(eep_num > maxim/2.0){
            analogWrite(motorApwm, 255);
          }else{
            analogWrite(motorApwm, 0);
          }
          break;
        case 4:
          if(eep_num > maxim/2.0){
            digitalWrite(motorAdir, HIGH);
          }else{
            digitalWrite(motorAdir, LOW);
          }
          break;
        case 5:
          delay(eep_num);
          analogWrite(motorApwm, 0);
          delay(maxim - eep_num);
          break;
      }
}

void loop() {
 for(int ibex = 0; ibex < geneLength; ibex++){
    controlMotors(EEPROM.read(ibex), ibex);
    antiControlMotors(EEPROM.read(ibex), ibex);
 }

}
