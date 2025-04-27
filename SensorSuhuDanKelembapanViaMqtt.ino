#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

const char* ssid = "Akbar";
const char* pass = "akbar780";

const char* broker = "broker.emqx.io";
const int port = 1883;
const char* topic = "/esp32/dht22";

#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Menghubungkan Wifi ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WIfi terhubung");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Konek ke MQTT");
    if (client.connect("ESP32Client123456", "" , "")) {
     Serial.println("Terhubung");
    } else {
      Serial.print("gagal, rc=");
      Serial.print(client.state());
      Serial.println("mencobal dalam 5 detik");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  dht.begin();
  setup_wifi();
  client.setServer(broker, port);

}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature)){
    Serial.println("Gagal baca sensor");
    return;
  }

  Serial.print("Suhu: ");
  Serial.print(temperature);
  Serial.print(" °C ");
  Serial.print("Kelembapan: ");
  Serial.print(humidity);
  Serial.println(" %");

  String payload = "Suhu: " + String(temperature) + " *C, " + "Kelembapan: " + String(humidity) + " %";
  client.publish(topic, (char*) payload.c_str());

  delay(5000);




}
