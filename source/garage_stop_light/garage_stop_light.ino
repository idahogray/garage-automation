#include <DHT.h>
#include <NewPing.h>
#include <SoftwareSerial.h>
#include <SimpleModbusSlaveSoftwareSerial.h>

#define DHT_TYPE DHT22

const int DHT_PIN = 2;

const int BAY_1_PING_PIN = 12;
const int BAY_1_LED_PIN = 13;

const int BAY_2_PING_PIN = 10;
const int BAY_2_LED_PIN = 11;

const int MAX_DISTANCE = 200;

NewPing bay1(BAY_1_PING_PIN, BAY_1_PING_PIN, MAX_DISTANCE);
NewPing bay2(BAY_2_PING_PIN, BAY_2_PING_PIN, MAX_DISTANCE);

DHT dht(DHT_PIN, DHT_TYPE);

enum
{
  HUMIDITY,
  TEMPERATURE,
  BAY_1_DISTANCE_CM,
  BAY_2_DISTANCE_CM,
  BAY_1_LED_STATE,
  BAY_2_LED_STATE,
  TOTAL_ERRORS,
  TOTAL_REGISTERS_SIZE
};

unsigned int holding_registers[TOTAL_REGISTERS_SIZE];

void setup()
{
  Serial.begin(115200);
  pinMode(BAY_1_LED_PIN, OUTPUT);
  pinMode(BAY_2_LED_PIN, OUTPUT);
  
  Serial.println("Configuring Modbus");
  modbus_configure(9600, 100, 1, TOTAL_REGISTERS_SIZE);
  
  Serial.println("Starting DHT");
}

void loop()
{
  delay(1000);
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
  
  if (bay1_distance_cm < 50 && bay1_distance_cm > 0)
    digitalWrite(BAY_1_LED_PIN, HIGH);
  else
    digitalWrite(BAY_1_LED_PIN, LOW);
    
  if (bay2_distance_cm < 50 && bay2_distance_cm > 0)
    digitalWrite(BAY_2_LED_PIN, HIGH);
  else
    digitalWrite(BAY_2_LED_PIN, LOW);
    
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  
  if (isnan(temperature) || isnan(humidity))
  {
    Serial.println("Failed to Read from DHT");
  }
  else
  {
    Serial.print("Humidity: "); 
    Serial.print(humidity);
    Serial.print(" %\t");
    Serial.print("Temperature: "); 
    Serial.print(temperature);
    Serial.println(" *C");
  }
    
  holding_registers[TOTAL_ERRORS] = modbus_update(holding_registers);
  holding_registers[HUMIDITY] = humidity * 100;
  holding_registers[TEMPERATURE] = temperature * 100;
  holding_registers[BAY_1_DISTANCE_CM] = bay1_distance_cm;
  holding_registers[BAY_2_DISTANCE_CM] = bay2_distance_cm;
  holding_registers[BAY_1_LED_STATE] = digitalRead(BAY_1_LED_PIN);
  holding_registers[BAY_2_LED_STATE] = digitalRead(BAY_2_LED_PIN);
  
  Serial.print("Total Errors: ");
  Serial.println(holding_registers[TOTAL_ERRORS]);
  Serial.print("Humidity: ");
  Serial.println(holding_registers[HUMIDITY]);
  Serial.print("Temperature (*C): ");
  Serial.println(holding_registers[TEMPERATURE]);
  Serial.print("Bay 1 Distance (cm): ");
  Serial.println(holding_registers[BAY_1_DISTANCE_CM]);
  Serial.print("Bay 2 Distance (cm): ");
  Serial.println(holding_registers[BAY_2_DISTANCE_CM]);
  Serial.print("Bay 1 LED Status: ");
  Serial.println(holding_registers[BAY_1_LED_STATE]);
  Serial.print("Bay 2 LED Status: ");
  Serial.println(holding_registers[BAY_2_LED_STATE]);
}
