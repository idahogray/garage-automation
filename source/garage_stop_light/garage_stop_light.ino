#include <NewPing.h>

const int BAY_1_PING_PIN = 12;
const int BAY_1_LED_PIN = 13;

const int BAY_2_PING_PIN = 10;
const int BAY_2_LED_PIN = 11;

const int MAX_DISTANCE = 200;

NewPing bay1(BAY_1_PING_PIN, BAY_1_PING_PIN, MAX_DISTANCE);
NewPing bay2(BAY_2_PING_PIN, BAY_2_PING_PIN, MAX_DISTANCE);

void setup()
{
  Serial.begin(115200);
  pinMode(BAY_1_LED_PIN, OUTPUT);
  pinMode(BAY_2_LED_PIN, OUTPUT);
}

void loop()
{
  delay(500);
  unsigned int bay1_uS = bay1.ping();
  unsigned int bay1_distance_cm = bay1_uS / US_ROUNDTRIP_CM;
  unsigned int bay2_uS = bay2.ping();
  unsigned int bay2_distance_cm = bay2_uS / US_ROUNDTRIP_CM;
  Serial.print("Bay 1 Ping: ");
  Serial.print(bay1_distance_cm);
  Serial.println("cm");
  Serial.print("Bay 2 Ping: ");
  Serial.print(bay2_distance_cm);
  Serial.println("cm");
  if (bay1_distance_cm < 50)
    digitalWrite(BAY_1_LED_PIN, HIGH);
  else
    digitalWrite(BAY_1_LED_PIN, LOW);
    
  if (bay2_distance_cm < 50)
    digitalWrite(BAY_2_LED_PIN, HIGH);
  else
    digitalWrite(BAY_2_LED_PIN, LOW);
}
