// TestSuite.pde
//
// Test Suite for Roomba class
//
// Runs on a Mega with the Create connected to Serial1
// Does not run any motors.
//

#include <Roomba.h>

// Defines the Roomba instance and the HardwareSerial it connected to
Roomba roomba(&Serial1);

void setup()
{
    Serial.begin(9600);
}

unsigned int errors = 0;

void error(const char *m)
{
  Serial.print("Error: ");
  Serial.println(m);
  errors++;
}

void check(boolean t, const char *m)
{
  if (!t)
    error(m);
}
bool connected=false;
void loop()
{
  
  uint8_t buf[10];
bool ret;
  while (!connected)
    {
        roomba.start();
        roomba.fullMode();
        roomba.drive(0, 0); // Stop
        Serial.flush();
        // Try to read a sensor. If that succeeds, we are connected
        ret = roomba.getSensors(Roomba::SensorVoltage, buf, 2);
        connected = ret;
    }
  
  roomba.leds(ROOMBA_MASK_LED_PLAY, 255, 255);
  
  roomba.leds(ROOMBA_MASK_LED_NONE, 0, 0);

  //bool ret = roomba.getSensors(7, buf, 1);
  
  check(ret == 1, "getSensors");
 
  uint8_t sensorIds[] = {7, 8, 9, 10 };
  ret = roomba.getSensorsList(sensorIds, sizeof(sensorIds), buf, 4);
  check(ret == 1, "getSensorsList");
  
  uint8_t s[] = { 131, 131, 131, 131 };
  roomba.script(s, sizeof(s));  
  roomba.playScript();
  uint8_t count = roomba.getScript(s, sizeof(s));
  check(count == 4, "script");
  roomba.script(NULL, 0);
  count = roomba.getScript(s, sizeof(s));
  check(count == 0, "script 2");
    
  // 2 bytes per note
  uint8_t song[] = {62, 12, 66, 12, 69, 12, 74, 36};
  roomba.song(0, song, sizeof(song));
  roomba.playSong(0);
  roomba.driveDirect(25, 25) ;
  delay(1000);
  roomba.drive(0, 0); // Stop
  // All done
  Serial.print("Done. Errors: ");
  Serial.println(errors, DEC);
  
  pinMode(13, OUTPUT);
  if (errors)
  {
    // Errors: Blink twice
    digitalWrite(13, HIGH);
    delay(500);
    digitalWrite(13, LOW);
    delay(500);
    digitalWrite(13, HIGH);
    delay(500);
    digitalWrite(13, LOW);
  }
  else
  {
    // OK, Blink once
    digitalWrite(13, HIGH);
    delay(500);
    digitalWrite(13, LOW);
  }

  connected = false;
}

  
