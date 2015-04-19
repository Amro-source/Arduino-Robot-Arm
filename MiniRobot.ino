 /*
serial config:
Board: Arduiono Pro / Pro Mini
Port:  tty.usbseriala400eMNr
Programmer: USBasploader

Teach mode: After a reset the robot arm follows the teach in arm while simple mapping the analog inputs every 25ms to the servo motors. Pressing the button stores each servo position in a array
Play mode: Double press the button switch to play mode. The sketch reads the array step by step and and moves the robot arm. For cool loocking movements i added a routine calculates different micro steps for each servo to have moving start and end sync on all axis. Also added a ramp for soft increase/decrease velocity. Shorter travel distances the robot does slow, longer distances with faster speed. Its all about timing so my thoughts in this moment
Play Mode version 1.1 The gripper input is used to set the delay (0,1,3,15,60,300 seconds) after a loop is done. The switch (it was left from the project start) pauses the robot
*/
// Definition
#include <Servo.h> // servo driver // servo header file

Servo servo_0;
Servo servo_1;
Servo servo_2;
Servo servo_3;

int sensorPin0 = A0;    // Shoulder   |
int sensorPin1 = A1;    // Hand       | these pins are connected with
int sensorPin2 = A2;    // Elbow      | ~22k variable resistors
int sensorPin3 = A3;    // Claw       |
int count0, arrayStep, arrayMax, countverz, Taster, stepsMax, steps, time = 1000, del = 1000, temp;
unsigned int  verz = 0;

long previousMillis1 = 0;
long previousMillis2 = 0;
long previousMillis3 = 0;
long previousMillis4 = 0;
long previousMicros = 0;
//storing millis and micros values.
unsigned long currentMillis = millis();
unsigned long currentMicros = micros();

// arrays
int Delay[7] = {0,0,1,3,15,60,300};
int SensVal[4]; // sensor values
float dif[4], ist[4], sol[4],  dir[4];
int joint0[180]; // these are the arrays 
int joint1[180]; // that stores the values of 
int joint2[180]; // the joints of the robot
int joint3[180]; //
int top = 179;   // max step that we can store into the ram
// status 
boolean playmode = false, Step = false;

void setup()
{
  pinMode(4, INPUT);  // sets the digital pin 4 as input
  pinMode(6, INPUT);  // sets the digital pin 4 as input
  pinMode(13, OUTPUT);  // sets the digital pin 4 as input
  digitalWrite(13, HIGH);   // sets the LED on
  servo_0.attach(3);        
  servo_1.attach(5);
  servo_2.attach(10);
  servo_3.attach(11);
  Serial.begin(115200);
  Serial.println("mini robot ready...");     
  //delay(1000);
  digitalWrite(13, LOW);
}

void loop()
{
  currentMillis = millis();
  currentMicros = micros();
  
  // Switch query  
  Button();
  
  if(!playmode) // manual control
  {        
    if(currentMillis - previousMillis1 > 25) // if 25 millisecond has passed
    {
      if (arrayStep < top)                   // and arrayStep(stored step) is less than maxstep
      {
        previousMillis1 = currentMillis;     // set again the previousmillis value
        readPot();  //read the potentiometers
        mapping();  //map the potentiometer for the servo position
        move_servo(); //move the servos.
        //record();   ----------------------record mode is off for manual control mode
      } // end zähler < max
    } // end step (<20ms
  } // end manual control
   
  else if(playmode) // play mode
  {
    if (Step) // next step from array read
    {
      digitalWrite(13, HIGH);
      if (arrayStep < arrayMax)
      {
        arrayStep += 1;
        Read();
        calculate();
        Step = 0;
        digitalWrite(13, LOW);  
      }
      else // list Finished. restart
      {
        arrayStep = 0; // 
        calc_pause(); // Break Length from Clawnpoti read     
        countverz = 0;
        while(countverz < verz)
        {
          countverz += 1;
          calc_pause();
          digitalWrite(13, HIGH); delay(25);   
          digitalWrite(13, LOW); delay(975);
        }
      }
      //Serial.println(arrayStep);
    }
    else // auscontrol
    {
      if (currentMicros - previousMicros > time) // speed For servo update/step
      {
        previousMicros = currentMicros;
        play_servo(); 
      }
    }
  }// end playmode

// ---------------------------------------------------------------------------------Hardware Pause Switch
    while (digitalRead(4) == true)
      { 
        digitalWrite(13, HIGH); delay(500);   
        digitalWrite(13, LOW); delay(500);
      }
// ---------------------------------------------------------------------------------- Text Output Serial
    // serial ausgabe 1 sek
    /*if(currentMillis - previousMillis2 > 5000)
    { 
      previousMillis2 = currentMillis;
      /*count0 = 0;
      while(count0 < 4)
      {
        int val = SensVal[count0];
      // val = map(val, 142, 888, 0, 180);
        Serial.println(val);
        //Serial.println("test");
        count0 += 1;
      }
      Serial.println(playmode); 
      Serial.println(arrayStep);    
      Serial.println(arrayMax);    
      Serial.println(" ");    
    }*/
}

// ---------------------------------------------------------------------------------------- sub routinen
void calc_pause()
{
    readPot();
    temp = SensVal[3];
    if (temp < 0) temp = 0;
    temp = map(temp, 0, 680, 0 ,6); 
    verz = Delay[temp];
    /*Serial.print(temp);
          Serial.print(" ");
          Serial.print(verz);
          Serial.print(" ");
          Serial.println(countverz);*/
}

void readPot()
{
   SensVal[0] = analogRead(sensorPin0); SensVal[0] += -10; // Shoulder Turn
   SensVal[1] = analogRead(sensorPin1); SensVal[1] += 280; // Shoulder 
   SensVal[2] = analogRead(sensorPin2); SensVal[2] += -50; // Hand
   SensVal[3] = analogRead(sensorPin3); // SensVal[3] += 0;// Claw
   //Serial.print(SensVal[2]);Serial.print(" "); // CHECK
}
void mapping()
{
  ist[0] = map(SensVal[0], 150, 900, 600, 2400);// Shoulder Turn
  ist[1] = map(SensVal[1], 1000, 100, 550, 2400);// Shoulder
  ist[2] = map(SensVal[2], 120, 860, 400, 2500);// Hand
  ist[3] = map(SensVal[3], 1023, 0, 500, 2500);// Claw
  //Serial.println(ist[2]); // CHECK
}
void record()
{
    joint0[arrayStep] = ist[0];
    joint1[arrayStep] = ist[1];
    joint2[arrayStep] = ist[2];
    joint3[arrayStep] = ist[3];
}
void Read()
{
    sol[0] = joint0[arrayStep];
    sol[1] = joint1[arrayStep];
    sol[2] = joint2[arrayStep];
    sol[3] = joint3[arrayStep];
}
void move_servo()
{                         // Connections Mapped   | just move the servos
  servo_0.writeMicroseconds(ist[3]); // Claw      | to this position
  servo_1.writeMicroseconds(ist[2]); // Hand      |
  servo_2.writeMicroseconds(ist[0]); // Shoulder  |
  servo_3.writeMicroseconds(ist[1]); // Elbow     |
}

// ------------------------------------------------------------ einzelschritte berechnen
void calculate()
{
      // wegstrecken berechnen
      dif[0] = abs(ist[0]-sol[0]);
      dif[1] = abs(ist[1]-sol[1]);
      dif[2] = abs(ist[2]-sol[2]);
      dif[3] = abs(ist[3]-sol[3]);

      // find the greatest value. why we need it?
      stepsMax = max(dif[0],dif[1]);
      stepsMax = max(stepsMax,dif[2]);
      stepsMax = max(stepsMax,dif[3]);
      
      //Serial.println(stepsMax); 
      
      if (stepsMax < 500) // slow if maxstep < 500
        del = 1200;
      else
        del = 600;
      
       // single-step calculate (step + pos/neg)
      if (sol[0] < ist[0]) dir[0] = 0-dif[0]/stepsMax; else dir[0] = dif[0]/stepsMax;
      if (sol[1] < ist[1]) dir[1] = 0-dif[1]/stepsMax; else dir[1] = dif[1]/stepsMax;
      if (sol[2] < ist[2]) dir[2] = 0-dif[2]/stepsMax; else dir[2] = dif[2]/stepsMax;
      if (sol[3] < ist[3]) dir[3] = 0-dif[3]/stepsMax; else dir[3] = dif[3]/stepsMax;
        //Serial.println(dir4);

}
void play_servo()
{
  steps += 1;
  if (steps < stepsMax)
  {
    //time = del*5;// anfahr rampe
    if(steps == 20) time = del*4;          // anfahr rampe up
    else if(steps == 40) time = del*3;
    else if(steps == 80) time = del*2;
    else if(steps == 100) time = del-1;
    if(steps == stepsMax-200) time = del*2;        // stop rampe down
    else if(steps == stepsMax-80) time = del*3;
    else if(steps == stepsMax-40) time = del*4;
    else if(steps == stepsMax-20) time = del*5;
    
    ist[0] += dir[0]; // set new pos
    ist[1] += dir[1];
    ist[2] += dir[2];
    ist[3] += dir[3];

    servo_0.writeMicroseconds(ist[3]); // Claw // Connections Mapped
    servo_1.writeMicroseconds(ist[2]); // Hand
    servo_2.writeMicroseconds(ist[0]); // Shoulder
    servo_3.writeMicroseconds(ist[1]); // Elbow
  }
  else
  {
    Step = 1; // next step read from array
    steps = 0; // servo between steps
  }
}

void data_out() // generated list with servo data (millisecond)
{               // just print the stored values ?!!!!!!!!!!!!
  int i = 0;
  while(i < arrayMax)//                           print all value of joint 0 from the array
  {
    digitalWrite(13, HIGH);
    i += 1;
    Serial.print(joint0[i]); Serial.print(", ");
  }
  Serial.println("Joint0");
  i = 0;
  while(i < arrayMax)//                           print all value of joint 1 from the array
  {
    digitalWrite(13, HIGH);
    i += 1;
    Serial.print(joint1[i]); Serial.print(", ");
  }
  Serial.println("Joint1");
  i = 0;
  while(i < arrayMax)//                           print all value of joint 2 from the array
  {
    digitalWrite(13, HIGH);
    i += 1;
    Serial.print(joint2[i]); Serial.print(", ");
  }
  Serial.println("Joint2");
  i = 0;
  while(i < arrayMax)//                           print all value of joint 3 from the array
  {
    digitalWrite(13, HIGH);
    i += 1;
    Serial.print(joint3[i]); Serial.print(", ");
  }
  Serial.println("Joint3");
}
/*
logic :
press button
instantly release(within 20ms)
            |
      -----------------------------------
      |                                 |
  if tester=0,            if tester = 1 and more than 250ms passed
  tester=1                          then tester=2        
  store millis
                                                                            |
                                                                            |
                                                                            |
                                                                            |
if button not pressed and tester=1 and more than 1 second has passed-------------else if tester=2
increase ArrrayStep                                                           arraystep = 0/>next record starts from 0
update arraymax pointer to arrayStep                                          playmode = true
record it(put into ram the ist values)                                        print all value from array to serial port & turn on led
tester=0                                                                      turn off the led
make playmode=false
blink led

if 2s  passed, tester=0
*/
void Button()
{
  if (digitalRead(6) == false) //if pin6 is LOW------------------------------------button press
  {
    delay(20); //delay 20ms
    if (digitalRead(6) == true) // if pin6 is high again, only then,---------------release within 20ms
    {
      if (Taster == 0) //if tester=0, 
      {
        Taster = 1; //change tester to 1 and store current millis to previousmillis
        previousMillis3 = currentMillis;
        //Serial.print("Status Record "); Serial.println(Taster); 
      }
      else if ((Taster == 1) && (currentMillis - previousMillis3 < 250))//if tester=1
      {
        Taster = 2;
        //Serial.println(Taster); 
      }
      /*else if ((Taster == 2) && (currentMillis - previousMillis3 < 500))
      {
        Taster = 3;
        Serial.println(Taster); 
      }*/
    }
  }
    
  if ((Taster == 1) && (currentMillis - previousMillis3 > 1000)) // write on array
  {
    arrayStep += 1;
    arrayMax = arrayStep;
    record();
    Taster = 0;
    playmode = false;
    Serial.print("Record Step: "); Serial.println(arrayStep);
    digitalWrite(13, HIGH);
    delay(100);
    digitalWrite(13, LOW);
  }
  else if (Taster == 2)
  {
    arrayStep = 0;
    playmode = true;
    Taster = 0;
    Step = 1;
    Serial.println("playmode ");
    data_out();
    delay(250);   
    digitalWrite(13, LOW);    
  }
  /*if (Taster == 3)
  {
    // ++ arrayStep
    // playmode = 1;
    Taster = 0;
    Serial.println("Clear ");
  }*/
  if (currentMillis - previousMillis3 > 2000) // Taster Status clear
  {
    Taster = 0;
    //Serial.println("restart ");
  }
}
