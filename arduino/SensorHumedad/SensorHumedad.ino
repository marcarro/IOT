#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>

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
'5','Extractor' 

En actuador, se guarda el estado del actuador (string), y la fecha en la que se guardo.
*/

// CONFIGURACIÓN CASA
//const char* ssid = "IZZI-2F56";
//const char* password = "RtemPaFA";

// CONFIGURACIÓN Tec-IoT
const char* ssid = "Tec-IoT";
const char* password = "spotless.magnetic.bridge";

HTTPClient httpClient;
WiFiClient wClient;

// CONFIGURACIÓN CASA
//String API_URL_SENSOR = "http://192.168.1.19:3100/api/logValue/1/"; // URL de la API
//String API_URL_ACTUADOR = "http://192.168.1.19:3100/api/logState/3/";

// CONFIGURACIÓN Tec-IoT
String API_URL_SENSOR = "http://10.22.169.47:3100/api/logValue/1/"; // URL de la API
String API_URL_ACTUADOR = "http://10.22.169.47:3100/api/logState/3/";

// Configuración de pines y variables
int sensorHumedad = A0;
int humedad = 0;
int pinBomba = D2;


void setup() {
  // Configuración inicial: se inicia la comunicación serial
  Serial.begin(9600);
  // Configuración de los pines
  pinMode(pinBomba, OUTPUT);
  // Configuración de la conexión WiFi
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  WiFi.begin(ssid, password);
  Serial.print("Conectando a red WiFi \"");
  Serial.print(ssid);
  Serial.print("\"");
  // Bucle mientras no haya conexión a internet
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.print("\nConectado! IP: ");
  Serial.println(WiFi.localIP());
  delay(500);
}

void loop() {
  // Leer el valor de humedad
  int lectura = analogRead(sensorHumedad);
  humedad = map(lectura, 1024, 0, 0, 100);
  int activated = 0;

  // Enviar datos a la API
  sendHumedadToAPI(humedad);

  // Realizar el control de la bomba de agua con parámetros de acuerdo con crisantemos
  if (humedad >= 50 && humedad <= 70) {
    digitalWrite(pinBomba, HIGH); 
    activated = 1;
    logActuador(activated);
  } else if (humedad > 60){
    digitalWrite(pinBomba, HIGH);
    activated = 1;
    logActuador(activated);
  } else {
    digitalWrite(pinBomba, LOW);
    activated = 2;
    logActuador(activated);
  }
  // Esperar 5 segundos antes de volver a realizar el loop
  delay(5000);
}

// Función para enviar datos de humedad a la API
void sendHumedadToAPI(int value) {
  // Si se encuentra una conexión a WiFi se hace lo siguiente
  if (WiFi.status() == WL_CONNECTED) {
    //Creación de la ruta de POST con el valor de humedad
    String data = API_URL_SENSOR + String(value); 
    Serial.println("Enviando datos a la API: " + data);
    // Comunicación con el servidor iniciada
    httpClient.begin(wClient, data.c_str());
    // Se establece el encabezado de la solicitud HTTP y se indica el tipo de contenido
    httpClient.addHeader("Content-Type", "Content-Type: application/json");
    // Se guarda la respuesta del servidor
    // 200 -> conexión exitosa
    // 500 -> error
    // -1 -> no hay conexión
    // 400 -> no existe conexión estable
    int httpResponseCode = httpClient.POST(data.c_str());
    Serial.print("Respuesta de la API: ");
    Serial.println(httpResponseCode);
    httpClient.end();
  }
}

// Función para registrar el estado del actuador en la API
void logActuador(int act){
  // Si existe conexión con internet se hace lo siguiente
  if (WiFi.status() == WL_CONNECTED) {
    //Creación de la ruta de POST con el estado del actuador
    String data = API_URL_ACTUADOR + String(act); 
    Serial.println("Enviando datos a la API: " + data);

    httpClient.begin(wClient, data.c_str());
    httpClient.addHeader("Content-Type", "Content-Type: application/json");
    int httpResponseCode = httpClient.POST(data.c_str());
    Serial.print("Respuesta de la API: ");
    Serial.println(httpResponseCode);
    httpClient.end();
  }
}