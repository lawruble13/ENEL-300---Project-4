#include "car.h"

Car Luigi;
Headlight h_right;
Headlight h_left;
int r = 0;
int g = 255;
int b = 0;
void setup()
{
  h_right.attachPins(2,4);
  h_left.attachPins(9,10);
  Luigi.attachHeadlights(h_right, h_left);
  Luigi.attachServos(11,12);
  Luigi.setName("Luigi");
  Luigi.setAdj(0,118);
  Luigi.setPersLight(6,5,3);/*
  for (int i = 0; i < 3; i++){
    Luigi.driveForward(1.1);
    Luigi.turnRight();
  }
  Luigi.driveForward(1.1);
  Luigi.turnRight(180.0);
  delay(150);
 // Luigi.turnRight();
  for (int i = 0; i < 4; i++){
    Luigi.driveForward(1.1);
    Luigi.turnLeft();
  }*/
  Serial.begin(9600);
}

void loop()
{
  Serial.print("Right: ");
  Serial.print(Luigi.headlight_right.checkDistance());
  Serial.print(" Left: ");
  Serial.println(Luigi.headlight_left.checkDistance());
  delay(10);
  
  Luigi.onPersLight(r,g,b);
  if(!Luigi.headlight_right.checkDistance() && Luigi.headlight_left.checkDistance()){
    r=0;
    g=0;
    b=255;
    Luigi.onPersLight(r,g,b);
    Luigi.driveForward(.01);
    r=0;
    g=255;
    b=0;
    Luigi.rightForward(200);
    Luigi.leftForward(200);
  } else if(Luigi.headlight_right.checkDistance()){
    Luigi.carStop();
    r=255;
    g=0;
    b=0;
    Luigi.onPersLight(r,g,b);
  } else {
    Luigi.carStop();
    Luigi.turnRight(60.0, -200);
    r=255;
    g=0;
    b=0;
    Luigi.onPersLight(r,g,b);
  }
}

