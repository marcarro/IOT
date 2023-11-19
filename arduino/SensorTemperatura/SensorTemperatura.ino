#include <DHT.h>
#include <DHT_U.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>

#define DHTPIN    2
#define DHTTYPE   DHT11
DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "Tec-IoT";
const char* password = "spotless.magnetic.bridge";

HTTPClient httpClient;
WiFiClient wClient;

const int FAN_PIN = D2;

String URL1 = "http://10.22.197.26:3100/api/logValue/2/";
String URL2 = "http://10.22.197.26:3100/api/logState/2/";

void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(FAN_PIN, OUTPUT);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  WiFi.begin(ssid, password);
  Serial.print("Conectando a red WiFi \"");
  Serial.print(ssid);
  Serial.print("\"");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("\nConectado! IP: ");
  Serial.println(WiFi.localIP());
  delay(500);
}

void loop() {
  float temperature = dht.readTemperature();

  Serial.print("Temperatura: ");
  Serial.print(temperature);
  Serial.print(" ºC\t");

  // Verifica si la temperatura es mayor a 30 grados
  int fanState;
  if (temperature > 25) {
    digitalWrite(FAN_PIN, HIGH);
    fanState = 2; // Ventilador encendido
    Serial.println("Ventilador encendido");
  } else {
    digitalWrite(FAN_PIN, LOW);
    fanState = 1; // Ventilador apagado
    
    Serial.println("Ventilador apagado");
  }

  logIntento(temperature);
  logEstado(fanState);

  delay(2000);
}

void logIntento(float temperature){
  if(WiFi.status() == WL_CONNECTED){
    String data = URL1;
    data = data + temperature;
    Serial.println(data); 
    
    httpClient.begin(wClient, data.c_str()); 
    httpClient.addHeader("Content-Type", "Content-Type: application/json");
    int httpResponseCode = httpClient.POST(data.c_str());
    Serial.println(httpResponseCode); 
    httpClient.end(); 
  }
  return;
}
void logEstado(int fanState){
  if(WiFi.status() == WL_CONNECTED){
    String data = URL2;
    data = data + fanState;
    Serial.println(fanState); 
    
    httpClient.begin(wClient, data.c_str()); 
    httpClient.addHeader("Content-Type", "Content-Type: application/json");
    int httpResponseCode = httpClient.POST(data.c_str());
    Serial.println(httpResponseCode); 
    httpClient.end(); 
  }
  return;
}


