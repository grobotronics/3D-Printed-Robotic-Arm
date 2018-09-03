#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
//Servo pins on Servo Driver 
int Up_down=1; 
int Forward_backward=3;
int Rotate=2;
int Grab=0;

//-----------------------------------
const int startBT1 =11;
const int teachBT1 = 12;
const int buzzer1 = 8;
const int gr1=9;
const int re1=10;
const int potForwardBackward1 = A0;
const int potUpDown1 = A1;
const int potRotate1 = A2;
const int potGrab1 = A3;

//-----------------------------------
//Max and Min values for servos ! Change them to meet your setup !
const int minGrab=50;//150
const int maxGrab=300;//300
const int minRotate=600;//300
const int maxRotate=300;//200
const int minUpDown=300;//300
const int maxUpDown=150;//150
const int minForwardBackward=100;//100
const int maxForwardBackward=350;//200
//-----------------------------------
int ReadUpDown,ReadForwardBackward,ReadRotate,ReadGrab,ReadTeach,ReadStart,readpotsw11,readpotsw21;
int TeachUpDown[100],TeachForwardBackward[100],TeachRotate[100],TeachGrab[100];
boolean Started =false;
int Index = 1;
int StepSpeed = 5; //Change this to fo faster!

void setup() {
  pwm.begin();
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
  Wire.setClock(400000);
  pinMode(teachBT1, INPUT_PULLUP);
  pinMode(startBT1, INPUT_PULLUP);
  pinMode(buzzer1, OUTPUT);;
  pinMode(re1,OUTPUT);
  pinMode(gr1,OUTPUT);
  digitalWrite(re1,LOW);
  digitalWrite(gr1,LOW);
  readInputs1();
  goHome1();
  goPot1();
}

void loop() {

  if (!Started){
    readInputs1();
    moveServos1();
    if (ReadTeach==LOW){
      savePosition1();
      digitalWrite(re1,HIGH);
      tone(buzzer1,100,30);
      delay(500);
      noTone(buzzer1);
    }
    if (ReadStart==LOW){
      tone(buzzer1,900);
      digitalWrite(gr1,HIGH);
      Started=true;
      delay(1000);
      noTone(buzzer1);  
    }
  }
  else{
    goHome1();
   runTeach1();
         digitalWrite(re1,LOW);
      digitalWrite(gr1,LOW);
  }
}

//Read potentiometers and buttons

void readInputs1()
{
  ReadUpDown = analogRead(potUpDown1);
  ReadUpDown = map(ReadUpDown,0,1024,minUpDown,maxUpDown);
  ReadForwardBackward = analogRead(potForwardBackward1);
  ReadForwardBackward = map(ReadForwardBackward,0,1024,minForwardBackward,maxForwardBackward);
  ReadRotate = analogRead(potRotate1);
  ReadRotate = map(ReadRotate,0,1024,minRotate,maxRotate);
  ReadGrab = analogRead(potGrab1);
  ReadGrab = map(ReadGrab,0,1024,minGrab,maxGrab);
  
  ReadTeach = digitalRead(teachBT1);
  ReadStart = digitalRead(startBT1);
  delay(5);
}
//Fuction for the main movement
void moveServos1()
{
 pwm.setPWM(Up_down,0,ReadUpDown);
   pwm.setPWM(Forward_backward,0,ReadForwardBackward);
    pwm.setPWM(Rotate,0,ReadRotate);
      pwm.setPWM(Grab,0,ReadGrab);
}
//Fuction for the saving the positions

void savePosition1()
{
  TeachUpDown[Index] = ReadUpDown;
  TeachForwardBackward[Index] = ReadForwardBackward;
  TeachRotate[Index] = ReadRotate;
  TeachGrab[Index] = ReadGrab;
  Index++;
}

//Fuction for the runnig  the stored positions

void runTeach1()
{
  for (int i=0; i<Index-1; i++){
    if (TeachRotate[i] < TeachRotate[i+1]){
      for (int j = TeachRotate[i]; j<= TeachRotate[i+1]; j++){
        pwm.setPWM(Rotate,0,j);
      
      }
    }
    else if (TeachRotate[i] > TeachRotate[i+1]){
      for (int j = TeachRotate[i]; j>= TeachRotate[i+1]; j--){
      pwm.setPWM(Rotate,0,j);
      }  
    }
    else{
  pwm.setPWM(Rotate,0,(TeachRotate[i]));

    }
    if (TeachGrab[i] < TeachGrab[i+1]){
      for (int j = TeachGrab[i]; j<=TeachGrab[i+1]; j++){
     pwm.setPWM(Grab,0,j);
      }
    }
    else if (TeachGrab[i] > TeachGrab[i+1]){
      for (int j = TeachGrab[i]; j>= TeachGrab[i+1]; j--){
     pwm.setPWM(Grab,0,j);        
     } 
    }
    else{
      pwm.setPWM(Grab,0,(TeachGrab[i]));
      
    }
    if (TeachForwardBackward[i] < TeachForwardBackward[i+1]){
      for (int j = TeachForwardBackward[i]; j<= TeachForwardBackward[i+1]; j++){
     pwm.setPWM(Forward_backward,0, j);        
      }
    }
    else if (TeachForwardBackward[i] > TeachForwardBackward[i+1]){
      for (int j = TeachForwardBackward[i]; j>= TeachForwardBackward[i+1]; j--){
             pwm.setPWM(Forward_backward,0,j);
      }
    }
    else{
           pwm.setPWM(Forward_backward,0,(TeachForwardBackward[i]));

    }
    if (TeachUpDown[i] < TeachUpDown[i+1]){
      for (int j = TeachUpDown[i]; j<= TeachUpDown[i+1]; j++){
      pwm.setPWM(Up_down,0, j);
      }
    }
    else if (TeachUpDown[i] > TeachUpDown[i+1]){
      for (int j = TeachUpDown[i]; j>= TeachUpDown[i+1]; j--){
     pwm.setPWM(Up_down, 0, j);
      }
    }
    else{
      pwm.setPWM(Up_down,0,(TeachUpDown[i]));

    }
  }
  Started=false;
}
//Fuction for the homing the arm before each new run cycle

void goHome1()
{
  if (ReadForwardBackward < 80){
      for (int j = ReadForwardBackward; j<=80; j++){
     pwm.setPWM(Forward_backward,0,j);
        delay(StepSpeed);
      }
    }
    else if (ReadForwardBackward > 80){
      for (int j = ReadForwardBackward; j>=80; j--){
        pwm.setPWM(Forward_backward,0,j);
        delay(StepSpeed);
      }
    }
    else{
   pwm.setPWM(Forward_backward,0,80);
    }
    if (ReadUpDown < 32){
      for (int j = ReadUpDown; j<=32; j++){
       pwm.setPWM(Up_down,0,j);
        delay(StepSpeed);
      }
    }
    else if (ReadUpDown > 32){
      for (int j = ReadUpDown; j>=32; j--){
      pwm.setPWM(Up_down,0,j);
        delay(StepSpeed);
      }
    }
    else{
   pwm.setPWM(Up_down,0,32);

      
    }
    if (ReadRotate < 0){
      for (int j = ReadRotate; j<=0; j++){
        pwm.setPWM(Rotate,0,j);
        delay(StepSpeed);
      }
    }
    else if (ReadRotate > 0){
      for (int j = ReadRotate; j>=0; j--){
pwm.setPWM(Rotate,0,j);      
        delay(StepSpeed);
      }
    }
    else{
      pwm.setPWM(Rotate,0,0);

    }
    if (ReadGrab < 148){
      for (int j = ReadGrab; j<=148; j++){
        pwm.setPWM(Grab,0,j);
        delay(StepSpeed);
      }
    }
    else if (ReadGrab > 148){
      for (int j = ReadGrab; j>=148; j--){
     pwm.setPWM(Grab,0,j);
        delay(StepSpeed);
      }
    }
    else{
 pwm.setPWM(Grab,0,148);
    }    
    //Always start from home position
    TeachForwardBackward[0]= 80;
    TeachUpDown[0]=32;
    TeachRotate[0]=20;
    TeachGrab[0]=148;
}

//Fuction for live movement of the arm 

void goPot1()
{
    if (0> ReadRotate){
      for (int j = 0; j>=ReadRotate; j--){
    pwm.setPWM(Rotate,0,j);
       
      }
    }
    else if (ReadRotate > 0){
      for (int j = 0; j<=ReadRotate; j++){
       pwm.setPWM(Rotate,0,j);

      }
    }
    else{
   pwm.setPWM(  Rotate, 0,ReadRotate);

    }
    if (ReadGrab > 148){
      for (int j = 148; j<=ReadGrab; j++){
       pwm.setPWM(Grab,5,0);
      }
    }
    else if (ReadGrab < 148){
      for (int j = 148; j>=ReadGrab; j--){
     pwm.setPWM(Grab,0,j);
      }
    }
    else{
 pwm.setPWM(Grab,0,ReadGrab);
    }
  if (80 > ReadForwardBackward){
      for (int j = 80; j>=ReadForwardBackward; j--){
 pwm.setPWM(Forward_backward,0,j);
      }
    }
    else if (80 < ReadForwardBackward){
      for (int j = 80; j<=ReadForwardBackward; j++){
 pwm.setPWM(Forward_backward,0,j);   
      }
    }
    else{
 pwm.setPWM(Forward_backward,0,ReadForwardBackward);
  
    }
    if (32 > ReadUpDown){
      for (int j = 32; j>=ReadUpDown; j--){
      pwm.setPWM(Up_down,0,j);
      }
    }
    else if (ReadUpDown > 32){
      for (int j = 32; j<=ReadUpDown; j++){
       pwm.setPWM(Up_down,0,j);
      }
    }
    else{
  pwm.setPWM(Up_down,0,ReadUpDown);
}
}

