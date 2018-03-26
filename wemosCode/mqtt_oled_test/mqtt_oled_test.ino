// This example uses an Adafruit Huzzah ESP8266
// to connect to shiftr.io.
//
// You can check on your device after a successful
// connection here: https://shiftr.io/try.
//
// by Joël Gähwiler
// https://github.com/256dpi/arduino-mqtt

#include <ESP8266WiFi.h>
#include <MQTTClient.h>

const char *ssid = "CASALUCE";
const char *pass = "robocats";

WiFiClient net;
MQTTClient client;

unsigned long lastMillis = 0;




#include <Wire.h>  // Include Wire if you're using I2C
#include <SFE_MicroOLED.h>  // Include the SFE_MicroOLED library

#define PIN_RESET 255  //
#define DC_JUMPER 0  // I2C Addres: 0 - 0x3C, 1 - 0x3D


MicroOLED oled(PIN_RESET, DC_JUMPER); // Example I2C declaration



void connect(); // <- predefine connect() for setup()

void setup() {

  oled.begin();
  oled.clear(PAGE);
  oled.setFontType(0);
  oled.println("Hi there!"); // Print a const string
  oled.display(); // Draw to the screen
  oled.display();

  
  Serial.begin(9600);
  WiFi.begin(ssid, pass);
  client.begin("broker.shiftr.io", net);
  connect();


  
}

void connect() {
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.print("\nconnecting...");
  while (!client.connect("d1", "ebd3575a", "39bccc93d8b275b1")) {
    Serial.print(".");
    writeMessageToScreen("...");
    delay(1000);
  }
  writeMessageToScreen("Connected!");
  
  Serial.println("\nconnected!");

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

void writeMessageToScreen(String msg){
  oled.clear(PAGE);
  oled.setFontType(0);
  oled.setCursor(0,0);
  oled.println(msg); // Print a const string
  oled.display(); // Draw to the screen
}


