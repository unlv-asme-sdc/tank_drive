#include <PS2X_lib.h> //PS2 Controller interface library

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

int enable4 = 2;
int pow4 = 3;
int dir4 = 4;


void setup() {
    Serial.begin(250000);

    //check controller status
    error = ps2x.config_gamepad(50, 51, 52, 53, true, true); //setup pins and settings:  
    //GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error

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

  pinMode(enable4, OUTPUT);
  pinMode(pow4, OUTPUT);
  pinMode(dir4, OUTPUT);
}

void loop() {
  //Serial.print("running");
  if (error != 0)//skip loop if no controller found
  {
    Serial.println("\t\t no controller");
    //disable driers
    digitalWrite(enable1, LOW);
    digitalWrite(enable2, LOW);
    digitalWrite(enable3, LOW);
    digitalWrite(enable4, LOW); 
    return;
  }
  
  //enable drivers
  digitalWrite(enable1, HIGH);
  digitalWrite(enable2, HIGH);
  digitalWrite(enable3, HIGH);
  digitalWrite(enable4, HIGH); 

  //read controller
  ps2x.read_gamepad(false, vibrate);

  //get joystick readings, 0-255 centered on 128
  int LX = ps2x.Analog(PSS_LX);   //0=left   255=right
  int LY = ps2x.Analog(PSS_LY);   //0=forward   255=backward
  int RX = ps2x.Analog(PSS_RX);   //0=left   255=right
  int RY = ps2x.Analog(PSS_RY);   //0=forward   255=backward

  int straight = 0;
  int side = 0;
  int turn = 0;
  
  int driveL = 0;
  int driveR = 0;
  
  Serial.print(LX);
  Serial.print("\t");
  Serial.print(LY);
  Serial.print("\t");
  Serial.print(RX);
  Serial.print("\t");
  Serial.print(RY);
  Serial.print("\t");

  if(LY <= 130 && LY >= 125)
  {
    
  }
  else if(LY < 125)   //joystick is forward
  {
    driveL = -(128 - LY) * 100/127;   //straight factor scaled to 100 -
  }
  else if(LY > 130)   //joystick is backward
  {
    driveL = (LY - 128) *  100/127;  //straight factor scaled to 100 +
  }

  if(RY <= 130 && RY >= 125)
    {
    
    }
  else if(RY < 125)   //joystick is forward
  {
    driveR = -(128 - RY) * 100/127;   //straight factor scaled to 100 -
  }
  else if(RY > 130)   //joystick is backward
  {
    driveR = (RY - 128) *  100/127;  //straight factor scaled to 100 +
  }
  
  //display factors
  Serial.print(straight);
  Serial.print("\t");
  Serial.print(side);
  Serial.print("\t");
  Serial.print(turn);
  Serial.print("\t");
  Serial.print(driveL);
  Serial.print("\t");
  Serial.print(driveR);
  Serial.print("\n");

  //set each motor's direction and speed
  //speed1
  int speed1 = driveL;
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
  
  //speed2
  int speed2 = driveL;
  if(speed2 >= 0)
  {
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
  
  //speed3
  int speed3 = -driveR;
  if(speed3 >= 0)
  {
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

  //speed 4
  int speed4 = -driveR;
  if(speed4 >= 0)
  {
    digitalWrite(dir4, HIGH);
  }
  else
  {
    digitalWrite(dir4, LOW);
  }
  
  speed4 = speed4 * 2.55;
  if(speed4 < 0)
  {
    speed4 = -speed4;
  }
  if(speed4 > 255)
  {
    analogWrite(pow4, 255);
  }
  else
  {
    analogWrite(pow4, speed2);
  }
}
