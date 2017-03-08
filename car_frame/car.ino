/*
 * Specifc Class Methods
 */
#include "car.h"

/*
 * Function Definitions for Probe class
 */

  /**
   * Copy assignment operator
   * 
   * @param other The object to be copied from. This disconnects all servos connected to the original object.
   * 
   * @return A reference to this object.
   */
   Probe& Probe::operator=(const Probe& other){
    if(this != &other){
      read_pin = other.read_pin;
      other.servo.detach();
      servo.attach(other.servo_pin);
      servo_pin = other.servo_pin;
      angle = other.angle;
    }
    return *this;
  }

  /**
   * Checks the distance of the nearest object to the probe.
   * TODO: Actually check the distance. Currently returns 4 m.
   * 
   * @return The distance to the object in meters, between ~ and ~ m
   */
  float Probe::checkDistance(){
    return 4.0;
  }
  /**
   * Sets up the probe from pin numbers.
   * 
   * @param r_pin The analog pin to read from.
   * @param s_pin The pin the servo is connected to.
   */
  void Probe::attach(int r_pin, int s_pin){
    read_pin = r_pin;
    if(servo.attached()){
      servo.detach();
    }
    servo.attach(s_pin);
    servo_pin = s_pin;
    servo.write(0);
    delay(1000);
    servo.write(180);
    delay(1000);
    servo.write(90);
    delay(1000);
    angle = 90;
  }
  /**
   * Moves the probe to a specific angle.
   * 
   * @param angle The angle to turn to. 0 is pointing directly left, 180 is pointing directly right.
   */
  void Probe::moveTo(int _angle){
    int time = 10+abs(_angle - angle)/speed;
    angle = _angle;
    servo.write(angle);
    delay(time);
  }

/*
 * Function Definitions for ColorSensor class
 */

  /**
   * Copy assignment operator
   * 
   * @param other Th object to be copied from.
   * 
   * @return A reference to this object.
   */
   ColorSensor& ColorSensor::operator=(const ColorSensor& other){
    if(this != &other){
      red_pin = other.red_pin;
      blue_pin = other.blue_pin;
      green_pin = other.green_pin;
      read_pin = other.read_pin;
    }
    return *this;
  }

  /**
   * Detects the color of an object in a range of ~ to ~ cm
   * 
   * @param flash Defaults to false. If true, LED will flash to show detected color.
   * @return 1 if red is detected, 2 if blue is detected
   */
  int ColorSensor::getColor(bool flash){
    // ColorSensor getColor method
    // Written by Cal Wilkes
    // Edited by Liam Wrubleski
    
    delay(500);

    digitalWrite(red_pin, HIGH);   // shines red light on target
    delay(800);
    int sensorValue_red = analogRead(read_pin);  // reads reflected light from target
    delay(200);
    digitalWrite(red_pin, LOW);
    float voltage_red = sensorValue_red * (5.0 / 1023.0); // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  
    delay(1200);
  
    digitalWrite(blue_pin, HIGH); // shines blue light on target
    delay(800);
    int sensorValue_blue = analogRead(read_pin);  // reads reflected light from target
    delay(200);
    digitalWrite(blue_pin, LOW);
    float voltage_blue = sensorValue_blue * (5.0 / 1023.0); // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  
    delay(1200);
  
  
    digitalWrite(green_pin, HIGH); // shines green light on target
    delay(800);
    int sensorValue_green = analogRead(read_pin);  // reads reflected light from target
    delay(200);
    digitalWrite(green_pin, LOW);
    float voltage_green = sensorValue_green * (5.0 / 1023.0); // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  
    delay(1200);
  
    if (voltage_red < voltage_blue) {
      if ((int)(10 * voltage_red) == (int)(10 * voltage_blue))
      {
        if ((int)(10 * voltage_green) < (int)(10 * voltage_blue)) {
          if(flash){
            for (int y = 0; y < 5; y++) {
              digitalWrite(red_pin, HIGH);   // indicates that red was detected
              delay(600);
              digitalWrite(red_pin, LOW);
              delay(200);
            }
          }
          return 1;
        }
        else {
          if(flash){
            for (int y = 0; y < 5; y++) {
              digitalWrite(blue_pin, HIGH);
              digitalWrite(green_pin, HIGH); // indicates that blue was detected
              delay(600);
              digitalWrite(blue_pin, LOW);
              digitalWrite(green_pin, LOW);
              delay(200);
            }
          }
          return 2;
        }
  
      }
      else {
        if(flash){
          for (int y = 0; y < 5; y++) {
            digitalWrite(red_pin, HIGH); // indicates that blue was detected
            delay(600);
            digitalWrite(red_pin, LOW);
            delay(200);
          }
        }
        return 1;
      }
    }
  
    else {
      if(flash){
        for (int y = 0; y < 5; y++) {
          digitalWrite(blue_pin, HIGH);
          digitalWrite(green_pin, HIGH); // indicates that blue was detected
          delay(600);
          digitalWrite(blue_pin, LOW);
          digitalWrite(green_pin, LOW);
          delay(200);
        }
      }
      return 2;
    }
  }
  
/*
 * Function Definitions for Car class
 */
  /**
   * Copy assignment operator
   * 
   * @param other The object to be copied from. This disconnects the previous object from the servos.
   * 
   * @return A reference to this object.
   */

   Car& Car::operator=(const Car& other){
    if(this != &other){
      other.servo_left.detach();
      servo_left.attach(left_pin);
      left_pin = other.left_pin;
      
      other.servo_right.detach();
      servo_right.attach(right_pin);
      right_pin = other.right_pin;
      probe = other.probe;
      colorSensor = other.colorSensor;
      adj_left = other.adj_left;
      adj_right = other.adj_right;
      top_speed = other.top_speed;
    }
    return *this;
   }


  /**
   * Starts the right wheel turning, in the direction that drives the car forwards.
   * 
   * @param speed The speed the wheel will turn at, between +-200.
   */
  void Car::rightForward(int speed){
    servo_right.writeMicroseconds(1500-speed+adj_right);
  }
  
  /**
   * Starts the left wheel turning, in the direction that drives the car forwards.
   * 
   * @param speed The speed the wheel will turn at, between +-200.
   */
  void Car::leftForward(int speed){
    servo_left.writeMicroseconds(1500+speed-adj_left);
  }

  /**
   * Drives the car forwards.
   * 
   * @param distance How far the car should go, in meters. 
   * @param speed How fast the car should go, between +-200. Defaults to 200.
   */
  void Car::driveForward(float distance, int speed){
    float time = abs(200.0/speed)*(distance/top_speed);
    time *= 1000;
    rightForward(speed);
    leftForward(speed);
    delay(time);
    carStop();
  }

    /**
   * Drives the car backwards.
   * 
   * @param distance How far the car should go, in meters. 
   * @param speed How fast the car should go, between +-200. Defaults to 200.
   */
  void Car::driveBackward(float distance, int speed){
    float time = abs(200.0/speed)*(distance/top_speed);
    time *= 1000;
    rightForward(-speed);
    leftForward(-speed);
    delay(time);
    carStop();
  }

  /**
   * Rotates the car left.
   * 
   * @param angle The angle by which the car should turn to the left in degrees, >= 0. Defaults to 90 degress.
   * @param speed The speed at which the car should turn, between +-200. Defaults to 200.
   */
  void Car::turnLeft(float angle, int speed){
    float time = (angle/90.0)*(1310-62);
    rightForward(speed);
    delay(time);
    carStop();
  }

  /**
   * Rotates the car right.
   * 
   * @param angle The angle by which the car should turn to the left in degress, >= 0. Defaults to 90 degress.
   * @param speed The speed at which the car should turn, between +-200. Defaults to 200
   */
  void Car::turnRight(float angle, int speed){
    float time = (angle/90.0)*(1310+10);
    leftForward(speed);
    delay(time);
    carStop();
  }

  /**
   * Stops the car.
   */
  void Car::carStop(){
    servo_right.writeMicroseconds(1500);
    servo_left.writeMicroseconds(1500);
  }

  /**
   * Aligns the car so it should drive parallel to a wall on its left, and be the specified distance (in meters) from the wall.
   * 
   * @param distance The required distance from the wall. Defaults to 10cm.
   */
   void Car::alignLeft(float distance){
    return;
   }

  /**
   * Aligns the car so it should drive parallel to a wall on its right, and be the specified distance (in meters) from the wall.
   * 
   * @param distance The required distance from the wall. Defaults to 10cm.
   */
   void Car::alignRight(float distance){
    return;
   }

  /**
   * Connects the wheel servos from the pin numbers.
   * 
   * @param right The pin number for the right wheel.
   * @param left The pin number for the left wheel.
   */
  void Car::attachServos(int right, int left){
    if(servo_right.attached()){
      servo_right.detach();
    }
    if(servo_left.attached()){
      servo_left.detach();
    }
    servo_right.attach(right);
    servo_left.attach(left);
    left_pin = left;
    right_pin = right;
  }
  
  /**
   * Sets the tuning adjustment for each wheel. 
   * TODO: Additive? Muliplicative?
   * 
   * @param right The adjustment for the right wheel
   * @param left The adjustment for the left wheel
   */
  void Car::setAdj(int right, int left){
    if((0>right) || (200<right) || (0>left) || (200<left)){
      exit(1);
    }
    adj_right = right;
    adj_left = left;
  }

  /**
   * Moves the attached probe to a specific angle. Added for convenience.
   * 
   * @param angle The angle to which the probe should move.
   */
  void Car::probeAngle(int angle){
    probe.moveTo(angle);
  }

  /**
   * Checks the distance using the attached probe. Added for convenience.
   * 
   * @return the distance of the object in front of the probe, in meters.
   */
  float Car::checkDistance(){
    return probe.checkDistance();
  }

  /**
   * Activates the buzzer in pulses.
   */
  void Car::alert(){
    return;
  }

  /**
   * Checks to see if the object in front of the car is red.
   * 
   * @return True if the object is red, false otherwise.
   */
  bool Car::isRed(){
    return (colorSensor.getColor() == colorSensor.red);
  }

