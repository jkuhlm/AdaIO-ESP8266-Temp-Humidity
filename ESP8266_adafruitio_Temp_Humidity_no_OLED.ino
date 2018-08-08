// Adafruit IO Temperature & Humidity Example
// Tutorial Link: https://learn.adafruit.com/adafruit-io-basics-temperature-and-humidity
//
// Adafruit invests time and resources providing this open source code.
// Please support Adafruit and open source hardware by purchasing
// products from Adafruit!
//
// Written by Todd Treece for Adafruit Industries
// Copyright (c) 2016-2017 Adafruit Industries
// Licensed under the MIT license.
//
// All text above must be included in any redistribution.

/************************** Configuration ***********************************/

// edit the config.h tab and enter your Adafruit IO credentials
// and any additional configuration needed for WiFi, cellular,
// or ethernet clients.
#include "config.h"

/************************ Example Starts Here *******************************/
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Wire.h>

// pin connected to DH22 data line
#define DATA_PIN D3

// create DHT22 instance
DHT_Unified dht(DATA_PIN, DHT22);

// set up the 'temperature' and 'humidity' feeds
AdafruitIO_Feed *temperatureF = io.feed("OverGarage-temperatureF");
AdafruitIO_Feed *humidity = io.feed("OverGarage-humidity");
float lastSavedTemperature = 0.0;

void setup() {

  // initialize dht22
  dht.begin();
  delay(1000);

  // connect to io.adafruit.com
  io.connect();

  // wait for a connection
  while (io.status() < AIO_CONNECTED) {
    delay(500);
  }

  //  //Set hostname
  //  WiFi.hostname("ESP_FamilyRoom");

}

void loop() {

  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();

  sensors_event_t event;
  dht.temperature().getEvent(&event);

  float celsius = event.temperature;

  dht.humidity().getEvent(&event);
  float relative_humidity = event.relative_humidity;

  // Check if any reads failed and exit early (to try again).
  if (isnan(celsius) || isnan(relative_humidity)) {
    return;
  }

  float fahrenheit = (celsius * 1.8) + 32;

  // save temperatuere Adafruit IO if have gine up or down by 0.25f
  if (fahrenheit > lastSavedTemperature + 0.25 || fahrenheit < lastSavedTemperature - 0.25) {
    temperatureF->save(fahrenheit);
    humidity->save(relative_humidity);
    lastSavedTemperature = fahrenheit;
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(100);                       // wait for a second
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    delay(100);                       // wait for a second
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(100);                       // wait for a second
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    delay(100);
  }

  delay(10000);

}

