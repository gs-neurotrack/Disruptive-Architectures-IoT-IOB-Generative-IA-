#include <WiFi.h>
#include <PubSubClient.h>

// ==============================
// CONFIGURAÇÕES DE REDE / MQTT
// ==============================

// ⚠️ No Wokwi use sempre essa rede
const char* WIFI_SSID     = "Wokwi-GUEST";
const char* WIFI_PASSWORD = "";

// Broker público
const char* MQTT_BROKER   = "test.mosquitto.org";
const int   MQTT_PORT     = 1883;

// Tópico para envio dos dados
const char* MQTT_TOPIC    = "workSense/data";

// ==============================
// PINOS DOS SENSORES
// ==============================
//
// NTC  -> GPIO 35 (entrada analógica)
// PIR  -> GPIO 5  (digital)
// LED  -> GPIO 2  (saída para indicar presença)

const int TEMP_PIN      = 35;    // Sensor de temperatura NTC
const int PIR_PIN       = 5;     // Sensor de presença
const int LED_PRESENCE  = 2;     // LED indicador

WiFiClient espClient;
PubSubClient mqttClient(espClient);

// ==============================
// FUNÇÕES DE REDE / MQTT
// ==============================

void connectWiFi() {
  Serial.println("Conectando ao WiFi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi conectado!");
}

void connectMQTT() {
  while (!mqttClient.connected()) {
    Serial.print("Conectando ao broker MQTT... ");
    String clientId = "WorkSense-ESP32-";
    clientId += String(random(0xffff), HEX);

    if (mqttClient.connect(clientId.c_str())) {
      Serial.println("Conectado!");
    } else {
      Serial.print("Falhou. RC=");
      Serial.println(mqttClient.state());
      delay(2000);
    }
  }
}

// ==============================
// LEITURA DE SENSORES
// ==============================

// Aproxima leitura analógica (0–4095) para faixa 0–50 ºC, com sentido correto
float readTemperatureC() {
  int raw = analogRead(TEMP_PIN);

  // Inverte: raw alto = temp baixa, raw baixo = temp alta
  float tempC = 50.0 - (raw * (50.0 / 4095.0));

  return tempC;
}


// Lê presença do PIR (0 ou 1)
int readPresence() {
  return digitalRead(PIR_PIN);
}

// ==============================
// SETUP
// ==============================

void setup() {
  Serial.begin(115200);

  pinMode(TEMP_PIN, INPUT);
  pinMode(PIR_PIN, INPUT);
  pinMode(LED_PRESENCE, OUTPUT);

  connectWiFi();
  mqttClient.setServer(MQTT_BROKER, MQTT_PORT);

  delay(200);
}

// ==============================
// LOOP PRINCIPAL
// ==============================

void loop() {

  if (!mqttClient.connected()) {
    connectMQTT();
  }

  mqttClient.loop();

  float temperature = readTemperatureC();
  int presence = readPresence();

  // LED acende se houver movimento
  digitalWrite(LED_PRESENCE, presence ? HIGH : LOW);

  // Monta JSON
  String payload = "{";
  payload += "\"temperature\":";
  payload += String(temperature, 1);
  payload += ",\"presence\":";
  payload += String(presence);
  payload += "}";

  Serial.print("Publicando: ");
  Serial.println(payload);

  mqttClient.publish(MQTT_TOPIC, payload.c_str());

  delay(3000);
}
