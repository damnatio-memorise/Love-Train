#include <TM1637.h>

// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain

// REQUIRES the following Arduino libraries:
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
// - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor

#include "DHT.h"
#include "TM1637.h"

#define DHTPIN 2     // Digital pin connected to the DHT sensor
// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.

// Uncomment whatever type you're using!
  #define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

#define CLK 10
#define DIO 9
TM1637 tm1637(CLK,DIO);
// Holds current clock time
int8_t TimeDisp[] = {0xff, 0xff, 0xff, 0xff};


// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  Serial.println(F("DHTxx test!"));

   
  tm1637.init();
  tm1637.set(2);         // TYPICAL = 2, DARKEST = 0, BRIGHTEST = 7;
  tm1637.display(0,10);  // show A B C D for testing
  tm1637.display(1,11);   
  tm1637.display(2,12); 
  tm1637.display(3,13);

  dht.begin();

  pinMode(13, OUTPUT);
}

int hour, minute, second;
unsigned char ClockPoint = 1;

void loop() {
  // Wait a few seconds between measurements.
  delay(2000);
  
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  
  second = t;  // get seconds
 
 // TimeDisp[0] = minute / 10;
  TimeDisp[0] = 1;
 // TimeDisp[1] = minute % 10;
  TimeDisp[1] = 4;
//  TimeDisp[2] = second / 10;
  TimeDisp[2] = second / 10;
//  TimeDisp[3] = second % 10;
  TimeDisp[3] = second % 10;
  tm1637.display(0,TimeDisp[0]);
  tm1637.display(1,TimeDisp[1]); 
  tm1637.display(2,TimeDisp[2]);
  tm1637.display(3,TimeDisp[3]);

  Serial.print(F("14组 Humidity: "));
  Serial.print(h);
  Serial.print(F("%  14组 Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.print(F("°F  14组 Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(hif);
  Serial.println(F("°F"));

  int T=t;
  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(100000/T);                       // wait for a second
  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
  delay(100000/T); 
}
