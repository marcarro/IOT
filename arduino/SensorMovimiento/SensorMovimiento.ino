#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <LiquidCrystal_I2C.h>
#include <string>

/*
SENSORES, ID y NOMBRE
'1','Humedad'
'2','Temperatura'
'3','Movimiento'
'4','Polvo'
'5','Luz'

En sensor, se guarda el valor del sensor (int), y la fecha en la que se guardo.
*/

/* 
ACTUADORES, ID y NOMBRE
'1','Pantalla LCD'
'2','Extractor de aire'
'3','Bomba de agua'
'4','Servomotor'
'5','LED' 

En actuador, se guarda el estado del actuador (string), y la fecha en la que se guardo.
*/

const int pirPin = D5;  // PIR sensor connected to digital pin D5
bool motionDetected = false;
LiquidCrystal_I2C lcd(0x27, 16, 2);

// const char* ssid = "INFINITUM88A2_2.4";
// const char* password = "QqZzgTE6uj";

const char* ssid = "Tec-IoT";
const char* password = "spotless.magnetic.bridge";

HTTPClient httpClient;
WiFiClient wClient;

String SensorURL = "http://10.22.235.51:3100/api/logValue/3/";
String ActURL = "http://10.22.235.51:3100/api/logState/1/1";

void setup() {
  lcd.begin(16, 2);
  lcd.init(); 
  lcd.backlight();
  Serial.begin(9600); 
  Serial.println("***Inicializando conexión a My SQL***");

  pinMode(pirPin, INPUT);

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
  int motionState = 0;
  motionDetected = digitalRead(pirPin);

  if (motionDetected) {
    motionState = 1;
    logIntento(motionState);
    logIntentoAct();
    displayMovement();
    lcd.clear();
  }
      
  delay(1000);
}

void displayMovement() {
  lcd.setCursor(0, 0);
  std::string mensaje;
  mensaje = "MOVIMIENTO DETECTADO";
  lcd.print("MOVIMIENTO DETECTADO");
  scrollMessage(mensaje.size());
}

void scrollMessage(int msgSz) {
  // scroll 13 positions (string length) to the left
  // to move it offscreen left:
  for (int positionCounter = 0; positionCounter < msgSz; positionCounter++) {
    // scroll one position left:
    lcd.scrollDisplayLeft();
    // wait a bit:
    delay(150);
  }

  // scroll 29 positions (string length + display length) to the right
  // to move it offscreen right:
  for (int positionCounter = 0; positionCounter < 16 + msgSz; positionCounter++) {
    // scroll one position right:
    lcd.scrollDisplayRight();
    // wait a bit:
    delay(150);
  }

  // scroll 16 positions (display length + string length) to the left
  // to move it back to center:
  for (int positionCounter = 0; positionCounter < 16; positionCounter++) {
    // scroll one position left:
    lcd.scrollDisplayLeft();
    // wait a bit:
    delay(150);
  }

  // delay at the end of the full loop:
  delay(1000);
}

void logIntento(float t){
  if(WiFi.status() == WL_CONNECTED){
    String data = SensorURL;
    data = data + t;
    Serial.println(data);
    
    httpClient.begin(wClient, data.c_str()); 
    httpClient.addHeader("Content-Type", "Content-Type: application/json");
    int httpResponseCode = httpClient.POST(data.c_str());
    Serial.println(httpResponseCode); 
    httpClient.end(); 
  }
  return;
}

void logIntentoAct(){
  if(WiFi.status() == WL_CONNECTED){
    String data = ActURL;
    
    httpClient.begin(wClient, data.c_str()); 
    httpClient.addHeader("Content-Type", "Content-Type: application/json");
    int httpResponseCode = httpClient.POST(data.c_str());
    httpClient.end(); 
  }
  return;
}
