bool sense_red(int red, int blue, int green, int sense) {

  float voltage_red = test_red(red, sense);
  float voltage_blue = test_blue(blue, sense);


  if (voltage_red < voltage_blue) {
    if ((int)(10 * voltage_red) == (int)(10 * voltage_blue))
    {
      float voltage_green = test_green(green, sense);
      if (voltage_green < voltage_blue)  {
        for (int y = 0; y < 5; y++) {
          digitalWrite(red, HIGH);   // indicates that red was detected
          delay(600);
          digitalWrite(red, LOW);
          delay(200);
        }
        return true;
      }
      else {
        for (int y = 0; y < 5; y++) {
          digitalWrite(green, HIGH); // indicates that blue was detected
          delay(300);
          digitalWrite(green, LOW);
          delay(100);
          digitalWrite(blue, HIGH); // indicates that blue was detected
          delay(300);
          digitalWrite(blue, LOW);
          delay(100);
        }
        return false;
      }

    }
    else {
      for (int y = 0; y < 5; y++) {
        digitalWrite(red, HIGH); // indicates that blue was detected
        delay(600);
        digitalWrite(red, LOW);
        delay(200);
      }
      return true;
    }
  }

  else {
    for (int y = 0; y < 5; y++) {
      digitalWrite(green, HIGH); // indicates that blue was detected
      delay(300);
      digitalWrite(green, LOW);
      delay(100);
      digitalWrite(blue, HIGH); // indicates that blue was detected
      delay(300);
      digitalWrite(blue, LOW);
      delay(100);
    }
    return false;
  }
}


float test_red(int red, int sense) {
  delay(100);
  digitalWrite(red, HIGH);   // shines red light on target
  delay(800);
  int sensorValue_red = analogRead(sense);  // reads reflected light from target
  delay(200);
  digitalWrite(red, LOW);
    delay(100);
  return sensorValue_red * (5.0 / 1023.0); // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):

}


float test_blue(int blue, int sense) {
  delay(100);
  digitalWrite(blue, HIGH); // shines blue light on target
  delay(800);
  int sensorValue_blue = analogRead(sense);  // reads reflected light from target
  delay(200);
  digitalWrite(blue, LOW);
    delay(100);
  return sensorValue_blue * (5.0 / 1023.0); // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
}

float test_green(int green, int sense) {

  delay(100);
  digitalWrite(green, HIGH);
  // shines green light on target
  delay(800);
  int sensorValue_green = analogRead(sense);  // reads reflected light from target
  delay(200);
  digitalWrite(green, LOW);
    delay(100);
  return sensorValue_green * (5.0 / 1023.0); // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):

}
