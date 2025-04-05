// 1-wheel_avoidance01.ino - autonomous obstacle avoidance mode
// This code is designed for the 3D printed one-wheeled robot that
//  uses an HC-SR04 Ultrasonic Sensor on the front 'servo steered' axle
//
// The motor, as well as the steering servo, are controlled using the Adafruit Motor shield library
// The front wheel servo is connected to the servo 1 port on the motor shield connected to the Arduino Uno

// The ultrasonic sensor is connected to the Arduino Uno Motor Shield A pins and is powered by 5V

#include <Servo.h>
#include <AFMotor.h>

Servo steerServo;
AF_DCMotor motor(1);

// define all the variables here to make them global

// ***************************
// ultrasonic sensor variables
// ***************************
int trigPin = A4;   // pin A4 used for the sensor Ping Pin (Trig)
int echoPin = A5;   // pin A5 used for the sensor Echo Pin (Echo)
unsigned long duration, inches;
int indec, cmdec;
int inchconv = 147; // ratio between pulse width and inches
int cmconv = 59;    // ratio between pulse width and cm
int avoid_distance = 30;   // set a defined distance to take avoidance action
int avoid_limit = 5;       // number of successive times that distance limit is reached before action taken - avoids spurious readings
int avoid_count = 0;       // number of times that distance limit is reached

// **********************
// slide switch variables
// **********************
int onoffPin = A0;       // ON/OFF slider switch connected to the A0 pin and is HIGH when ON
int oblinePin = A1;      // OB/LINE slider switch connected to the A1 pin and is HIGH for LINE
// set some initial non-real states
int onoffstate = 9;
int oblinestate = 9;
int onofflast = 9;
int oblinelast = 9;
int servo_last = 999;

int cm, lcm, rcm;

int steerPin = 10;  // this is Servo 1 on the motor shield

int steerCentre = 132;  // set this 'offset' to be dead ahead setting (or adjust servo horn mounting)


void setup()
{
  Serial.begin(115200);
  
  steerServo.attach(steerPin, 1000, 2000);   // configures pin,min,max ie can only be 9 or 10 on early basic boards
                                             // min = pulse width for the min (0-degree) angle (default is 544)
                                             // max = pulse width for the max (180-degree) angle (default is 2400)
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(onoffPin, INPUT);    // ON/OFF pin set as INPUT 
  pinMode(oblinePin, INPUT);   // OB/LINE pin set as INPUT

  Serial.println(" autonomous avoidance mode program start: ");
  steer(0);
}

void loop()
{
  // run the autonomous obstacle avoidance function continuously if the ON/OFF switch is in the ON position
  
  if (digitalRead(onoffPin)) {
    onoffstate = 1;
    if (onoffstate != onofflast) {
      Serial.println(" ON/OFF switch set to the ON position ");
      onofflast = onoffstate;
    }
    
    auto_avoid();  // run the main avoidance routine
    
  }
  
  else {
    onoffstate = 0;
    if (onoffstate != onofflast) {
      Serial.println(" ON/OFF switch set to the OFF position ");
      onofflast = onoffstate;
    }

    // in the OFF position so make sure the robot is stopped
    //   and the steering is placed inthe central position
    halt();
    steer(0);
    
  }
  
}


// **************************************
// autonomous obstacle avoidance function
// **************************************
void auto_avoid()
{
  
  int steerdir, cm, lcm, rcm;
  forward(200);
  delay(100);                // keep going forward and check the distance ahead every 100 ms
  cm = getDistance();

  if (cm < avoid_distance) {  // if an object is less than the avoid distance then 'count' the # of times this has happened
    avoid_count = avoid_count + 1;
    Serial.print(" distance: ");
    Serial.println(cm);
    Serial.print(" avoid count: ");
    Serial.println(avoid_count);
  }
  
  if(avoid_count >= avoid_limit) {  // if avoid count limit is reached then take avoidance action
    avoid_count = 0;  // reset the count limit first
    halt();
    steer(-55);
    lcm = getDistance();    // measure the distance ahead to the left
    steer(55);
    rcm = getDistance();    // measure the distance ahead to the right

    // back up steering to either left or right to try and avoid object
    if (lcm < rcm) {
      steerdir = 1;
    } 
    else {
      steerdir = -1;
    }
    steer(-steerdir*45);
    reverse(150);
    delay(1000);
    steer(steerdir*45);
    forward(150);
    delay(1000);
    steer(0);
  }
}


// *************************************
// direction and speed control functions
// *************************************

void forward (int spd)
{
  motor.run(FORWARD);
  motor.setSpeed(spd);  
}
 
void reverse(int spd)
{
  motor.run(BACKWARD);
  motor.setSpeed(spd);  
 }
 
void spinLeft(int spd)
{
  // not set up yet
}
 
void spinRight(int spd)
{
  // not set up yet
}
 
void halt()
{
  motor.run(RELEASE);
  delay(10);
}


// *********************************************
// function to sense the distance to an obstacle
// *********************************************
int getDistance()
{
  int rval;
  // send trigger pulse and time how long it takes to 'hear' it come back
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH, 38000L);  // Set timeout to 38mS
  if (duration == 0)
    duration = 1000;
  rval = microsecondsToCentimeters(duration);
//  Serial.println(rval);
  return rval;
}

// *******************
// conversion function
// *******************
long microsecondsToCentimeters(long microseconds)
{
  return microseconds / cmconv;
}

// ********************************************
// function to set the servo angle for steering
// ********************************************
void steer(int angle)
{
  if (angle < -55) { angle = -55; }       // set a minimum relative angle that the wheels can turn to
  if (angle > +55) { angle = +55; }       // set a maximum relative angle that the wheels can turn to
  steerServo.write(steerCentre + angle);  // set the servo angle relative to the 'steerCentre' offset
  if (angle != servo_last) {
    Serial.print(" servo position: ");
    Serial.println(angle);
    servo_last = angle;
  }
  
  delay(200); // wait for servo to get there
}




