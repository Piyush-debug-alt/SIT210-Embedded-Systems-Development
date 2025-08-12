#include <WiFiNINA.h>
#include "DHT.h"
#include "ThingSpeak.h"

#define SENSOR_PIN 2        
#define SENSOR_TYPE DHT22   

char wifiName[] = "Motorola edge 50 fusion";     
char wifiPassword[] = "Piyush@2005"; 

WiFiClient wifiClient;
DHT tempSensor(SENSOR_PIN, SENSOR_TYPE);

unsigned long channelID = 3032934; 
const char *writeAPIKey = "JVZZPYMBEEN0VXAL"; 

void setup() {
  Serial.begin(9600);
  tempSensor.begin();
  WiFi.begin(wifiName, wifiPassword);
  ThingSpeak.begin(wifiClient);

  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");
}

void loop() {
  float temperatureValue = tempSensor.readTemperature();

  if (isnan(temperatureValue)) {
    Serial.println("Error: Unable to read from sensor!");
    return;
  }

  Serial.print("Temp: ");
  Serial.println(temperatureValue);

  ThingSpeak.setField(1, temperatureValue);
  int sendStatus = ThingSpeak.writeFields(channelID, writeAPIKey);

  if (sendStatus == 200) {
    Serial.println("Data uploaded to ThingSpeak");
  } else {
    Serial.print("Upload failed, code: ");
    Serial.println(sendStatus);
  }

  delay(30000); 
}
