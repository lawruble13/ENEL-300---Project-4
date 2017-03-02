  #include "car.h"
  
  void Car::sayHello(){
    Serial.begin(9600);
    Serial.print("Hello! My name is ");
    Serial.print(_name);
    Serial.println(".");
    Serial.end();
  }
  void Car::rightForward(int speed){
    servo_right.writeMicroseconds(1500-speed+adj_right);
  }
  void Car::leftForward(int speed){
    servo_left.writeMicroseconds(1500+speed-adj_left);
  }
  void Car::driveForward(float distance, int speed){
    float time = (200.0/speed)*(distance/top_speed);
    time *= 1000;
    rightForward(speed);
    leftForward(speed);
    delay(time);
    carStop();
  }
  void Car::turnLeft(float angle, int speed){
    float time = (angle/90.0)*(1310-62);
    rightForward(speed);
    delay(time);
    carStop();
  }
  void Car::turnRight(float angle, int speed){
    float time = (angle/90.0)*(1310+10);
    leftForward(speed);
    delay(time);
    carStop();
  }
  void Car::carStop(){
    servo_right.writeMicroseconds(1500);
    servo_left.writeMicroseconds(1500);
  }

  void Car::setName(String _name_){
    _name = _name_;
  }
  void Car::attachHeadlights(Headlight& right, Headlight& left){
    headlight_right = right;
    headlight_left = left;
  }

  void Car::attachServos(int right, int left){
    if(servo_right.attached()){
      servo_right.detach();
    }
    if(servo_left.attached()){
      servo_left.detach();
    }
    servo_right.attach(right);
    servo_left.attach(left);
  }

  void Car::setPersLight(int rpin, int gpin, int bpin){
    pinMode(rpin, OUTPUT);
    pinMode(gpin, OUTPUT);
    pinMode(bpin, OUTPUT);
    pers_light_r = rpin;
    pers_light_g = gpin;
    pers_light_b = bpin;
  }
  void Car::onPersLight(int r, int g, int b){
    analogWrite(pers_light_r, r);
    analogWrite(pers_light_g, g);
    analogWrite(pers_light_b, b);
  }
  void Car::setAdj(int right, int left){
    if((0>right) || (200<right) || (0>left) || (200<left)){
      exit(1);
    }
    adj_right = right;
    adj_left = left;
  }
  
  int Headlight::checkDistance(){
    tone(led_pin, 38000, 8);
    delay(1);
    int ir=digitalRead(sens_pin);
    delay(1);
    return !ir;
  }

  void Headlight::attachPins(int _led_pin, int _sens_pin){
    led_pin = _led_pin;
    sens_pin = _sens_pin;
    pinMode(led_pin, OUTPUT);
    pinMode(sens_pin, INPUT);
  }

