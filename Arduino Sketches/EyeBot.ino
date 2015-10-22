#include <Servo.h>

enum direction { FORWARD, BACKWARD };

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
#define shoulderMax 1900
#define shoulderMin 1100
#define elbowMax 2100
#define elbowMin 1300
#define wristMax 1900
#define wristMin 1100
#define clawMax 1900
#define clawMin 1100

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

int Byte1Received;
int Byte2Received;
char val;         // variable to receive data from the serial port
byte data = 0; //value to transmit, binary 10101010
byte mask = 1; //our bitmask
direction movement_direction = FORWARD; //default movement direction

/****** SETUP: RUNS ONCE ******/
void setup()   
{
  Serial.begin(9600);  
  Serial.println("--- EyeBot Arduino is set up ---");
  Serial.println(" Please input a command in the form [servo]:[action] ");
  Serial.println("\n\tServos: pl, sh, el, wr, cl");  
  Serial.println("\tActions: right, left, up, down, open, close"); 

  platformLocation = 1500;
  shoulderLocation = 1700;
  elbowLocation = 1800;
  wristPitchLocation = 1500;
  wristRotationLocation = 1500;
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

    val = data;
    int movement = 0;
    movement_direction = direction(command[5]);
    if (movement_direction == FORWARD) {
      Serial.println("FORWARD");
      movement = servoSpeed;
    } else {
      Serial.println("BACKWARD");
      movement = -servoSpeed;
    }
    
    if (!command[4] && !command[3]) {  //simple servo movements
      Serial.println("simple");

      byte command = data & 7;

      switch(command) {

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

      platform.writeMicroseconds(platformLocation);
      shoulder.writeMicroseconds(shoulderLocation);
      elbow.writeMicroseconds(elbowLocation);
      wrist_pitch.writeMicroseconds(wristPitchLocation);
      wrist_rotation.writeMicroseconds(wristRotationLocation);
      claw.writeMicroseconds(clawLocation);
      
    }
    else if (!command[4] && command[3]) {  //complex movements
      Serial.println("complex");


      
    }
    else if (command[4]) {
      Serial.println("roomba");

      
    }
  }
}


