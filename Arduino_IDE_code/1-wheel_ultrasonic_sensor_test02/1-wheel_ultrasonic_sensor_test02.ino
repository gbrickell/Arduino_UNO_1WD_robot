// 1-wheel ultrasonic sensor test 01
// This code is designed for the 3D printed one-wheeled robot that
//  uses an HC-SR04 Ultrasonic Sensor on the front 'servo steered' axle
//
// The ultrasonic sensor is connected to the Arduino Uno Motor Shield A pins and is powered by 5V

// define all the variable here to make them global

int trigPin = A4;   // pin A4 used for the sensor Ping Pin (Trig)
int echoPin = A5;   // pin A5 used for the sensor Echo Pin (Echo)
unsigned long duration, inches;
int indec, cmdec;
int inchconv = 147; // ratio between pulse width and inches
int cmconv = 59;    // ratio between pulse width and cm

int onoffPin = A0;       // ON/OFF slider switch connected to the A0 pin and is HIGH when ON
int oblinePin = A1;      // OB/LINE slider switch connected to the A1 pin and is HIGH for LINE

int onoffstate = 9;
int oblinestate = 9;
int onofflast = 9;
int oblinelast = 9;

int cm, lcm, rcm;

void setup()
{
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(onoffPin, INPUT);    // ON/OFF pin set as INPUT 
  pinMode(oblinePin, INPUT);   // OB/LINE pin set as INPUT
  
  Serial.println(" Ultrasonic sensor test program start: ");

}

void loop()
{
  onoffstate = 0;
  oblinestate = 0;
  if (digitalRead(onoffPin)) onoffstate = 1;
  if (digitalRead(oblinePin)) oblinestate = 1;

  if (onoffstate == 1 and oblinestate == 0) {

    // do the ultrasonic sensing as the ON/OFF slider switch is ON
    //     and the OB/LINE switch is at OB

    // display slider switch positions if anything has changed
    if (onoffstate != onofflast or oblinestate != oblinelast) {
      Serial.println("  ON/OFF switch set to the ON position and ");
      Serial.println(" OB/LINE switch set to the OB position ");
      // update the last state variables
      onofflast = onoffstate;
      oblinelast = oblinestate;
    }

    if (digitalRead(onoffPin)) {  
      cm = getDistance();
      Serial.print(cm);
      Serial.println(" cm");
      delay(1000);
    }
      
  }

  else {

    // display slider switch positions if anything has changed
    if (onoffstate != onofflast or oblinestate != oblinelast) {
      Serial.println("  ON/OFF switch set to the OFF position and/or ");
      Serial.println(" OB/LINE switch set to the LINE position ");
      // update the last state variables
      onofflast = onoffstate;
      oblinelast = oblinestate;
    }
    
  }

  delay(100);

}


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


long microsecondsToCentimeters(long microseconds)
{
  return microseconds / cmconv;
}


