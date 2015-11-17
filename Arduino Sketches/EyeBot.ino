#include <Servo.h>          // import the servo library
#include <math.h>           // import the math library
#include <SoftwareSerial.h> // import the serial library

Servo platform;
Servo shoulder;
Servo elbow;
Servo wrist_pitch;
Servo wrist_rotation;
Servo claw;

/*** PORTS ***/
#define platformPort 8
#define shoulderPort 9
#define elbowPort 10
#define wristPitchPort 11
#define wristRotationPort 12
#define clawPort 13

/** LIMITATIONS **/
#define platformMax 2100
#define platformMin 800
#define shoulderMax 2300
#define shoulderMin 700
#define elbowMax 2400
#define elbowMin 600
#define wristPitchMax 1900
#define wristPitchMin 1100
#define wristRotationMax 1900
#define wristRotationMin 1100
#define clawMax 2250
#define clawMin 550

/*** SERVO LOCATIONS ***/
int platformLocation;
int shoulderLocation;
int elbowLocation;
int wristPitchLocation;
int wristRotationLocation;
int clawLocation;

#define servoSpeed 10
#define servoDelay 10

/*
 * Operating Limitations
 * 
 * Platform:  -
 * Shoulder:  900 - 2100
 * Elbow:     900 - 2100
 * Wrist:     0 - 0
 * Claw:      0 - 0
 * 
 */

//SoftwareSerial Bluetooth(2, 3);  // RX, TX for bluetooth
int Byte1Received;
int Byte2Received;
char val;         // variable to receive data from the serial port
byte data = 0; //value to transmit, binary 10101010
byte mask = 1; //our bitmask

/****** SETUP: RUNS ONCE ******/
void setup()   
{
  
  Serial.begin(9600);  
  Serial.println("--- EyeBot Arduino is set up ---");

  platformLocation = 1500;
  shoulderLocation = 1500;
  elbowLocation = 2050;
  wristPitchLocation = 1500;
  wristRotationLocation = 1650;
  clawLocation = 1500;

  platform.attach(platformPort);
  shoulder.attach(shoulderPort);
  elbow.attach(elbowPort);
  wrist_pitch.attach(wristPitchPort);
  wrist_rotation.attach(wristRotationPort);
  claw.attach(clawPort);

  platform.writeMicroseconds(platformLocation);
  shoulder.writeMicroseconds(shoulderLocation);
  elbow.writeMicroseconds(elbowLocation);
  wrist_pitch.writeMicroseconds(wristPitchLocation);
  wrist_rotation.writeMicroseconds(wristRotationLocation);
  claw.writeMicroseconds(clawLocation);

  //empty queue
  while (Serial.available()) {
    Serial.read();
    delay(10);
  }
}

/****** LOOP: RUNS CONSTANTLY ******/
void loop()   
{
  if (Serial.available() > 0)
  {
    data = Serial.read();         // read it and store it in 'val'
    val = data;
    Serial.print("Value Received: ");
    Serial.print(data, BIN);
    Serial.print(" ");
    Serial.println(val);

    bool command[8];
    mask = 00000001;
    for (int i = 0; i < 8; ++i) {
        command[i] = data & mask;
        mask <<= 1;
    }

    byte command_standard = data & 64;
    byte command_direction = data & 32;
    byte command_type = data & 24;
    byte command_number = data & 7;

    int movement = 0;
    if (command_direction == 32) {
      Serial.println("FORWARD");     //lowercase
      movement = servoSpeed;
    } else {
      Serial.println("BACKWARD");    //uppercase
      movement = -servoSpeed;
    }
    

   if (command_type == 0) {
    
      switch(command_number) {

        case 0:   //nothing
          break;
        case 1:   //platform
          platformLocation += movement;
          Serial.println("platform");
          break;
        case 2:
          shoulderLocation += movement;
          Serial.println("shoulder");
          break;
        case 3:
          elbowLocation += movement;
          Serial.println("elbow");
          break;
        case 4:
          wristPitchLocation += movement;
          Serial.println("wristPitch");
          break;
        case 5:
          wristRotationLocation += movement;
          Serial.println("wristRotation");
          break;
        case 6:
          clawLocation += movement;
          Serial.println("claw");
          break;
        case 7:   //nothing
          break;
      }

    }
    else if (command_type == 8) {  //complex movements
      Serial.println("complex");

      double shoulder_rad = (shoulderLocation - shoulderMin) / (shoulderMax-shoulderMin) * PI;
      double elbow_rad = PI - (elbowLocation - elbowMin) / (elbowMax-elbowMin) * PI;
      double elbow_relative = elbow_rad + shoulder_rad - PI;
      double wrist_rad = (wristPitchLocation - wristPitchMin) / (wristPitchMax-wristPitchMin) * PI;
      double shoulder_inc = .03;
      switch(command_number) {

        case 0:   //nothing
          break;
        case 1:   //platform
        {
          Serial.println("Forward / Backward");


          //double asin_value = asin();
          
          double L1 = 9;
          double L2 = 5;
          elbow_relative += asin((L1*sin(shoulder_rad)+L2*sin(elbow_relative) - L1*sin(shoulder_rad+shoulder_inc))/L2);
          shoulder_rad += shoulder_inc;
          elbow_rad = elbow_relative - shoulder_rad + PI;
         
          break;
        }
        case 2:
        {
          Serial.println("Up / Down");
          break;
        }
        case 3:
          Serial.println("Follow Claw Forward / Backward");
          break;
        default:
          break;
      }



      shoulderLocation  = ((shoulder_rad * (shoulderMax - shoulderMin) / PI) + shoulderMin);
      elbowLocation     = (((PI - elbow_rad) * (elbowMax - elbowMin) / PI) + elbowMin);
//      wrist_pitch.writeMicroseconds((wrist_rad * (wristPitchMax - wristPitchMin) / PI) + wristPitchMin);

      
    }
    else if (command_type == 16 || command_type == 24) {
      Serial.println("roomba");
      
    }

    platform.writeMicroseconds(platformLocation);
    Serial.print("Platform: ");
    Serial.println(platformLocation);
    shoulder.writeMicroseconds(shoulderLocation);
    Serial.print("Shoulder: ");
    Serial.println(shoulderLocation);
    elbow.writeMicroseconds(elbowLocation);
    Serial.print("Elbow: ");
    Serial.println(elbowLocation);
    wrist_pitch.writeMicroseconds(wristPitchLocation);
    Serial.print("Wrist Pitch: ");
    Serial.println(wristPitchLocation);
    wrist_rotation.writeMicroseconds(wristRotationLocation);
    Serial.print("Wrist Rotation: ");
    Serial.println(wristRotationLocation);
    claw.writeMicroseconds(clawLocation);
    Serial.print("Claw: ");
    Serial.println(clawLocation);
  }
}

