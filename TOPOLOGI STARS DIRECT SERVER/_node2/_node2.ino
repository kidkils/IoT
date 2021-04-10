#include "DHT.h"
#include <Wire.h>
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

//led pin
#define redLed D5
#define greenLed D6

//soil moisture pin
const int soilMoist_pin = A0;

//rain pin
#define rain_pin D0

// assign the ESP8266 pins 
#define D1 5
#define D2 4
#define D4 2
#define D3 0

// assign the SPI bus to pins
#define BME_SCK D4 //SCL
#define BME_MISO D1 //SD0
#define BME_MOSI D3 //SDA
#define BME_CS D2 //CSB

//DHT11 pin sensor
#define DHTPIN D7 //DHT PIN OUT
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK); // software SPI

const char* ssid = "Agus";
const char* password = "nieAndra01";
const char* mqtt_server = "192.168.43.71";
const char* mqtt_user = "admin";
const char* mqtt_password = "12345coba";

WiFiClient espClient;
PubSubClient client(espClient);

unsigned long delayTime;

void setup() {
    Serial.begin(9600);

    pinMode (redLed, OUTPUT);
    pinMode (greenLed, OUTPUT);
  
    pinMode(DHTPIN, INPUT);
    dht.begin();
    
    setup_wifi();
    client.setServer(mqtt_server, 1883);
    
    Serial.println(F("BME280 test"));

    bool status;
    
    // default settings
    status = bme.begin();
    if (!status) {
        Serial.println("Could not find a valid BME280 sensor, check wiring!");
        while (1);
    }
    
    Serial.println("-- Default Test --");
    delayTime = 1000;

    Serial.println();
}

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client2", mqtt_user, mqtt_password)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
    
      // ... and resubscribe
      // client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void loop() { 
    printValues();
    delay(delayTime);
    client.loop();
}


void printValues() {
    if (!client.connected()) {
      reconnect();
    }

    //SENSOR RUIN READ
    float rain = analogRead(rain_pin);
    
    //SENSOR MOISTURE READ
    float soilMoisture;
    soilMoisture = ( 100.00 - ( (analogRead(soilMoist_pin)/1023.00) * 100.00 ) );
    
    //DHT11 SENSOR PRINT VALUE
    Serial.print("Kelembaban udara = ");
    Serial.print(dht.readHumidity());
    Serial.print("% ");
    
    Serial.print("Suhu = ");
    Serial.print(dht.readTemperature()); 
    Serial.println(" C ");

    //MOISTURE SENSOR PRINT VALUE
    Serial.print("Soil Moisture(in Percentage) = ");
    Serial.print(soilMoist_pin);
    Serial.println("%");
  
    //BME280 SENSOR PRINT VALUE
    Serial.print("Temperature = ");
    Serial.print(bme.readTemperature());
    Serial.println(" *C");

    Serial.print("Pressure = ");
    Serial.print(bme.readPressure() / 100.0F);
    Serial.println(" hPa");

    Serial.print("Approx. Altitude = ");
    Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
    Serial.println(" m");

    Serial.print("Humidity = ");
    Serial.print(bme.readHumidity());
    Serial.println(" %");

    //CONDITION OF SENSOR RUIN
    if(rain < 1){
    Serial.print("sedang hujan = ");
    Serial.print(rain);
    }
    else {
      Serial.print("tidak hujan = ");
      Serial.print(rain);
    }

    Serial.println();
    
    //HUMIDITY CLIENT PUBLISH
    if(client.publish("Humidity-node2", String(dht.readHumidity()).c_str() )){
    Serial.print("Humidity telah dikirm");
    digitalWrite(greenLed, HIGH);
    digitalWrite(redLed, LOW);
    } 
    else{
      Serial.print("Humidity gagal dikirm");
      digitalWrite(greenLed, LOW);
      digitalWrite(redLed, HIGH);
      Serial.println();
      client.connect("ESP8266Client2", mqtt_user, mqtt_password);
      client.publish("Humidity-node2", String(dht.readHumidity()).c_str() );
    }
    
    if(client.publish("Humidity-node2", String(bme.readHumidity()).c_str() )){
      Serial.print("Humidity telah dikirm");
      digitalWrite(greenLed, HIGH);
      digitalWrite(redLed, LOW);
    } 
    else{
      Serial.print("Humidity gagal dikirm");
      digitalWrite(greenLed, LOW);
      digitalWrite(redLed, HIGH);
      Serial.println();
      client.connect("ESP8266Client2", mqtt_user, mqtt_password);
      client.publish("Humidity-node2", String(bme.readHumidity()).c_str() );
    }

    //TEMPERATURE CLIENT PUBLISH
    if(client.publish("Temperature-node2", String(dht.readTemperature()).c_str() )){
      Serial.print("Temperature telah dikirm");
      digitalWrite(greenLed, HIGH);
      digitalWrite(redLed, LOW);
    } 
    else{
      Serial.print("Temperature gagal dikirm");
      digitalWrite(greenLed, LOW);
      digitalWrite(redLed, HIGH);
      Serial.println();
      client.connect("ESP8266Client2", mqtt_user, mqtt_password);
      client.publish("Temperature-node2", String(dht.readTemperature()).c_str() );
    }

    if(client.publish("Temperature-node2", String(bme.readTemperature()).c_str() )){
      Serial.print("Temperature telah dikirm");
      digitalWrite(greenLed, HIGH);
      digitalWrite(redLed, LOW);
    } 
    else{
      Serial.print("Temperature gagal dikirm");
      digitalWrite(greenLed, LOW);
      digitalWrite(redLed, HIGH);
      Serial.println();
      client.connect("ESP8266Client2", mqtt_user, mqtt_password);
      client.publish("Temperature-node2", String(bme.readTemperature()).c_str() );
    }

    //MOISTURE CLIENT PUBLISH
    if(client.publish("Moisture-node2", String(soilMoisture).c_str())){
      Serial.print("Moisture telah dikirm");
      digitalWrite(greenLed, HIGH);
      digitalWrite(redLed, LOW);
    } 
    else{
      Serial.print("Moisture gagal dikirm");
      digitalWrite(greenLed, LOW);
      digitalWrite(redLed, HIGH);
      Serial.println();
      client.connect("ESP8266Client2", mqtt_user, mqtt_password);
      client.publish("Moisture-node2", String(soilMoisture).c_str());
    }

    //RUIN CLIENT PUBLISH
    if(client.publish("Rain-node2", String(rain).c_str())){
      Serial.print("Rain telah dikirm");
      digitalWrite(greenLed, HIGH);
      digitalWrite(redLed, LOW);
    } 
    else{
      Serial.print("Rain gagal dikirm");
      digitalWrite(greenLed, LOW);
      digitalWrite(redLed, HIGH);
      Serial.println();
      client.connect("ESP8266Client2", mqtt_user, mqtt_password);
      client.publish("Rain-node2", String(rain).c_str());
    }

    //PRESSURE AIR CLIENT PUBLISH
    if(client.publish("Pressure-node2", String(bme.readPressure() / 100.0F).c_str())){
      Serial.print("Pressure telah dikirm");
      digitalWrite(greenLed, HIGH);
      digitalWrite(redLed, LOW);
    } 
    else{
      Serial.print("Pressure gagal dikirm");
      digitalWrite(greenLed, LOW);
      digitalWrite(redLed, HIGH);
      Serial.println();
      client.connect("ESP8266Client2", mqtt_user, mqtt_password);
      client.publish("Pressure-node2", String(bme.readPressure() / 100.0F).c_str());
    }
    
    //APPROX ALTITUDE CLIENT PUBLISH
    if(client.publish("Altitude-node2", String(bme.readAltitude(SEALEVELPRESSURE_HPA)).c_str())){
      Serial.print("Altitude telah dikirm");
      digitalWrite(greenLed, HIGH);
      digitalWrite(redLed, LOW);
    } 
    else{
      Serial.print("Altitude gagal dikirm");
      digitalWrite(greenLed, LOW);
      digitalWrite(redLed, HIGH);
      Serial.println();
      client.connect("ESP8266Client2", mqtt_user, mqtt_password);
      client.publish("Altitude-node2", String(bme.readAltitude(SEALEVELPRESSURE_HPA)).c_str());
    }
  
    Serial.println();
}
