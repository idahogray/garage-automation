#include <NewPing.h>

#define PING_PIN 12
#define MAX_DISTANCE 200

NewPing sonar(PING_PIN, PING_PIN, MAX_DISTANCE);

void setup()
{
  Serial.begin(115200);
}

void loop()
{
  delay(50);
  unsigned int uS = sonar.ping();
  Serial.print("Ping: ");
  Serial.print(uS / US_ROUNDTRIP_CM);
  Serial.println("cm");
}
