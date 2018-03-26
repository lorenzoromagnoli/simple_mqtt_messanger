#include <SPI.h>
#include <WiFi101.h>
#include <MQTTClient.h>

const char *ssid = "CASALUCE";
const char *pass = "robocats";

WiFiClient net;
MQTTClient client;

unsigned long lastMillis = 0;

#include <Wire.h>
#include <LiquidCrystal_PCF8574.h>

LiquidCrystal_PCF8574 lcd(0x27);  // set the LCD address to 0x27 for a 16 chars and 2 line display


void setup() {
  Serial.begin(9600);

  int error;
  Wire.begin();
  Wire.beginTransmission(0x27);
  error = Wire.endTransmission();
  Serial.print("Error: ");
  Serial.print(error);

  if (error == 0) {
    Serial.println(": LCD found.");

  } else {
    Serial.println(": LCD not found.");
  } // if

  lcd.begin(16, 2); // initialize the lcd
  writeMessageToScreen("...");


  
  
  WiFi.begin(ssid, pass);
  client.begin("broker.shiftr.io", net);
  
  connect();
 


}

void connect() {
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    writeMessageToScreen("Connecting...");
    delay(1000);
  }
  Serial.print("\nconnecting...");
  while (!client.connect("mkr1000", "ebd3575a", "39bccc93d8b275b1")) {
    Serial.print(".");
    delay(1000);
  }
  
  Serial.println("\nconnected!");
  writeMessageToScreen("Connected!");

  client.subscribe("/display");
  // client.unsubscribe("/example");
}

void loop() {
  client.loop();
  delay(10); // <- fixes some issues with WiFi stability

  if (!client.connected()) {
    connect();
  }

  // publish a message roughly every second.
  if (millis() - lastMillis > 1000) {
    lastMillis = millis();
    client.publish("/ciao", "mondo");
  }
}

void messageReceived(String topic, String payload, char * bytes, unsigned int length) {
  Serial.print("incoming: ");
  Serial.print(topic);
  Serial.print(" - ");
  Serial.print(payload);
  Serial.println();
  writeMessageToScreen(payload);
}

void writeMessageToScreen(String msg) {
    lcd.setBacklight(255);
    lcd.home(); lcd.clear();
    lcd.print(msg);
}


