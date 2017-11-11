#include <PS2X_lib.h> //PS2 Controller interface library

/* This is the code for the three wheel omni drive robot. This current code is based off the original fourwheeler code by patrick.
   Things that need to be checked on: 
   DRIVE SYSTEM
   CONTROLLER INPUTS   -- REPLACE RY TO LY
   
                                   
*/
PS2X ps2x; //create ps2x object of PS2X Class

int error;
byte vibrate = 0;

int enable1 = 11;
int pow1 = 12;
int dir1 = 13;

int enable2 = 8;
int pow2 = 9;
int dir2 = 10;

int enable3 = 5;
int pow3 = 6;
int dir3 = 7;

//int enable4 = 2;
//int pow4 = 3;
//int dir4 = 4;


void setup() {
    Serial.begin(9600);

    //This checks the controller status
    //it'll give us some feedback if an error occurs
    error = ps2x.config_gamepad(50, 51, 52, 53, true, true); //setup pins and settings:  
    //GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error
    //
    
    if (error == 0) {
      Serial.println("Found Controller, configured successful");
      Serial.println("Try out all the buttons, X will vibrate the controller, faster as you press harder;");
      Serial.println("holding L1 or R1 will print out the analog stick values.");
      Serial.println("Go to www.billporter.info for updates and to report bugs.");
    }

    else if (error == 1)
      Serial.println("No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips");

    else if (error == 2)
      Serial.println("Controller found but not accepting commands. see readme.txt to enable debug. Visit www.billporter.info for troubleshooting tips");

    else if (error == 3)
      Serial.println("Controller refusing to enter Pressures mode, may not support it. ");

    Serial.print(ps2x.Analog(1), HEX);
  
  int type = ps2x.readType();
  switch (type) {
    case 0:
      Serial.println("Unknown Controller type");
      break;
    case 1:
      Serial.println("DualShock Controller Found");
      break;
    case 2:
      Serial.println("GuitarHero Controller Found");
      break;
  }


  //setup driver pins
  pinMode(enable1, OUTPUT);
  pinMode(pow1, OUTPUT);
  pinMode(dir1, OUTPUT);
  
  pinMode(enable2, OUTPUT);
  pinMode(pow2, OUTPUT);
  pinMode(dir2, OUTPUT); 

  pinMode(enable3, OUTPUT);
  pinMode(pow3, OUTPUT);
  pinMode(dir3, OUTPUT);

  //pinMode(enable4, OUTPUT);
  //pinMode(pow4, OUTPUT);
  //pinMode(dir4, OUTPUT);
}

void loop() {
  Serial.print("running");
  if (error != 0)//skip loop if no controller found
  {
    Serial.println("\t\t no controller");
    //disable driers
    digitalWrite(enable1, LOW);
    digitalWrite(enable2, LOW);
    digitalWrite(enable3, LOW);
    //digitalWrite(enable4, LOW); 
    return;
  }
  
  //enable drivers
  digitalWrite(enable1, HIGH);
  digitalWrite(enable2, HIGH);
  digitalWrite(enable3, HIGH);
  //digitalWrite(enable4, HIGH); 

  //read controller
  ps2x.read_gamepad(false, vibrate);


  int LX = ps2x.Analog(PSS_LX);
  int RX = ps2x.Analog(PSS_RX);
  int RY = ps2x.Analog(PSS_RY);

  int strait = 0;
  int strait1 = 0;
  int strait2 = 0;
  int strait3 = 0;
 
  int side = 0;
  int side1 = 0;
  int side2 = 0;
  int side3 = 0;
 
  int turn = 0;
  int turn1 = 0;
  int turn2 = 0;
  int turn3 = 0;
  

  //Determine strait factor
  if(RY <= 130 && RY >= 125)
  {
      //do not change strait factor
  }
  else if(RY < 125)
  {
    //strait = -(127 - RY) * 100/127;
    strait1 = -(127 - RY) * cos(150 - 0);
    strait2 = -(127 - RY) * cos(30 - 0);
    strait3 = -(127 - RY) * cos(270 - 0);
  }
  else if(RY > 130)
  {
    //strait = (RY - 127) *  100/127;
    strait1 = (RY - 127) * cos(150 - 0);
    strait2 = (RY - 127) * cos(30 - 0);
    strait3 = (RY - 127) * cos(270 - 0);
  }

  //Determine side factor
  if(RX <= 130 && RX >= 125)
  {
      //do not change strait factor
  }
  else if(RX < 125)
  {
    //strait = -(127 - RX) * 100/125;
    side1 = -(127 - RX) * cos(150 - 0);
    side2 = -(127 - RX) * cos(30 - 0);
    side3 = -(127 - RX) * cos(270 - 0);
  }
  else if(RX > 130)
  {
    //strait = (RX - 127) *  100/127;
    side1 = (RX - 127) * cos(150 - 0);
    side2 = (RX - 127) * cos(30 - 0);
    side3 = (RX - 127) * cos(270 - 0);
  }

  //Determine turning factor. This would be the robot turning at a center point.
  if(LX <= 130 && LX >= 125)
  {
      //do not change turn factor
  }
  else if(LX < 125)
  {
    //turn = -(127 - LX) *  100/127;
    turn1 = -(127 - LX) * cos(150 - 0);
    turn2 = -(127 - LX) * cos(30 - 0);
    turn3 = -(127 - LX) * cos(270 - 0);
  }
  else if(LX > 130)
  {
    //turn = (LX - 127) *  100/127;
    turn1 = (LX - 127) * cos(150 - 0);
    turn2 = (LX - 127) * cos(30 - 0);
    turn3 = (LX - 127) * cos(270 - 0);
  }

  //set each motor's direction and speed
  //speed1
  int speed1 = strait + side + turn;
  if(speed1 >= 0)
  {
    digitalWrite(dir1, HIGH);
  }
  else
  {
    digitalWrite(dir1, LOW);
  }
  
  speed1 = abs(speed1) * 255/100;
  if(speed1 > 255)
  {
    analogWrite(pow1, 255);
  }
  else
  {
    analogWrite(pow1, speed1);
  }
  
  //speed2
  int speed2 = strait - side + turn;
  if(speed2 >= 0)
  {
    digitalWrite(dir2, HIGH);
  }
  else
  {
    digitalWrite(dir2, LOW);
  }
  
  speed2 = abs(speed2) * 255/100;
  if(speed2 > 255)
  {
    analogWrite(pow2, 255);
  }
  else
  {
    analogWrite(pow2, speed2);
  }
  
  //speed3 This has to dictate that the 3rd wheel moves with the correct stuffs.
  int speed3 = -strait - side + turn;
  if(speed3 >= 0)
  {
    digitalWrite(dir3, HIGH);
  }
  else
  {
    digitalWrite(dir3, LOW);
  }
  
  speed3 = abs(speed3) * 255/100;
  if(speed3 > 255)
  {
    analogWrite(pow3, 255);
  }
  else
  {
    analogWrite(pow3, speed3);
  }

  //speed 4 DEFUNCT THIS IS CODE FOR THE 4 WHEEL
  /*int speed4 = -strait + side + turn;
  if(speed4 >= 0)
  {
    digitalWrite(dir4, HIGH);
  }
  else
  {
    digitalWrite(dir4, LOW);
  }
  speed4 = abs(speed4) * 255/100;
  if(speed4 > 255)
  {
    analogWrite(pow4, 255);
  }
  else
  {
    analogWrite(pow4, speed2);
  }
  */
}
