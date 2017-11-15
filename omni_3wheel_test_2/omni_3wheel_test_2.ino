/* Hey, this is the program for the 3 wheel omnidrive for ASME UNLV
 *  As you may notice, if you are certain people in the group, this is orginally Patrick's 4 wheel code
 *  Now I'm more responsible, I'll be commenting on everything. Cuz I aint Patrick. It'll take some time,
 *  but I'll get it done. (((It's not fully commented as of 11/13/2017)))
 *  
 *  SO the basic fundamentals of the drive train is having one corner be the front that two of the wheels
 *  drive towards. Straffing will be done by having the dead weight actually be turning while the other two main wheels
 *  cancel out each other. Thus we straff using that and we'll just have all 3 turn concurrent to get a nice turning stuffs.
 *  
*/


#include <PS2X_lib.h> //PS2 Controller interface library

PS2X ps2x; //create ps2x object of PS2X Class

//This section is exactly like the 4 wheeler just with the 4th wheel removed.
//The 3 other motors all connect through the same means as their counterparts.

int error; //This value is for checking if the controller has come across an issue.
byte vibrate = 0;

//Motor One's stuff
int enable1 = 11;
int pow1 = 12;
int dir1 = 13;

//Motor Two's stuff
int enable2 = 8;
int pow2 = 9;
int dir2 = 10;

//Motor Three's stuff
int enable3 = 5;
int pow3 = 6; 
int dir3 = 7;


void setup() {
    Serial.begin(250000);
    //So, this is the setup. First things first is that we check the controller status.
    //The error code below will return a value of 1-3 if something has run amok.
    error = ps2x.config_gamepad(50, 51, 52, 53, true, true); //setup pins and settings:  
    //GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error

    if (error == 0) { // So if the returned error value is zero then everything is fine...for now. This will let us peacefully
                      //proceed to the loop where we run everything.
      Serial.println("Found Controller, configured successful");
      Serial.println("Try out all the buttons, X will vibrate the controller, faster as you press harder;");
      Serial.println("holding L1 or R1 will print out the analog stick values.");
      Serial.println("Go to www.billporter.info for updates and to report bugs.");
    }

    else if (error == 1)//Error value of one is that the controller is either disconnected or improperly connected.
      Serial.println("No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips");

    else if (error == 2)// Error value of two is that the controller is acting funky. Best advice so far, just restart the robot. This happens a bit.
      Serial.println("Controller found but not accepting commands. see readme.txt to enable debug. Visit www.billporter.info for troubleshooting tips");

    else if (error == 3)//Error value of three. This means you just chuck the current controller into the bin and buy a new one.
      Serial.println("Controller refusing to enter Pressures mode, may not support it. ");

    Serial.print(ps2x.Analog(1), HEX);

  //Crappy scan of the controller's type.(((DON'T KNOW IF THIS WORKS...DIDN'T CHECK PATRICK'S TEST SCREEN)))
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

}
// This here is the loop. Everything that we want the operator to control is stuffed into here, with some calls.
void loop() {
  //Serial.print("running");
  if (error != 0)//skip loop if no controller found
  {//So this relates to the error stuff above. Just checks to see if the controller is operating as it should.
    Serial.println("\t\t no controller");
    //This turns off all the drivers if the controller has an issue.
    digitalWrite(enable1, LOW);
    digitalWrite(enable2, LOW);
    digitalWrite(enable3, LOW); 
    return;
  }
  
  //Enables all the drivers if there is no controller issue detected.
  digitalWrite(enable1, HIGH);
  digitalWrite(enable2, HIGH);
  digitalWrite(enable3, HIGH); 

  //read controller's inputs.
  ps2x.read_gamepad(false, vibrate);

  //get joystick readings, 0-255 centered on 128
  int LX = ps2x.Analog(PSS_LX);   //0=left   255=right
  int LY = ps2x.Analog(PSS_LY);   //0=forward   255=backward
  int RX = ps2x.Analog(PSS_RX);   //0=left   255=right
  int RY = ps2x.Analog(PSS_RY);   //0=forward   255=backward

  int straight = 0;
  //int straight2 = 0;
  //int straight3 = 0;
  
  int side = 0;
  //int side2 = 0;
  //int side3 = 0;
  
  int turn = 0;
  //int turn2 = 0;
  //int turn3 = 0;

  //Print all the joystick values for user reading.
  Serial.print(LX);
  Serial.print("\t");
  Serial.print(LY);
  Serial.print("\t");
  Serial.print(RX);
  Serial.print("\t");
  Serial.print(RY);
  Serial.print("\t");

  //STRAIGHT FACTORS
  //Determine straight factor
  if(LY <= 130 && LY >= 125)  //joystick is centered verticaly
  {
      //do not change straight factor
  }
  else if(LY < 125)   //joystick is forward
  {
    //straight = -(128 - LY) * 100/127;   //straight factor scaled to 100 -
    straight = -((128) - LY) * 100/127;
  }
  else if(LY > 130)   //joystick is backward
  {
    straight = (LY - 128) *  100/127;  //straight factor scaled to 100 +
  }

  //Determine side factor
  if(LX <= 130 && LX >= 125)    //joystick is centered horizontally
  {
      //do not change side factor
  }
  else if(LX < 125)   //joystick is left
  {
    side = -(127 - LX) * 100/125;   //side factor scaled to 100 -
  }
  else if(LX > 130)   //joystick is right
  {
    side = (LX - 127) *  100/127;   //side factor scaled to 100 +
  }

  //Determine tunring factor
  if(RX <= 130 && RX >= 125)    //joystick is centered horizontally
  {
      //do not change turn factor
  }
  else if(RX < 125)   //joystick is left
  {
    turn = (127 - RX) *  100/127;    //turn factor scaled to 100 +
  }
  else if(RX > 130)   //joystick is right
  {
    turn = -(RX - 127) *  100/127;    //side factor scaled to 100 -
  }

  //displays the factors
  Serial.print(straight);
  Serial.print("\t");
  Serial.print(side);
  Serial.print("\t");
  Serial.print(turn);
  Serial.print("\n");

  //Here is where we set each motor's speed and direction.
  //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX  SPEED 1
  int speed1 = straight - (0.6*side) + turn;
  if(speed1 >= 0)
  {
    digitalWrite(dir1, HIGH);
  }
  else
  {
    digitalWrite(dir1, LOW);
  }
  
  speed1 = speed1 * 2.55;
  if(speed1 < 0)
  {
    speed1 = -speed1;
  }
  if(speed1 > 255)
  {
    analogWrite(pow1, 255);
  }
  else
  {
    analogWrite(pow1, speed1);
  }
  //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
  //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX  SPEED 2
  //Remember that motor 2 is the deadweight and that it runs for the straffing and the turning, but not if the direction is straight.
  int speed2 = side + turn;
  if(speed2 >= 0)
  {//Unlike the other motors, M2 only runs if turning or straffing.
    digitalWrite(dir2, HIGH);
  }
  else
  {
    digitalWrite(dir2, LOW);
  }

  //Serial.print(speed2);
  //Serial.print("\t");
  speed2 = speed2 * 2.55;
  if(speed2 < 0)
  {
    speed2 = -speed2;
  }
  //Serial.print(speed2);
  //Serial.print("\n");
  if(speed2 > 255)
  {
    analogWrite(pow2, 255);
  }
  else
  {
    analogWrite(pow2, speed2);
  }

  //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
  //XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX  SPEED 3
  int speed3 = -straight - (0.6*side) + turn;
  if(speed3 >= 0)
  {//should be just like Motor 1, just inverse.
    digitalWrite(dir3, HIGH);
  }
  else
  {
    digitalWrite(dir3, LOW);
  }
  
  speed3 = speed3 * 2.55;
  if(speed3 < 0)
  {
    speed3 = -speed3;
  }
  if(speed3 > 255)
  {
    analogWrite(pow3, 255);
  }
  else
  {
    analogWrite(pow3, speed3);
  }
}
