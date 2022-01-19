#include <stdio.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include <Servo.h>
#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>

#define HTTP_REST_PORT 80
#define WIFI_RETRY_DELAY 500
#define MAX_WIFI_INIT_RETRY 50
#define IR_PIN 4

const char* wifi_ssid = "YOUR SSID";
const char* wifi_passwd = "SECRET";


IRsend irsend(IR_PIN);  // Set the GPIO to be used to sending the message.

ESP8266WebServer http_rest_server(HTTP_REST_PORT);

int init_wifi() {
  int retries = 0;
  Serial.println("Connecting to WiFi AP..........");
  WiFi.mode(WIFI_STA);
  WiFi.begin(wifi_ssid, wifi_passwd);
  // check the status of WiFi connection to be WL_CONNECTED
  while ((WiFi.status() != WL_CONNECTED) && (retries < MAX_WIFI_INIT_RETRY)) {
    retries++;
    delay(WIFI_RETRY_DELAY);
    Serial.print("#");
  }
  return WiFi.status(); // return the WiFi connection status
}

void post_ir_code() {
  StaticJsonBuffer<500> jsonBuffer;
  String post_body = http_rest_server.arg("plain");
  Serial.println(post_body);
  JsonObject& jsonBody = jsonBuffer.parseObject(http_rest_server.arg("plain"));
  Serial.print("HTTP Method: ");
  Serial.println(http_rest_server.method());
  if (!jsonBody.success()) {
    Serial.println("error on parsing json body");
    http_rest_server.send(400);
  }
  else {
      http_rest_server.sendHeader("Location", "/ir/" + String(1));
      http_rest_server.send(201);
      irsend.sendNEC(strtoul(jsonBody["code"], NULL, 0));
 }
}

void config_rest_server_routing() {
  http_rest_server.on("/", HTTP_GET, []() {
    http_rest_server.send(200, "text/html", "Welcome to the ESP8266 REST Web Server");
  });
  http_rest_server.on("/ir", HTTP_POST, post_ir_code);
}

void setup(void) {
  Serial.begin(115200);
  irsend.begin();
  if (init_wifi() == WL_CONNECTED) {
    Serial.print("Connected to ");
    Serial.print(wifi_ssid);
    Serial.print("--- IP: ");
    Serial.println(WiFi.localIP());
  }
  else {
    Serial.print("Error connecting to: ");
    Serial.println(wifi_ssid);
  }

  config_rest_server_routing();
  http_rest_server.begin();
  Serial.println("HTTP REST Server Started");
}

void loop(void) {
  http_rest_server.handleClient();
}
