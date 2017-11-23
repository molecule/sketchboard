#include <Servo.h>  // servo library

#include "Timer.h" //Timer library



Timer t;



//servo1 is a 360 servo, writing values changes direction and speed 

//(0 is full speed in one direction, 90 is stop, and 180 is full speed in opposite direction)

// values in between change the speed

Servo servo1;  //FISHING LINE SERVO: servo control object for longwise movement



//servo2 is a 180 servo, writing values tells it which angle to move to. only get one speed.

Servo servo2;  //PEN SERVO: servo control object for widthwise movement of pen. 

const int groundpin = 18;             // analog input pin 4 -- ground

const int powerpin = 19;              // analog input pin 5 -- voltage

const int xpin = A3;                  // x-axis of the accelerometer

const int ypin = A2;                  // y-axis

const int zpin = A1;                  // z-axis (only on 3-axis models)





//accelerometer threshold  values to indicate if board is centered or turning

const int centeredMin = 500;

const int centeredMax = 550;

const int leftTurn = 500;

const int rightTurn = 550;



//number of samples to take from accelerometer

const int sampleSize = 10;



//how long for the fishing line servo to move in one direction before switching

const int switchTime = 10000;



//is the fishing line servo moving forward or not

boolean forward = false;



//readings from accelerometer

int xValue;

int yValue;



//timer event

int stopEvent;



void setup()

{

  Serial.begin(9600);

  //fishing line servo goes in pin 6

  servo1.attach(6);

  //pen servo goes in pin 10

  servo2.attach(10);

  pinMode(groundpin, OUTPUT);

  pinMode(powerpin, OUTPUT);

  digitalWrite(groundpin, LOW);

  digitalWrite(powerpin, HIGH);

  //Checks every switchTime seconds to change direction

  int stopEvent = t.every(switchTime, moveX);

  //center the pen arduino

  servo2.write(90);

  delay(1000);

  //start the fishing line arduino

  servo1.write(0);

}



void moveX() {

  if (forward) {

    forward = false;

    servo1.write(0);

  } else {

   forward = true;

   servo1.write(180); 

  }

}



void loop()

{

  //check if it's time to switch directions

  t.update();

  

  //here until the for loop is gathering samples from the accelerometer to be averaged

  int position;

  int xAvg;

  int yAvg;



  xValue = analogRead(xpin);

  yValue = analogRead(ypin); 

  

  xAvg = 0;

  yAvg = 0;

  

  for(int i = 0; i < sampleSize; i++) {

    xAvg += analogRead(xpin);

    yAvg += analogRead(ypin);

  }

  

  //average readings from the accelerometer

  xAvg = xAvg/sampleSize;

  yAvg = yAvg/sampleSize;



  //if readings say its centered

  if(yAvg > centeredMin && yAvg < centeredMax) {

   //center the arduino

   servo2.write(90);

   //if readings say left turn

  } else if(yAvg < leftTurn) {

   //turn to the 50 degree angle

   servo2.write(50); 

   //if readings say right turn

  } else if(yAvg > rightTurn) {

    //turn to the 130 degree angle 

    servo2.write(130); 

  } else {

    //else, center it

   servo2.write(90); 

  }

  

}
