/*
 * Specifc Class Methods
 */
#include "car.h"
#include "colorSensor.h"

/*
 * Function Definitions for Probe class
 */

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

  bool ColorSensor::isRed(){
  
    return sense_red(red_pin, blue_pin, green_pin, read_pin);
  }

  /**
   * Attaches color sensor.
   * 
   * @param r The red pin.
   * @param g The green pin.
   * @param b The blue pin. 
   * @param s The sensor pin.
   * 
   * 
   */
     void ColorSensor::attach(int r, int g, int b, int s){
      red_pin=r;
      green_pin=g;
      blue_pin=b;
      read_pin=s;
      pinMode(r, OUTPUT);
      pinMode(g, OUTPUT);
      pinMode(b, OUTPUT);
      pinMode(s, INPUT);
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
    servo_right.writeMicroseconds(1500-speed*adj_right);
  }
  
  /**
   * Starts the left wheel turning, in the direction that drives the car forwards.
   * 
   * @param speed The speed the wheel will turn at, between +-200.
   */
  void Car::leftForward(int speed){
    servo_left.writeMicroseconds(1500+speed*adj_left);
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
    float time = (angle/90.0)*(1050);
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
    float time = (angle/90.0)*(1150);
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
   * 
   * @param right The adjustment for the right wheel
   * @param left The adjustment for the left wheel
   */
  void Car::setAdj(float right, float left){
    if(right == 0 || left == 0){
      return;
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
   * Attaches the probe
   * 
   * @param servo_pin The pin the servo is on.
   * @param read_pin The pin to read from.
   */
  void Car::probeAttach(int read_pin, int servo_pin){
    probe.servo.write(0);
    delay(1000);
    probe.servo.write(160);
    delay(1000);
    probe.angle = 180;
    probe.moveTo(90);
    probe.servo.attach(servo_pin);
    probe.read_pin = A0;
  }

  /**
   * Attaches the color sensor.
   * 
   * @param r The pin for red.
   * @param g The pin for green.
   * @param b The pin for blue.
   * @param s The pin to read from.
   */
   void Car::colorAttach(int r, int g, int b, int s){
    colorSensor.attach(r,g,b,s);
   }

  /**
   * Checks the distance using the attached probe. Added for convenience.
   * 
   * @return the distance of the object in front of the probe, in meters.
   */
  bool Car::checkDistance(){
    return probe.checkDistance();
  }

  float Car::getDistance(){
    return probe.getDistance();
  }

  /**
   * Activates the buzzer in pulses.
   */
  void Car::alert(){
    pinMode(7, OUTPUT);
    digitalWrite(7, HIGH);

    pinMode(buzzer_pin, OUTPUT); // Set buzzer - pin 9 as an output

    int tones [] = { 880, 987, 739, 392, 587};  // array of different tones to play
    
    for(int i =0; i< 5; i++){
      tone(buzzer_pin,tones[i]); // Send sound signal...
      delay(700);        // ...for 0.7 sec
      noTone(buzzer_pin);     // Stop sound...
      delay(300);        // ...for 0.3sec
    }
    return;
  }

  /**
   * Checks to see if the object in front of the car is red.
   * 
   * @return True if the object is red, false otherwise.
   */
  bool Car::isRed(){
    return colorSensor.isRed();
  }

