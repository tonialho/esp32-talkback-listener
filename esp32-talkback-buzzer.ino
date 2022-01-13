/**
 * ESP32 for listening to ThingSpeak TalkBack queue.
 * 
 * Include config.h file with your WLAN credentials and the TalkBack URL
 * 
 * Toni Alho 2022
 */


// Confiq file for keeping WLAN credentials and Talkback URL out from the code and public repositories
#include "config.h"

// Libraries 
#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>

// WLAN login credentials
const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;

// Talkback URL
String talkbackURL = TALKBACK_URL;

String checker;
bool completion = false;
int buzzerPin = 13;

void setup()
{
    // Start serial
    Serial.begin(115200);
    delay(10);

    // Connect to the WLAN
    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    // Set the ESP32 on-board led pin to output mode
    pinMode(2, OUTPUT);

    // Buzzer not working atm
    pinMode(buzzerPin, OUTPUT);
}


void loop() 
{
   Serial.println("Starting loop");
   Serial.println("Checking for previous completion cooldown");

  // Cooldown from previous completion
  // The delay should be set according to the time TalkBack value "true" is reseted back to "false"
  if(completion == true) {
    Serial.println("On cooldown");
    delay (150000);
    completion = false;
  }

  HTTPClient http;
  http.begin(talkbackURL);
      
  // Send HTTP GET request
  int httpResponseCode = http.GET();
      
  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    String payload = http.getString();
    Serial.println(payload);
    Serial.println();

    // JSON tricks
    JSONVar myObject = JSON.parse(payload);
    
    if (JSON.typeof(payload) == "undefined") {
      Serial.println("Parsing input failed!");
      return;
    }
    
    Serial.print("JSON object = ");
    Serial.println(myObject);
    Serial.println();

    // Look for the value of JSON file's second key, which should be "true" or "false"
    JSONVar keys = myObject.keys();
    Serial.print("JSON object at [1] = ");
    Serial.println(myObject[keys[1]]);
    checker = myObject[keys[1]];

    // The magic happens here
    if(checker == "true") {
      Serial.println("CONGRATSZZZZZZZZZ!!!!");

      // For debuggind purposes, just blink the built-in led      
      blink();

      completion = true;
    }

    if(checker == "false") {
      Serial.println("NO REWARD!!!!");
      Serial.println();      
    }
  }
 
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
 
  // End the connection
  http.end();

  // Delay for the loop, affects on how quick the response to the change of Talkback value is
  delay(2000);
}


void blink() {
  digitalWrite(2, HIGH);
  delay(300);
  digitalWrite(2, LOW);    
  delay(300);  
  digitalWrite(2, HIGH);
  delay(300);
  digitalWrite(2, LOW);    
  delay(300);  
  digitalWrite(2, HIGH);
  delay(300);
  digitalWrite(2, LOW);    
  delay(300);    
  digitalWrite(2, HIGH);
  delay(300);
  digitalWrite(2, LOW);    
  delay(300);  
  digitalWrite(2, HIGH);
  delay(300);
  digitalWrite(2, LOW);    
  delay(300);

  // Buzzer not working atm
  digitalWrite(buzzerPin, HIGH);
  delay(300);
  digitalWrite(buzzerPin, LOW);    
  delay(300);
  digitalWrite(buzzerPin, HIGH);
  delay(300);
  digitalWrite(buzzerPin, LOW);    
  delay(300);
}
