#include <NewPing.h>

#define PING_PIN 12
#define MAX_DISTANCE 200
#define LED_PIN 13

NewPing sonar(PING_PIN, PING_PIN, MAX_DISTANCE);

void setup()
{
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
}

void loop()
{
  delay(500);
  unsigned int uS = sonar.ping();
  unsigned int distance_cm = uS / US_ROUNDTRIP_CM;
  Serial.print("Ping: ");
  Serial.print(distance_cm);
  Serial.println("cm");
  if (distance_cm < 50)
    digitalWrite(LED_PIN, HIGH);
  else
    digitalWrite(LED_PIN, LOW);
}
