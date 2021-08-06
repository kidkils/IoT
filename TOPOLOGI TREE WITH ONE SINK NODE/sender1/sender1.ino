#include <espnow.h>
#include <ESP8266WiFi.h>
#include <Adafruit_BME280.h>
#include <Adafruit_Sensor.h>
#include "DHT.h"

// Set your Board ID (ESP32 Sender #1 = BOARD_ID 1, ESP32 Sender #2 = BOARD_ID 2, etc)
#define BOARD_ID 1


//MAC Address of the receiver 
uint8_t broadcastAddress[] = {0x40, 0xF5, 0x20, 0x3E, 0x88, 0x48};

const int soilMoist_pin = A0;

#define rain_pin D0

//led pin
#define redLed D5
#define blueLed D6

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

#define DHTPIN D7 //DHT PIN OUT
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE);

#define SEALEVELPRESSURE_HPA (1013.25)
Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK); 

//Structure example to send data
//Must match the receiver structure
typedef struct struct_message {
    int id;
    float temp;
    float hum;
    float pres;
    float alt;
    float temp11;
    float hum11;
    float soilmoist;
    float rain;
    int readingId;
} struct_message;

//Create a struct_message called myData
struct_message myData;

unsigned long previousMillis = 0;   // Stores last time temperature was published
const long interval = 10000;        // Interval at which to publish sensor readings

unsigned int readingId = 0;

// Insert your SSID
constexpr char WIFI_SSID[] = "Agus";

int32_t getWiFiChannel(const char *ssid) {
  if (int32_t n = WiFi.scanNetworks()) {
    for (uint8_t i=0; i<n; i++) {
      if (!strcmp(ssid, WiFi.SSID(i).c_str())) {
        return WiFi.channel(i);
      }
    }
  }
  return 0;
}

void initBME(){
  if (!bme.begin(0x76)) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }
}

float readTemperature() {
  float temp = bme.readTemperature();
  return temp;
}

float readHumidity() {
  float hum = bme.readHumidity();
  return hum;
}

float readPressure() {
  float pres = bme.readPressure() / 100.0F;
  return pres;
}

float readAltitude() {
  float alt = bme.readAltitude(SEALEVELPRESSURE_HPA);
  return alt;
}

float readTemperature11() {
  float temp11 = dht.readTemperature();
  return temp11;
}

float readHumidity11() {
  float hum11 = dht.readHumidity();
  return hum11;
}

float readSoilMoist() {
  float soilmoist = ( 100.00 - ( (analogRead(soilMoist_pin)/1023.00) * 100.00 ) );
  return soilmoist;
}

float readRain() {
  float rain = analogRead(rain_pin);
  return rain;
}


// Callback when data is sent
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0){
    Serial.println("Delivery success");
    digitalWrite(blueLed, HIGH);
    digitalWrite(redLed, LOW);
  }
  else{
    Serial.println("Delivery fail");
    digitalWrite(redLed, HIGH);
    digitalWrite(blueLed, LOW);
  }
}
 
void setup() {
  //Init Serial Monitor
  Serial.begin(115200);
  initBME(); 

  pinMode (redLed, OUTPUT);
  pinMode (blueLed, OUTPUT);
  
  pinMode(DHTPIN, INPUT);
  dht.begin();
  
  // Set device as a Wi-Fi Station and set channel
  WiFi.mode(WIFI_STA);

  int32_t channel = getWiFiChannel(WIFI_SSID);

  WiFi.printDiag(Serial); // Uncomment to verify channel number before
  wifi_promiscuous_enable(1);
  wifi_set_channel(channel);
  wifi_promiscuous_enable(0);
  WiFi.printDiag(Serial); // Uncomment to verify channel change after

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
   esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);

  esp_now_register_send_cb(OnDataSent);
  
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
}
 
void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    // Save the last time a new reading was published
    previousMillis = currentMillis;
    //Set values to send
    myData.id = BOARD_ID;
    myData.temp = readTemperature();
    myData.temp11 = readTemperature11();
    myData.hum = readHumidity();
    myData.hum11 = readHumidity11();
    myData.pres = readPressure();
    myData.alt = readAltitude();
    myData.soilmoist = readSoilMoist();
    myData.rain = readRain();
    myData.readingId = readingId++;
     
    esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));

    Serial.print("loop");
  }
}
