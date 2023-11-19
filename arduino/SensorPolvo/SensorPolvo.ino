#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <Servo.h>

//int servo_pin = 21; // Pin D7 para el actuador
int servo_pin = 13;
//Servo myservo;
Servo motor;

int angle = 0; // Ángulo inicial del servomotor

const char* ssid = "Tec-IoT";
const char* password = "spotless.magnetic.bridge";
HTTPClient httpClient;
WiFiClient wClient;

String URL1 = "http://10.22.200.34:3100/api/logValue/4/"; // Post de valor de sensor
String URL2 = "http://10.22.200.34:3100/api/logState/4/"; // Post de valor de estado de actuador

int measurePin = A0; // Pin del sensor
int ledPower = 2;
int samplingTime = 280;
int deltaTime = 40;
int sleepTime = 9680;
float voMeasured = 0;
float calcVoltage = 0;
float dddensity = 0;
float density = 0;
int state = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("*Inicializando conexión a My SQL*");

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

  // Configuración del pin del actuador
  motor.attach(servo_pin);
  pinMode(servo_pin, OUTPUT);
}

void loop() {
  digitalWrite(ledPower, LOW);
  delayMicroseconds(samplingTime);
  voMeasured = analogRead(measurePin);
  delayMicroseconds(deltaTime);
  digitalWrite(ledPower, HIGH);
  delayMicroseconds(sleepTime);

  calcVoltage = 5 * voMeasured / 1024;
  dddensity = 0.17 * calcVoltage - 0.1;
  density = (calcVoltage - 0.0356) * 120000;

  if (isnan(density)) {
    Serial.println("Error obteniendo los datos del sensor de polvo");
    return;
  }

  Serial.print(" - Dust Density(mg/m3): ");
  Serial.println(density);

  logValue(density);

  // Control del actuador (puerta)
  if (density > 2000) {
    state = 2;
    Serial.println("Actuador encendido");
    abrirPuerta();
  } else {
    state = 1;
  }
  logState(state);

  delay(10000);
}

void logValue(float density) {
  if (WiFi.status() == WL_CONNECTED) {
    String data = URL1;
    data = data + density;
    Serial.println(data);

    httpClient.begin(wClient, data.c_str());
    httpClient.addHeader("Content-Type", "Content-Type: application/json");
    int httpResponseCode = httpClient.POST(data.c_str());
    Serial.println(httpResponseCode);
    httpClient.end();
  }
  return;
}

void logState(int state) {
  if (WiFi.status() == WL_CONNECTED) {
    String data = URL2;
    data = data + state;
    Serial.println(data);

    httpClient.begin(wClient, data.c_str());
    httpClient.addHeader("Content-Type", "Content-Type: application/json");
    int httpResponseCode = httpClient.POST(data.c_str());
    Serial.println(httpResponseCode);
    httpClient.end();
  }
  return;
}

// Función para abrir la puerta
/*void abrirPuerta() {
  for (angle = 0; angle < 180; angle += 1) {
    myservo.write(angle);
    delay(15);
  }
}*/

void abrirPuerta(){
  motor.write(9);
  delay(500);
  motor.write(20);
  delay(500);
  motor.write(30);
  delay(500);
   motor.write(40);
  delay(500);
    motor.write(50);
  delay(500);
  motor.write(60);
  delay(500);
  motor.write(70);
  delay(500);
  motor.write(80);
  delay(500);
  motor.write(90);
  delay(500);
  motor.write(100);
  delay(500);
    motor.write(110);
  delay(500);
    motor.write(120);
  delay(500);
  motor.write(130);
  delay(500);

  digitalWrite(4,HIGH);
  digitalWrite(5,LOW);
  delay(5000);
}


// Puedes agregar más funciones para realizar acciones específicas del actuador según tus necesidades.

