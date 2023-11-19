  // Incluir librerías del sensor
  #include <Wire.h>
  #include <BH1750.h>
  #include <ESP8266WiFi.h>
  #include <WiFiClient.h>
  #include <ESP8266HTTPClient.h>

  // Incializar el sensor
  BH1750 lightSensor(0x23);
  // Varible para modificar la luminosidad del led
  int brightness = 0;

  // Configuración del internet
  const char* ssid = "IZZI-E7F8";
  // = "Tec-IoT";
  const char* password = "hZ3msHWj";
  // = "spotless.magnetic.bridge"; // Incluir contraseña de la red de internet

  HTTPClient httpClient;
  WiFiClient wClient;

  // MOdificar URL
  String URL1 = "http://192.168.1.13:3100/api/logLux/2/"; //ip:3100
  String URL2 = "http://192.168.1.13:3100/api/logLed/2/"; //ip:3100

  void setup() {
  // Comunicación serial
  Serial.begin(9600);
  Serial.println("***Inicializando conexión a My SQL***");

  // Configuración del sensor
  // Entradas IC2 SDA:pin D2 = pin 4 y SCL : pin D1 = pin 5
  Wire.begin(4,5);
  // Inicializar sensor de luminosidad BH1750
  lightSensor.begin();
  // Configuración de salidas led
  pinMode(D0, OUTPUT);

  // Configuración del wifi
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
  // lightSensor.readLightLevel();
  float lux = lightSensor.readLightLevel();
  // Impresión de valores de luminosidad del sensor
  Serial.print("Luminosidad del sensor: ");
  Serial.print(lux);
  Serial.println(" lx");

  // Ajuste de valores de luminosidad
  // Si lux es menor de 20
  if (lux < 20) {
  // brightness será máxima = 255
    brightness = 255;
  } else if (lux < 400) { // Si lux es menor de 400
  // brightness tendrá una intensidad menor a la máxima
    brightness = 180;
  } else if (lux < 1000) { // Si lux es menor de 1000
  // brightness tendrá una intensidad de 140
    brightness = 140;
  } else if (lux < 5000) { // Si lux es menor de 5000
  // brightness tendrá una intensidad de 90
  brightness = 90;
  } else if (lux < 10000) { // Si lux es menor de 10000
  // brightness tendrá una intensidad de 50
    brightness = 50;
  } else { // Si lux sobrepasa cualquiera de los valores previamente establecidos
  // brightness será cero -> el led será apagado
   brightness = 0;
  }

  // Modificar físicamente la luminosidad del led
  analogWrite(D0, brightness); // Configura la intensidad del LED
  // Impresión de valores de  luminosidad del led
  Serial.print("Luminosidad en el led: ");
  Serial.println(brightness);
  // Postear valor leído por el sensor
  logSensor(lux);
  // Postear valor del led
  logActuador(brightness);

  delay(300000); // Frecuencia de muestreo -> Espera 5 minutos

  }

  void logSensor(float lux){
    if(WiFi.status() == WL_CONNECTED){
     String data = URL1;
      data = data + lux;
      Serial.println(data);
      httpClient.begin(wClient, data.c_str());
      httpClient.addHeader("Content-Type", "Content-Type: application/json");
       int httpResponseCode = httpClient.POST(data.c_str());
       Serial.println(httpResponseCode);
      httpClient.end();
      }
  return;
  }

  void logActuador(int brightness){
    if(WiFi.status() == WL_CONNECTED){
    String data = URL2;
    data = data + brightness;
    Serial.println(data);
    httpClient.begin(wClient, data.c_str());
    httpClient.addHeader("Content-Type", "Content-Type: application/json");
    int httpResponseCode = httpClient.POST(data.c_str());
    Serial.println(httpResponseCode);
    httpClient.end();
    }
  return;
  }
