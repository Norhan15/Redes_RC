#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "NORM1152";
const char* password = "123456789";

const char* serverName = "http://44.193.125.71:3000/api/redes";

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando a WiFi...");
  }
  Serial.println("Conectado a WiFi");

  scanAndSendNetworks();
}

void loop() {
  delay(60000);
  scanAndSendNetworks();
}

void scanAndSendNetworks() {
  Serial.println("Escaneando redes WiFi...");

  int numNetworks = WiFi.scanNetworks();
  for (int i = 0; i < numNetworks; i++) {
    String ssid = WiFi.SSID(i);
    String mac = WiFi.BSSIDstr(i);
    int rssi = WiFi.RSSI(i);
    int encType = WiFi.encryptionType(i);

    String postData = "{\"ssid\":\"" + ssid + "\",\"mac_address\":\"" + mac + "\",\"rssi\":" + rssi + ",\"encryption_type\":" + encType + "}";

    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      http.begin(serverName);
      http.addHeader("Content-Type", "application/json");
      int httpResponseCode = http.POST(postData);

      if (httpResponseCode > 0) {
        String response = http.getString();
        Serial.println("Respuesta del servidor: " + response);
      } else {
        Serial.println("Error en la solicitud POST");
      }

      http.end();
    } else {
      Serial.println("Desconectado de WiFi");
    }
  }
}