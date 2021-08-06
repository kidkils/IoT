#include <espnow.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "ESPAsyncWebServer.h"
#include "ESPAsyncTCP.h"
#include <Arduino_JSON.h>
#include <Wire.h>

// Replace with your network credentials (STATION)
const char* ssid = "Agus";
const char* password = "nieAndra01";
const char* mqtt_server = "192.168.43.113";
const char* mqtt_user = "admin";
const char* mqtt_password = "12345coba";

//led pin
#define redLed D1
#define blueLed D3
#define greenLed D2

// Structure example to receive data
// Must match the sender structure
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
  unsigned int readingId;
} struct_message;

struct_message incomingReadings;

struct_message board1;
struct_message board2;
struct_message board3;
struct_message board4;
struct_message board5;

WiFiClient espClient;
PubSubClient client(espClient);

//JSONVar board;
//
//AsyncWebServer server(80);
//AsyncEventSource events("/events");

struct_message boardsStruct[5] = {board1, board2, board3, board4, board5};

// callback function that will be executed when data is received
void OnDataRecv(uint8_t * mac_addr, uint8_t *incomingData, uint8_t len) { 
  // Copies the sender mac address to a string
  char macStr[18];
  Serial.print("Packet received from: ");
  digitalWrite(blueLed, HIGH);
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.println(macStr);
  memcpy(&incomingReadings, incomingData, sizeof(incomingReadings));

  boardsStruct[incomingReadings.id-1].temp = incomingReadings.temp;
  boardsStruct[incomingReadings.id-1].hum = incomingReadings.hum;
  boardsStruct[incomingReadings.id-1].pres = incomingReadings.pres;
  boardsStruct[incomingReadings.id-1].alt = incomingReadings.alt;
  boardsStruct[incomingReadings.id-1].temp11 = incomingReadings.temp11;
  boardsStruct[incomingReadings.id-1].hum11 = incomingReadings.hum11;
  boardsStruct[incomingReadings.id-1].soilmoist = incomingReadings.soilmoist;
  boardsStruct[incomingReadings.id-1].rain = incomingReadings.rain;
  boardsStruct[incomingReadings.id-1].readingId = incomingReadings.readingId;
  
//  String jsonString = JSON.stringify(board);
//  events.send(jsonString.c_str(), "new_readings", millis());
  
  Serial.printf("Board ID %u: %u bytes\n", incomingReadings.id, len);
  Serial.printf("t value: %4.2f \n", boardsStruct[incomingReadings.id-1].temp);
  Serial.printf("h value: %4.2f \n", boardsStruct[incomingReadings.id-1].hum);
  Serial.printf("p value: %4.2f \n", boardsStruct[incomingReadings.id-1].pres);
  Serial.printf("a value: %4.2f \n", boardsStruct[incomingReadings.id-1].alt);
  Serial.printf("t11 value: %4.2f \n", boardsStruct[incomingReadings.id-1].temp11);
  Serial.printf("h11 value: %4.2f \n", boardsStruct[incomingReadings.id-1].hum11);
  Serial.printf("m value: %4.2f \n", boardsStruct[incomingReadings.id-1].soilmoist);
  Serial.printf("r value: %4.2f \n", boardsStruct[incomingReadings.id-1].rain);
  Serial.printf("readingID value: %d \n", boardsStruct[incomingReadings.readingId-1]);
  long rssi = WiFi.RSSI();
  Serial.print("RSSI:");
  Serial.print(rssi);
  Serial.print(" dBm");
  Serial.println();
}



void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);

  pinMode (redLed, OUTPUT);
  pinMode (blueLed, OUTPUT);
  pinMode (greenLed, OUTPUT);

  setup_wifi();

  client.setServer(mqtt_server, 1883);
  
  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);
}

void setup_wifi() {
    // Set the device as a Station and Soft Access Point simultaneously
  WiFi.mode(WIFI_AP_STA);
  
  // Set device as a Wi-Fi Station
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Setting as a Wi-Fi Station..");
  }
  
  Serial.print("Station IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Wi-Fi Channel: ");
  Serial.println(WiFi.channel());
}

//
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("NODE-SERVER", mqtt_user, mqtt_password)) {
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
    //Node Sensor 1//
    float Node1_Temperature = boardsStruct[0].temp;
    float Node1_Humidity = boardsStruct[0].hum;
    float Node1_Pressure = boardsStruct[0].pres;
    float Node1_Altitude = boardsStruct[0].alt;
    float Node1_Temperature11 = boardsStruct[0].temp11;
    float Node1_Humidity11 = boardsStruct[0].hum11;
    float Node1_SoilMoisture = boardsStruct[0].soilmoist;
    float Node1_Rain = boardsStruct[0].rain;
    float Node1_ReadinID = boardsStruct[0].readingId;

    //Node sensor 2//
    float Node2_Temperature = boardsStruct[1].temp;
    float Node2_Humidity = boardsStruct[1].hum;
    float Node2_Pressure = boardsStruct[1].pres;
    float Node2_Altitude = boardsStruct[1].alt;
    float Node2_Temperature11 = boardsStruct[1].temp11;
    float Node2_Humidity11 = boardsStruct[1].hum11;
    float Node2_SoilMoisture = boardsStruct[1].soilmoist;
    float Node2_Rain = boardsStruct[1].rain;
    float Node2_ReadinID = boardsStruct[1].readingId;

    //Node sensor 3//
    float Node3_Temperature = boardsStruct[2].temp;
    float Node3_Humidity = boardsStruct[2].hum;
    float Node3_Pressure = boardsStruct[2].pres;
    float Node3_Altitude = boardsStruct[2].alt;
    float Node3_Temperature11 = boardsStruct[2].temp11;
    float Node3_Humidity11 = boardsStruct[2].hum11;
    float Node3_SoilMoisture = boardsStruct[2].soilmoist;
    float Node3_Rain = boardsStruct[2].rain;
    float Node3_ReadinID = boardsStruct[2].readingId;

    //Node sensor 4//
    float Node4_Temperature = boardsStruct[3].temp;
    float Node4_Humidity = boardsStruct[3].hum;
    float Node4_Pressure = boardsStruct[3].pres;
    float Node4_Altitude = boardsStruct[3].alt;
    float Node4_Temperature11 = boardsStruct[3].temp11;
    float Node4_Humidity11 = boardsStruct[3].hum11;
    float Node4_SoilMoisture = boardsStruct[3].soilmoist;
    float Node4_Rain = boardsStruct[3].rain;
    float Node4_ReadinID = boardsStruct[3].readingId;

    //Node sensor 5//
    float Node5_Temperature = boardsStruct[4].temp;
    float Node5_Humidity = boardsStruct[4].hum;
    float Node5_Pressure = boardsStruct[4].pres;
    float Node5_Altitude = boardsStruct[4].alt;
    float Node5_Temperature11 = boardsStruct[4].temp11;
    float Node5_Humidity11 = boardsStruct[4].hum11;
    float Node5_SoilMoisture = boardsStruct[4].soilmoist;
    float Node5_Rain = boardsStruct[4].rain;
    float Node5_ReadinID = boardsStruct[4].readingId;
    
    //publish Node Sensor 1//
    if(client.publish("Temperature-node1", String(Node1_Temperature).c_str()) ){
      Serial.printf("Temperature-node1 value : %4.2f \n", Node1_Temperature);
      digitalWrite(greenLed, HIGH);
      digitalWrite(redLed, LOW);
    }
    else{
      Serial.print("Temperature-node1 has failed to send");
      digitalWrite(greenLed, LOW);
      digitalWrite(redLed, HIGH);
      Serial.println();
      client.connect("NODE-SERVER", mqtt_user, mqtt_password);
      client.publish("Temperature-node1", String(Node1_Temperature).c_str());
    }

    if(client.publish("Temperature-node1", String(Node1_Temperature11).c_str()) ){
       Serial.printf("Temperature-node1 value : %4.2f \n", Node1_Temperature11);
       digitalWrite(greenLed, HIGH);
       digitalWrite(redLed, LOW);
    }
    else{
      Serial.print("Temperature-node1 has failed to send");
      digitalWrite(greenLed, LOW);
      digitalWrite(redLed, HIGH);
      Serial.println();
      client.connect("NODE-SERVER", mqtt_user, mqtt_password);
      client.publish("Temperature-node1", String(Node1_Temperature11).c_str());
    }

    if(client.publish("Humidity-node1", String(Node1_Humidity).c_str()) ){
      Serial.printf("Humidity-node1 value : %4.2f \n", Node1_Humidity);
      digitalWrite(greenLed, HIGH);
      digitalWrite(redLed, LOW);
    }
    else{
      Serial.print("Humidity-node1 has failed to send");
      digitalWrite(greenLed, LOW);
      digitalWrite(redLed, HIGH);
      Serial.println();
      client.connect("NODE-SERVER", mqtt_user, mqtt_password);
      client.publish("Humidity-node1", String(Node1_Humidity).c_str());
    }

    if(client.publish("Humidity-node1", String(Node1_Humidity11).c_str()) ){
      Serial.printf("Humidity-node1 value : %4.2f \n", Node1_Humidity11);
      digitalWrite(greenLed, HIGH);
      digitalWrite(redLed, LOW);
    }
    else{
      Serial.print("Humidity-node1 has failed to send");
      digitalWrite(greenLed, LOW);
      digitalWrite(redLed, HIGH);
      Serial.println();
      client.connect("NODE-SERVER", mqtt_user, mqtt_password);
      client.publish("Humidity-node1", String(Node1_Humidity11).c_str());
    }

    if(client.publish("Pressure-node1", String(Node1_Pressure).c_str()) ){
      Serial.printf("Pressure-node1 value : %4.2f \n", Node1_Pressure);
      digitalWrite(greenLed, HIGH);
      digitalWrite(redLed, LOW);
    }
    else{
      Serial.print("Pressure-node1 has failed to send");
      digitalWrite(greenLed, LOW);
      digitalWrite(redLed, HIGH);
      Serial.println();
      client.connect("NODE-SERVER", mqtt_user, mqtt_password);
      client.publish("Pressure-node1", String(Node1_Pressure).c_str());
    }

    if(client.publish("Altitude-node1", String(Node1_Altitude).c_str()) ){
      Serial.printf("Altitude-node1 value : %4.2f \n", Node1_Altitude);
      digitalWrite(greenLed, HIGH);
      digitalWrite(redLed, LOW);
    }
    else{
      Serial.print("Altitude-node1 has failed to send");
      digitalWrite(greenLed, LOW);
      digitalWrite(redLed, HIGH);
      Serial.println();
      client.connect("NODE-SERVER", mqtt_user, mqtt_password);
      client.publish("Altitude-node1", String(Node1_Altitude).c_str());
    }

    if(client.publish("Moisture-node1", String(Node1_SoilMoisture).c_str()) ){
      Serial.printf("Moisture-node1 value : %4.2f \n", Node1_SoilMoisture);
      digitalWrite(greenLed, HIGH);
      digitalWrite(redLed, LOW);
    }
    else{
      Serial.print("Moisture-node1 has failed to send");
      digitalWrite(greenLed, LOW);
      digitalWrite(redLed, HIGH);
      Serial.println();
      client.connect("NODE-SERVER", mqtt_user, mqtt_password);
      client.publish("Moisture-node1", String(Node1_SoilMoisture).c_str());
    }

    if(client.publish("Rain-node1", String(Node1_Rain).c_str()) ){
      Serial.printf("Rain-node1 value : %4.2f \n", Node1_Rain);
      digitalWrite(greenLed, HIGH);
      digitalWrite(redLed, LOW);
    }
    else{
      Serial.print("Rain-node1 has failed to send");
      digitalWrite(greenLed, LOW);
      digitalWrite(redLed, HIGH);
      Serial.println();
      client.connect("NODE-SERVER", mqtt_user, mqtt_password);
      client.publish("Rain-node1", String(Node1_Rain).c_str());
    }

    //publish Node Sensor 2//
    if(client.publish("Temperature-node2", String(Node2_Temperature).c_str()) ){
      Serial.printf("Temperature-node2 value : %4.2f \n", Node2_Temperature);
      digitalWrite(greenLed, HIGH);
      digitalWrite(redLed, LOW);
    }
    else{
      Serial.print("Temperature-node2 has failed to send");
      digitalWrite(greenLed, LOW);
      digitalWrite(redLed, HIGH);
      Serial.println();
      client.connect("NODE-SERVER", mqtt_user, mqtt_password);
      client.publish("Temperature-node2", String(Node2_Temperature).c_str());
    }
    
    if(client.publish("Temperature-node2", String(Node2_Temperature11).c_str()) ){
       Serial.printf("Temperature-node2 value : %4.2f \n", Node2_Temperature11);
       digitalWrite(greenLed, HIGH);
       digitalWrite(redLed, LOW);
    }
    else{
      Serial.print("Temperature-node2 has failed to send");
      digitalWrite(greenLed, LOW);
      digitalWrite(redLed, HIGH);
      Serial.println();
      client.connect("NODE-SERVER", mqtt_user, mqtt_password);
      client.publish("Temperature-node2", String(Node2_Temperature11).c_str());
    }

    if(client.publish("Humidity-node2", String(Node2_Humidity).c_str()) ){
      Serial.printf("Humidity-node2 value : %4.2f \n", Node2_Humidity);
      digitalWrite(greenLed, HIGH);
      digitalWrite(redLed, LOW);
    }
    else{
      Serial.print("Humidity-node2 has failed to send");
      digitalWrite(greenLed, LOW);
      digitalWrite(redLed, HIGH);
      Serial.println();
      client.connect("NODE-SERVER", mqtt_user, mqtt_password);
      client.publish("Humidity-node2", String(Node2_Humidity).c_str());
    }

    if(client.publish("Humidity-node2", String(Node2_Humidity11).c_str()) ){
      Serial.printf("Humidity-node2 value : %4.2f \n", Node2_Humidity11);
      digitalWrite(greenLed, HIGH);
      digitalWrite(redLed, LOW);
    }
    else{
      Serial.print("Humidity-node2 has failed to send");
      digitalWrite(greenLed, LOW);
      digitalWrite(redLed, HIGH);
      Serial.println();
      client.connect("NODE-SERVER", mqtt_user, mqtt_password);
      client.publish("Humidity-node2", String(Node2_Humidity11).c_str());
    }

    if(client.publish("Pressure-node2", String(Node2_Pressure).c_str()) ){
      Serial.printf("Pressure-node2 value : %4.2f \n", Node2_Pressure);
      digitalWrite(greenLed, HIGH);
      digitalWrite(redLed, LOW);
    }
    else{
      Serial.print("Pressure-node2 has failed to send");
      digitalWrite(greenLed, LOW);
      digitalWrite(redLed, HIGH);
      Serial.println();
      client.connect("NODE-SERVER", mqtt_user, mqtt_password);
      client.publish("Pressure-node2", String(Node2_Pressure).c_str());
    }

    if(client.publish("Altitude-node2", String(Node2_Altitude).c_str()) ){
      Serial.printf("Altitude-node2 value : %4.2f \n", Node2_Altitude);
      digitalWrite(greenLed, HIGH);
      digitalWrite(redLed, LOW);
    }
    else{
      Serial.print("Altitude-node2 has failed to send");
      digitalWrite(greenLed, LOW);
      digitalWrite(redLed, HIGH);
      Serial.println();
      client.connect("NODE-SERVER", mqtt_user, mqtt_password);
      client.publish("Altitude-node2", String(Node2_Altitude).c_str());
    }

    if(client.publish("Moisture-node2", String(Node2_SoilMoisture).c_str()) ){
      Serial.printf("Moisture-node2 value : %4.2f \n", Node2_SoilMoisture);
      digitalWrite(greenLed, HIGH);
      digitalWrite(redLed, LOW);
    }
    else{
      Serial.print("Moisture-node2 has failed to send");
      digitalWrite(greenLed, LOW);
      digitalWrite(redLed, HIGH);
      Serial.println();
      client.connect("NODE-SERVER", mqtt_user, mqtt_password);
      client.publish("Moisture-node2", String(Node2_SoilMoisture).c_str());
    }

    if(client.publish("Rain-node2", String(Node2_Rain).c_str()) ){
      Serial.printf("Rain-node2 value : %4.2f \n", Node2_Rain);
      digitalWrite(greenLed, HIGH);
      digitalWrite(redLed, LOW);
    }
    else{
      Serial.print("Rain-node2 has failed to send");
      digitalWrite(greenLed, LOW);
      digitalWrite(redLed, HIGH);
      Serial.println();
      client.connect("NODE-SERVER", mqtt_user, mqtt_password);
      client.publish("Rain-node2", String(Node2_Rain).c_str());
    }

    //publish Node Sensor 3//
    if(client.publish("Temperature-node3", String(Node3_Temperature).c_str()) ){
      Serial.printf("Temperature-node3 value : %4.2f \n", Node3_Temperature);
      digitalWrite(greenLed, HIGH);
      digitalWrite(redLed, LOW);
    }
    else{
      Serial.print("Temperature-node3 has failed to send");
      digitalWrite(greenLed, LOW);
      digitalWrite(redLed, HIGH);
      Serial.println();
      client.connect("NODE-SERVER", mqtt_user, mqtt_password);
      client.publish("Temperature-node3", String(Node3_Temperature).c_str());
    }

    if(client.publish("Temperature-node3", String(Node3_Temperature11).c_str()) ){
       Serial.printf("Temperature-node3 value : %4.2f \n", Node3_Temperature11);
       digitalWrite(greenLed, HIGH);
       digitalWrite(redLed, LOW);
    }
    else{
      Serial.print("Temperature-node3 has failed to send");
      digitalWrite(greenLed, LOW);
      digitalWrite(redLed, HIGH);
      Serial.println();
      client.connect("NODE-SERVER", mqtt_user, mqtt_password);
      client.publish("Temperature-node3", String(Node3_Temperature11).c_str());
    }

    if(client.publish("Humidity-node3", String(Node3_Humidity).c_str()) ){
      Serial.printf("Humidity-node3 value : %4.2f \n", Node3_Humidity);
      digitalWrite(greenLed, HIGH);
      digitalWrite(redLed, LOW);
    }
    else{
      Serial.print("Humidity-node3 has failed to send");
      digitalWrite(greenLed, LOW);
      digitalWrite(redLed, HIGH);
      Serial.println();
      client.connect("NODE-SERVER", mqtt_user, mqtt_password);
      client.publish("Humidity-node3", String(Node3_Humidity).c_str());
    }

    if(client.publish("Humidity-node3", String(Node3_Humidity11).c_str()) ){
      Serial.printf("Humidity-node3 value : %4.2f \n", Node3_Humidity11);
      digitalWrite(greenLed, HIGH);
      digitalWrite(redLed, LOW);
    }
    else{
      Serial.print("Humidity-node3 has failed to send");
      digitalWrite(greenLed, LOW);
      digitalWrite(redLed, HIGH);
      Serial.println();
      client.connect("NODE-SERVER", mqtt_user, mqtt_password);
      client.publish("Humidity-node3", String(Node3_Humidity11).c_str());
    }

    if(client.publish("Pressure-node3", String(Node3_Pressure).c_str()) ){
      Serial.printf("Pressure-node3 value : %4.2f \n", Node3_Pressure);
      digitalWrite(greenLed, HIGH);
      digitalWrite(redLed, LOW);
    }
    else{
      Serial.print("Pressure-node3 has failed to send");
      digitalWrite(greenLed, LOW);
      digitalWrite(redLed, HIGH);
      Serial.println();
      client.connect("NODE-SERVER", mqtt_user, mqtt_password);
      client.publish("Pressure-node3", String(Node3_Pressure).c_str());
    }

    if(client.publish("Altitude-node3", String(Node3_Altitude).c_str()) ){
      Serial.printf("Altitude-node3 value : %4.2f \n", Node3_Altitude);
      digitalWrite(greenLed, HIGH);
      digitalWrite(redLed, LOW);
    }
    else{
      Serial.print("Altitude-node3 has failed to send");
      digitalWrite(greenLed, LOW);
      digitalWrite(redLed, HIGH);
      Serial.println();
      client.connect("NODE-SERVER", mqtt_user, mqtt_password);
      client.publish("Altitude-node3", String(Node3_Altitude).c_str());
    }

    if(client.publish("Moisture-node3", String(Node3_SoilMoisture).c_str()) ){
      Serial.printf("Moisture-node3 value : %4.2f \n", Node3_SoilMoisture);
      digitalWrite(greenLed, HIGH);
      digitalWrite(redLed, LOW);
    }
    else{
      Serial.print("Moisture-node3 has failed to send");
      digitalWrite(greenLed, LOW);
      digitalWrite(redLed, HIGH);
      Serial.println();
      client.connect("NODE-SERVER", mqtt_user, mqtt_password);
      client.publish("Moisture-node3", String(Node3_SoilMoisture).c_str());
    }

    if(client.publish("Rain-node3", String(Node3_Rain).c_str()) ){
      Serial.printf("Rain-node3 value : %4.2f \n", Node3_Rain);
      digitalWrite(greenLed, HIGH);
      digitalWrite(redLed, LOW);
    }
    else{
      Serial.print("Rain-node3 has failed to send");
      digitalWrite(greenLed, LOW);
      digitalWrite(redLed, HIGH);
      Serial.println();
      client.connect("NODE-SERVER", mqtt_user, mqtt_password);
      client.publish("Rain-node3", String(Node3_Rain).c_str()); 
    }

    //publish Node Sensor 4//
    if(client.publish("Temperature-node4", String(Node4_Temperature).c_str()) ){
      Serial.printf("Temperature-node4 value : %4.2f \n", Node4_Temperature);
      digitalWrite(greenLed, HIGH);
      digitalWrite(redLed, LOW);
    }
    else{
      Serial.print("Temperature-node4 has failed to send");
      digitalWrite(greenLed, LOW);
      digitalWrite(redLed, HIGH);
      Serial.println();
      client.connect("NODE-SERVER", mqtt_user, mqtt_password);
      client.publish("Temperature-node4", String(Node4_Temperature).c_str());
    }

    if(client.publish("Temperature-node4", String(Node4_Temperature11).c_str()) ){
      Serial.printf("Temperature-node4 value : %4.2f \n", Node4_Temperature11);
      digitalWrite(greenLed, HIGH);
      digitalWrite(redLed, LOW);
    }
    else{
      Serial.print("Temperature-node4 has failed to send");
      digitalWrite(greenLed, LOW);
      digitalWrite(redLed, HIGH);
      Serial.println();
      client.connect("NODE-SERVER", mqtt_user, mqtt_password);
      client.publish("Temperature-node4", String(Node4_Temperature11).c_str());
    }

    if(client.publish("Humidity-node4", String(Node4_Humidity).c_str()) ){
      Serial.printf("Humidity-node4 value : %4.2f \n", Node4_Humidity);
      digitalWrite(greenLed, HIGH);
      digitalWrite(redLed, LOW);
    }
    else{
      Serial.print("Humidity-node4 has failed to send");
      digitalWrite(greenLed, LOW);
      digitalWrite(redLed, HIGH);
      Serial.println();
      client.connect("NODE-SERVER", mqtt_user, mqtt_password);
      client.publish("Humidity-node4", String(Node4_Humidity).c_str());
    }

    if(client.publish("Humidity-node4", String(Node4_Humidity11).c_str()) ){
      Serial.printf("Humidity-node4 value : %4.2f \n", Node4_Humidity11);
      digitalWrite(greenLed, HIGH);
      digitalWrite(redLed, LOW);
    }
    else{
      Serial.print("Humidity-node4 has failed to send");
      digitalWrite(greenLed, LOW);
      digitalWrite(redLed, HIGH);
      Serial.println();
      client.connect("NODE-SERVER", mqtt_user, mqtt_password);
      client.publish("Humidity-node4", String(Node4_Humidity11).c_str());
    }

    if(client.publish("Pressure-node4", String(Node4_Pressure).c_str()) ){
      Serial.printf("Pressure-node4 value : %4.2f \n", Node4_Pressure);
      digitalWrite(greenLed, HIGH);
      digitalWrite(redLed, LOW);
    }
    else{
      Serial.print("Pressure-node4 has failed to send");
      digitalWrite(greenLed, LOW);
      digitalWrite(redLed, HIGH);
      Serial.println();
      client.connect("NODE-SERVER", mqtt_user, mqtt_password);
      client.publish("Pressure-node4", String(Node4_Pressure).c_str());
    }

    if(client.publish("Altitude-node4", String(Node4_Altitude).c_str()) ){
      Serial.printf("Altitude-node4 value : %4.2f \n", Node4_Altitude);
      digitalWrite(greenLed, HIGH);
      digitalWrite(redLed, LOW);
    }
    else{
      Serial.print("Altitude-node4 has failed to send");
      digitalWrite(greenLed, LOW);
      digitalWrite(redLed, HIGH);
      Serial.println();
      client.connect("NODE-SERVER", mqtt_user, mqtt_password);
      client.publish("Altitude-node4", String(Node4_Altitude).c_str());
    }

    if(client.publish("Moisture-node4", String(Node4_SoilMoisture).c_str()) ){
      Serial.printf("Moisture-node4 value : %4.2f \n", Node4_SoilMoisture);
      digitalWrite(greenLed, HIGH);
      digitalWrite(redLed, LOW);
    }
    else{
      Serial.print("Moisture-node4 has failed to send");
      digitalWrite(greenLed, LOW);
      digitalWrite(redLed, HIGH);
      Serial.println();
      client.connect("NODE-SERVER", mqtt_user, mqtt_password);
      client.publish("Moisture-node4", String(Node4_SoilMoisture).c_str());
    }

    if(client.publish("Rain-node4", String(Node4_Rain).c_str()) ){
      Serial.printf("Rain-node4 value : %4.2f \n", Node4_Rain);
      digitalWrite(greenLed, HIGH);
      digitalWrite(redLed, LOW);
    }
    else{
      Serial.print("Rain-node4 has failed to send");
      digitalWrite(greenLed, LOW);
      digitalWrite(redLed, HIGH);
      Serial.println();
      client.connect("NODE-SERVER", mqtt_user, mqtt_password);
      client.publish("Rain-node4", String(Node4_Rain).c_str());
    }

    //publish Node Sensor 5//
    if(client.publish("Temperature-node5", String(Node5_Temperature).c_str()) ){
      Serial.printf("Temperature-node5 value : %4.2f \n", Node5_Temperature);
      digitalWrite(greenLed, HIGH);
      digitalWrite(redLed, LOW);
    }
    else{
      Serial.print("Temperature-node5 has failed to send");
      digitalWrite(greenLed, LOW);
      digitalWrite(redLed, HIGH);
      Serial.println();
      client.connect("NODE-SERVER", mqtt_user, mqtt_password);
      client.publish("Temperature-node5", String(Node5_Temperature).c_str());
    }

    if(client.publish("Temperature-node5", String(Node5_Temperature11).c_str()) ){
      Serial.printf("Temperature-node5 value : %4.2f \n", Node5_Temperature11);
      digitalWrite(greenLed, HIGH);
      digitalWrite(redLed, LOW);
    }
    else{
      Serial.print("Temperature-node5 has failed to send");
      digitalWrite(greenLed, LOW);
      digitalWrite(redLed, HIGH);
      Serial.println();
      client.connect("NODE-SERVER", mqtt_user, mqtt_password);
      client.publish("Temperature-node5", String(Node5_Temperature11).c_str());
    }

    if(client.publish("Humidity-node5", String(Node5_Humidity).c_str()) ){
      Serial.printf("Humidity-node5 value : %4.2f \n", Node5_Humidity);
      digitalWrite(greenLed, HIGH);
      digitalWrite(redLed, LOW);
    }
    else{
      Serial.print("Humidity-node5 has failed to send");
      digitalWrite(greenLed, LOW);
      digitalWrite(redLed, HIGH);
      Serial.println();
      client.connect("NODE-SERVER", mqtt_user, mqtt_password);
      client.publish("Humidity-node5", String(Node5_Humidity).c_str());
    }

    if(client.publish("Humidity-node5", String(Node5_Humidity11).c_str()) ){
      Serial.printf("Humidity-node5 value : %4.2f \n", Node5_Humidity11);
      digitalWrite(greenLed, HIGH);
      digitalWrite(redLed, LOW);
    }
    else{
      Serial.print("Humidity-node5 has failed to send");
      digitalWrite(greenLed, LOW);
      digitalWrite(redLed, HIGH);
      Serial.println();
      client.connect("NODE-SERVER", mqtt_user, mqtt_password);
      client.publish("Humidity-node5", String(Node5_Humidity11).c_str());
    }

    if(client.publish("Pressure-node5", String(Node5_Pressure).c_str()) ){
      Serial.printf("Pressure-node5 value : %4.2f \n", Node5_Pressure);
      digitalWrite(greenLed, HIGH);
      digitalWrite(redLed, LOW);
    }
    else{
      Serial.print("Pressure-node5 has failed to send");
      digitalWrite(greenLed, LOW);
      digitalWrite(redLed, HIGH);
      Serial.println();
      client.connect("NODE-SERVER", mqtt_user, mqtt_password);
      client.publish("Pressure-node5", String(Node5_Pressure).c_str());
    }

    if(client.publish("Altitude-node5", String(Node5_Altitude).c_str()) ){
      Serial.printf("Altitude-node5 value : %4.2f \n", Node5_Altitude);
      digitalWrite(greenLed, HIGH);
      digitalWrite(redLed, LOW);
    }
    else{
      Serial.print("Altitude-node5 has failed to send");
      digitalWrite(greenLed, LOW);
      digitalWrite(redLed, HIGH);
      Serial.println();
      client.connect("NODE-SERVER", mqtt_user, mqtt_password);
      client.publish("Altitude-node5", String(Node5_Altitude).c_str());
    }
    if(client.publish("Moisture-node5", String(Node5_SoilMoisture).c_str()) ){
      Serial.printf("Moisture-node5 value : %4.2f \n", Node5_SoilMoisture);
      digitalWrite(greenLed, HIGH);
      digitalWrite(redLed, LOW);
    }
    else{
      Serial.print("Moisture-node5 has failed to send");
      digitalWrite(greenLed, LOW);
      digitalWrite(redLed, HIGH);
      Serial.println();
      client.connect("NODE-SERVER", mqtt_user, mqtt_password);
      client.publish("Moisture-node5", String(Node5_SoilMoisture).c_str());
    }

    if(client.publish("Rain-node5", String(Node5_Rain).c_str()) ){
      Serial.printf("Rain-node5 value : %4.2f \n", Node5_Rain);
      digitalWrite(greenLed, HIGH);
      digitalWrite(redLed, LOW);
    }
    else{
      Serial.print("Rain-node5 has failed to send");
      digitalWrite(greenLed, LOW);
      digitalWrite(redLed, HIGH);
      Serial.println();
      client.connect("NODE-SERVER", mqtt_user, mqtt_password);
      client.publish("Rain-node5", String(Node5_Rain).c_str());
    }
  
  delay(5000);
  client.loop();
}
//  static unsigned long lastEventTime = millis();
//  static const unsigned long EVENT_INTERVAL_MS = 5000;
//  if ((millis() - lastEventTime) > EVENT_INTERVAL_MS) {
//    events.send("ping",NULL,millis());
//    lastEventTime = millis();
//  }
