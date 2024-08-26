#include <WiFi.h>
#include <MQTT.h>

#define dapur 13
#define kamarMandi 14
#define kamarTidur 26
#define red 18
#define green 19
#define blue 21

const char ssid[] = "XO";
const char pass[] = "kissmedontsayno";

WiFiClient net;
MQTTClient client;

void rgb(bool merah, bool hijau, bool biru){
  digitalWrite(red, merah);
  digitalWrite(green, hijau);
  digitalWrite(blue, biru);
}

void connect() {
  pinMode(dapur, OUTPUT);
  pinMode(kamarMandi, OUTPUT);
  pinMode(kamarTidur, OUTPUT);
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);

  Serial.print("checking wifi...");
  rgb(1,0,0);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  rgb(0,0,1);

  Serial.print("\nconnecting...");
  while (!client.connect("Pejuang Sertip")) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nconnected!");
  rgb(0,1,0);

  client.subscribe("rumasama/#");
  // client.unsubscribe("/hello");
}

void messageReceived(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);
  if (topic == "rumasama/dapur") {
    if (payload == "on") {
      digitalWrite(dapur, 1);
    } else {
      digitalWrite(dapur, 0);
    }
  }

  if (topic == "rumasama/mandi") {
    if (payload == "on") {
      digitalWrite(kamarMandi, 1);
    } else {
      digitalWrite(kamarMandi, 0);
    }
  }

  if (topic == "rumasama/tidur") {
    if (payload == "on") {
      digitalWrite(kamarTidur, 1);
    } else {
      digitalWrite(kamarTidur, 0);
    }
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, pass);

  client.begin("broker.emqx.io", net);
  client.onMessage(messageReceived);

  connect();
}

void loop() {
  client.loop();
  delay(10);  // <- fixes some issues with WiFi stability

  if (!client.connected()) {
    connect();
  }
}