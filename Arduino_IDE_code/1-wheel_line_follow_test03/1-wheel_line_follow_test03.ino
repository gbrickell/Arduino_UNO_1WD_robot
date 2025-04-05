// 1-wheel line follow test 02
// This code is designed for the 3D printed one-wheeled robot that
//  uses a line following Sensor underneath the front 'servo steered' axle
//
// The line following sensor is connected to the Arduino Uno Motor Shield A pins and is powered by 5V

// define all the variable here to make them global

int linePin = A3;  // pin A5 used for the line following output pin

int onoffPin = A0;    // ON/OFF slider switch connected to the A0 pin and is HIGH when ON
int oblinePin = A1;   // OB/LINE slider switch connected to the A1 pin and is HIGH for LINE

int onoffstate = 9;
int oblinestate = 9;
int onofflast = 9;
int oblinelast = 9;

int seeingblack = 0;
int seeingwhite = 0;
int pinState = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(onoffPin, INPUT);    // ON/OFF pin set as INPUT 
  pinMode(oblinePin, INPUT);   // OB/LINE pin set as INPUT
  pinMode(linePin, INPUT);     // line follow sensor OUT pin set as INPUT
  

  Serial.println(" Line follow sensor test program start: ");
  
}

void loop()
{
  // run the test cycle continuously if the ON/OFF switch is in the ON position
  //     and the OB/LINE switch is on LINE

  onoffstate = 0;
  oblinestate = 0;
  // check the slide switch positions and set logic states
  if (digitalRead(onoffPin)) onoffstate = 1;
  if (digitalRead(oblinePin)) oblinestate = 1;

  // either do the line sensing or keep looping depending upon the switch state
  if (onoffstate == 1 and oblinestate == 1) {

    // do the line sensing as the ON/OFF slider switch is ON
    //     and the OB/LINE switch is at LINE

    // display slider switch positions if anything has changed
    if (onoffstate != onofflast or oblinestate != oblinelast) {
      Serial.println("  ON/OFF switch set to the ON position and ");
      Serial.println(" OB/LINE switch set to the LINE position ");
      // update the last state variables
      onofflast = onoffstate;
      oblinelast = oblinestate;
    }

    /// now do the line sensing
    // do the sensing for 100 loops before checking swityches again 
    for (int i=0; i<=99; i++) {    

      pinState = digitalRead(linePin);
      //Serial.print(" The sensor pinState is: ");
      //Serial.println(pinState);
      // if the sensor is LOW (=0) it's above the black line
      if (pinState == 0 && seeingblack == 0) {
        Serial.println(" The sensor is seeing a black surface");
        seeingblack = 1;
        seeingwhite = 0;
      }

      else if (pinState == 1 && seeingwhite == 0) {
        Serial.println(" The sensor is seeing a white surface");
        seeingblack = 0;
        seeingwhite = 1;
      }

      // pause for short time
      delay(5);

    }
    
  }
  
  else {
    
    // display slider switch positions if anything has changed
    if (onoffstate != onofflast or oblinestate != oblinelast) {
      Serial.println("  ON/OFF switch set to the OFF position and/or ");
      Serial.println(" OB/LINE switch set to the OB position ");
      // update the last state variables
      onofflast = onoffstate;
      oblinelast = oblinestate;
    }
    
  }

  delay(10);
  
}

