#ifndef CAR_H
#define CAR_H
#include <Servo.h>

#define LEFT (int)1
#define RIGHT (int)2
using namespace std;

class Headlight {
  int led_pin;
  int sens_pin;
  public:
  int checkDistance();
  void attachPins(int _led_pin, int _sens_pin);
};

class Car {
private:
  String _name;
/*  Headlight headlight_left;
  Headlight headlight_right;*/

  Servo servo_left;
  Servo servo_right;

  int pers_light_r;
  int pers_light_g;
  int pers_light_b;
  int adj_left;
  int adj_right;
  float top_speed = 0.161290323;
public:
  Headlight headlight_left;
  Headlight headlight_right;
  void sayHello();
  void rightForward(int speed=200);
  void leftForward(int speed=200);
  void driveForward(float distance, int speed=200);
  void turnLeft(float angle=90.0, int speed=200);
  void turnRight(float angle=90.0, int speed=200);
  void carStop();

  void setName(String _name_);
  void attachHeadlights(Headlight& right, Headlight& left);
  void attachServos(int right, int left);
  void setPersLight(int rpin, int gpin, int bpin);
  void setAdj(int right, int left);
  void onPersLight(int r, int g, int b);
};
#endif
