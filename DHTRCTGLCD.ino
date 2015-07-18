#include <Wire.h>
#include "RTClib.h"
#include <Wire.h>
#include "RTClib.h"
#include "DHT.h"

#define DHTPIN 8     // what pin we're connected to
#define DHTTYPE DHT11   // DHT 11 
#include <PCD8544.h>

static PCD8544 lcd;
DHT dht(DHTPIN, DHTTYPE);
RTC_DS1307 rtc;
int LED = 7;

// A custom "degrees" symbol...
static const byte DEGREES_CHAR = 1;
static const byte degrees_glyph[] = { 0x00, 0x07, 0x05, 0x07, 0x00 };



void setup () {
  Serial.begin(9600);
Serial.println("DHTxx test!");
 
  dht.begin();

   pinMode(LED, OUTPUT);
#ifdef AVR
  Wire.begin();
#else
  Wire1.begin(); 
#endif
  rtc.begin();
 lcd.begin(84, 48);
  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
lcd.createChar(DEGREES_CHAR, degrees_glyph);

}


void loop () {
float h = dht.readHumidity();
 float t = dht.readTemperature();
 float f = dht.readTemperature(true);
 
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

 float hi = dht.computeHeatIndex(f, h);
 
    DateTime now = rtc.now();
    digitalWrite(LED, HIGH);
    lcd.setCursor(0, 0); 
    lcd.print(now.year(), DEC);
    lcd.print('/');
    lcd.print(now.month(), DEC);
    lcd.print('/');
    lcd.print(now.day(), DEC);
    lcd.print(' ');
    lcd.setCursor(0, 1);
    lcd.print(now.hour(), DEC);
    lcd.print(':');
    lcd.print(now.minute(), DEC);
    lcd.print(':');
    lcd.println(now.second(), DEC);
    lcd.setCursor(0, 2);
    lcd.print("H: "); 
    lcd.print(h);
    lcd.println(" %");
    lcd.setCursor(0, 3);
    lcd.print("T: "); 
    lcd.print(t,1);
    lcd.print("\001C ");

        
    delay(100);
    digitalWrite(LED, LOW);
}
