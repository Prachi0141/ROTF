/*
 * MotorKnob
 *
 * A stepper motor follows the turns of a potentiometer
 * (or other sensor) on analog input 0.
 *
 * http://www.arduino.cc/en/Reference/Stepper
 * This example code is in the public domain.
 */

#include <Stepper.h>

// change this to the number of steps on your motor
#define STEPS 200

// create an instance of the stepper class, specifying
// the number of steps of the motor and the pins it's
// attached to
Stepper stepper(STEPS, 2, 3, 4, 5);

// the previous reading from the analog input
int previous = 0;

void setup() {
  // set the speed of the motor to 30 RPMs
 stepper.setSpeed(100);
 pinMode(5,OUTPUT);
 pinMode(6,OUTPUT);
 analogWrite(6,100);
 analogWrite(7,100);
}

void loop() {
  // get the sensor value
  //int val = analogRead(0);

  // move a number of steps equal to the change in the
  // sensor reading
  stepper.step(STEPS*5);
  delay(1000);
 
  // remember the previous value of the sensor
  //previous = val;
}
