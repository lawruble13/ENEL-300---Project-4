#ifndef CAR_H
#define CAR_H
#include <Servo.h>
#include "FSM.h"

#define LEFT (int)1
#define RIGHT (int)2
using namespace std;

class Probe {

  public:
  Servo servo;
  int read_pin;
  int angle;
  void moveTo(int _angle){
    _angle = map(_angle, 0, 180, 0, 155);
    int pos;
    if(angle < _angle){
      for (pos = angle; pos <= _angle; pos += 1) { // goes from 0 degrees to 180 degrees
       // in steps of 1 degree
       servo.write(pos);              // tell servo to go to position in variable 'pos'
       delay(8);                       // waits 15ms for the servo to reach the position
       angle = _angle;
      }
   }
   if(angle > _angle){
     for (pos = angle; pos >= _angle; pos -= 1) { // goes from 180 degrees to 0 degrees
       servo.write(pos);              // tell servo to go to position in variable 'pos'
       delay(8);                       // waits 15ms for the servo to reach the position
       angle = _angle;
     }
   }
  }

  bool checkDistance(){
    pinMode(read_pin, INPUT);
    if(analogRead(read_pin) > 400){
      return true;
    }
    return false;
  }
  int getDistance(){
    float d = (float(analogRead(read_pin))-677.55)/(-48.98);
    return d;
  }
};

class ColorSensor {
private:
  int red_pin;
  int blue_pin;
  int green_pin;
  int read_pin;
public: 
  ColorSensor& operator=(const ColorSensor& other);
  bool isRed();
  float test_red();
  float test_blue();
  float test_green();
  const int red = 1;
  const int blue = 2;
  int getColor(bool flash = false);
  void attach(int r, int g, int b, int s);
};

class Car{
private:
  Servo servo_left;
  Servo servo_right;
  int left_pin;
  int right_pin;
  Probe probe;
  ColorSensor colorSensor;
  int buzzer_pin;

  float adj_left;
  float adj_right;
  float top_speed = 0.17096774238;//0.161290323;
public:
  float distanceForward = 0;
  float distanceRight = 0;
  float distanceLeft = 0;

  Car& operator=(const Car& other);

  void rightForward(int speed=200);
  void leftForward(int speed=200);
  void driveForward(float distance, int speed=200);
  void driveBackward(float distance, int speed=200);
  void turnLeft(float angle=90.0, int speed=200);
  void turnRight(float angle=90.0, int speed=200);
  void alignLeft(float distance = 0.1);
  void alignRight(float distance = 0.1);
  void carStop();

  void attachServos(int right, int left);
  void setAdj(float right, float left);
  void setBuzzer(int b_pin){buzzer_pin = b_pin;}

  void probeAngle(int angle);
  void probeAttach(int read_pin, int servo_pin);
  bool checkDistance();
  float getDistance();
  void alert();
  bool isRed();
  void colorAttach(int r, int g, int b, int s);
};
#endif
