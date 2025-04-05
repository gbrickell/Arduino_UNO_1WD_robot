// 1-wheel steering servo test 01
// This code is designed for the 3D printed one-wheeled robot that
//  uses an HC-SR04 Ultrasonic Sensor on the front 'servo steered' axle
//
// The servo is connected to the servo 1 port on the Arduino UNO

#include <Servo.h>


int steerPin = 10;  // this is Servo 1 on the motor shield
Servo steerServo;

int steerCentre = 132;  // set this 'offset' to be dead ahead setting (or adjust servo horn mounting)

int onoffPin = A0;       // ON/OFF slider switch connected to the A0 pin and is HIGH when ON

void setup()
{
  Serial.begin(9600);
  
  pinMode(onoffPin, INPUT);    // ON/OFF pin set as INPUT
  
  steerServo.attach(steerPin, 1000, 2000);   // configures pin,min,max ie can only be 9 or 10 on early basic boards
                                             // min = pulse width for the min (0-degree) angle (default is 544)
                                             // max = pulse width for the max (180-degree) angle (default is 2400)
  steer(0);

  bool onoffprint = true;

  Serial.println(" Steering servo test program start: ");

  Serial.println(" servo position: 0 ");
  
}

void loop()
{
  // run the test cycle continuously if the ON/OFF switch is in the ON position
  bool onoffprint;
  if (digitalRead(onoffPin)) {
    Serial.println(" ON/OFF switch set to the ON position ");
    onoffprint = true;
    runTests();
  }
  else {
    if (onoffprint) {
      // Serial.println(onoffprint);
      Serial.println(" ON/OFF switch set to the OFF position ");
      onoffprint = false;
      // Serial.println(onoffprint);
    }
  }
}


void runTests()
{

  steer(-70);
  Serial.println(" servo position: -70 ");
  delay(700);

  steer(70);
  Serial.println(" servo position: +70 ");
  delay(700);

  steer(-55);
  Serial.println(" servo position: -55 ");
  delay(700);

  steer(55);
  Serial.println(" servo position: +55 ");
  delay(700);
  
  steer(0);
  Serial.println(" servo position: 0 ");
  delay(700);
  
  steer(-30);
  Serial.println(" servo position: -30 ");
  delay(700);
  
  steer(30);
  Serial.println(" servo position: +30 ");
  delay(700);
  
  steer(0);
  Serial.println(" servo position: 0 ");
  delay(700);
  
}

void steer(int angle)
{
  if (angle < -55) { angle = -55; }       // set a minimum relative angle that the wheels can turn to
  if (angle > +55) { angle = +55; }       // set a maximum relative angle that the wheels can turn to
  steerServo.write(steerCentre + angle);  // set the servo angle relative to the 'steerCentre' offset
  delay(800); // wait for servo to get there
}





