#include <DHT.h>
#include <NewPing.h>
#include <SoftwareSerial.h>
#include <SimpleModbusSlaveSoftwareSerial.h>

#define DHT_TYPE DHT22

const int DHT_PIN = 2;

const int BAY_1_PING_PIN = 12;
const int BAY_1_LED_PIN = 13;
const int BAY_1_DOOR_CONTROL_PIN = 11;

const int BAY_2_PING_PIN = 9;
const int BAY_2_LED_PIN = 10;
const int BAY_2_DOOR_CONTROL_PIN = 8;

const int MAX_DISTANCE = 200;

NewPing bay1(BAY_1_PING_PIN, BAY_1_PING_PIN, MAX_DISTANCE);
NewPing bay2(BAY_2_PING_PIN, BAY_2_PING_PIN, MAX_DISTANCE);

DHT dht(DHT_PIN, DHT_TYPE);

enum
{
  HUMIDITY,
  TEMPERATURE_C,
  TEMPERATURE_F,
  BAY_1_DISTANCE_CM,
  BAY_2_DISTANCE_CM,
  BAY_1_LED_STATE,
  BAY_2_LED_STATE,
  BAY_1_DOOR_CONTROL,
  BAY_2_DOOR_CONTROL,
  TOTAL_ERRORS,
  TOTAL_REGISTERS_SIZE
};

unsigned int holding_registers[TOTAL_REGISTERS_SIZE];

void setup()
{
  Serial.begin(115200);
  pinMode(BAY_1_LED_PIN, OUTPUT);
  pinMode(BAY_2_LED_PIN, OUTPUT);
  digitalWrite(BAY_1_DOOR_CONTROL_PIN, HIGH);
  digitalWrite(BAY_2_DOOR_CONTROL_PIN, HIGH);
  pinMode(BAY_1_DOOR_CONTROL_PIN, OUTPUT);
  pinMode(BAY_2_DOOR_CONTROL_PIN, OUTPUT);
  
  Serial.println("Configuring Modbus");
  modbus_configure(9600, 100, 1, TOTAL_REGISTERS_SIZE);
  
  Serial.println("Starting DHT");
}

void loop()
{
  delay(1000);
  unsigned int bay1_uS = bay1.ping();
  unsigned int bay1_distance_cm = 10; //bay1_uS / US_ROUNDTRIP_CM;
  unsigned int bay2_uS = bay2.ping();
  unsigned int bay2_distance_cm = 20; //bay2_uS / US_ROUNDTRIP_CM;
  
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
    Serial.print(" *C");
    Serial.print("\t");
    Serial.print("Temperature: "); 
    Serial.print(temperature * 1.8 + 32);
    Serial.println(" *F");
  }
    
  holding_registers[TOTAL_ERRORS] = modbus_update(holding_registers);
  holding_registers[HUMIDITY] = humidity * 100;
  holding_registers[TEMPERATURE_C] = temperature * 100;
  holding_registers[TEMPERATURE_F] = (temperature * 1.8 + 32) * 100;
  holding_registers[BAY_1_DISTANCE_CM] = bay1_distance_cm;
  holding_registers[BAY_2_DISTANCE_CM] = bay2_distance_cm;
  holding_registers[BAY_1_LED_STATE] = digitalRead(BAY_1_LED_PIN);
  holding_registers[BAY_2_LED_STATE] = digitalRead(BAY_2_LED_PIN);

  if (holding_registers[BAY_1_DOOR_CONTROL] > 0)
  {
    Serial.println("Bay 1 Door Control Set");
    digitalWrite(BAY_1_DOOR_CONTROL_PIN, LOW);
    holding_registers[BAY_1_DOOR_CONTROL] = 0;
    delay(250);
    digitalWrite(BAY_1_DOOR_CONTROL_PIN, HIGH);
  }
  else
  {
    Serial.println("Bay 1 Door Control Reset");
  }
  if (holding_registers[BAY_2_DOOR_CONTROL] > 0)
  {
    Serial.println("Bay 2 Door Control Set");
    digitalWrite(BAY_2_DOOR_CONTROL_PIN, LOW);
    holding_registers[BAY_2_DOOR_CONTROL] = 0;
    delay(250);
    digitalWrite(BAY_2_DOOR_CONTROL_PIN, HIGH);
  }
  else
  {
    Serial.println("Bay 2 Door Control Reset");
  }
    
  
  Serial.print("Total Errors: ");
  Serial.println(holding_registers[TOTAL_ERRORS]);
  Serial.print("Humidity (% * 100): ");
  Serial.println(holding_registers[HUMIDITY]);
  Serial.print("Temperature (*C * 100): ");
  Serial.println(holding_registers[TEMPERATURE_C]);
  Serial.print("Temperature (*F * 100): ");
  Serial.println(holding_registers[TEMPERATURE_F]);
  Serial.print("Bay 1 Distance (cm): ");
  Serial.println(holding_registers[BAY_1_DISTANCE_CM]);
  Serial.print("Bay 2 Distance (cm): ");
  Serial.println(holding_registers[BAY_2_DISTANCE_CM]);
  Serial.print("Bay 1 LED Status: ");
  Serial.println(holding_registers[BAY_1_LED_STATE]);
  Serial.print("Bay 2 LED Status: ");
  Serial.println(holding_registers[BAY_2_LED_STATE]);
}
