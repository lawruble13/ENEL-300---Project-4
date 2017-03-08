#ifndef CAR_H
#define CAR_H
#include <Servo.h>
#include "base.h"

#define LEFT (int)1
#define RIGHT (int)2
using namespace std;

class Probe {
private:
  int read_pin;
  Servo servo;
  int servo_pin;
  int angle;
  const float speed = 211.7647;
public:
  Probe& operator=(const Probe& other);
  float checkDistance();
  void attach(int r_pin, int s_pin);
  void moveTo(int angle);
};

class ColorSensor {
private:
  int red_pin;
  int blue_pin;
  int green_pin;
  int read_pin;
public: 
  ColorSensor& operator=(const ColorSensor& other);
  const int red = 1;
  const int blue = 2;
  int getColor(bool flash = false);
};

class Car{
private:
  Servo servo_left;
  Servo servo_right;
  int left_pin;
  int right_pin;
  Probe probe;
  ColorSensor colorSensor;

  int adj_left;
  int adj_right;
  float top_speed = 0.161290323;
public:

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
  void setAdj(int right, int left);

  void probeAngle(int angle);
  float checkDistance();
  void alert();
  bool isRed();
};
#endif
