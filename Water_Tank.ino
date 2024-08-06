#include <Ultrasonic.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>


#define TRIGGER_PIN   27
#define ECHO_PIN      26
#define TANK_HEIGHT   100     // Height of the tank in cm
#define LCD_ADDR      0x27    // I2C address of LCD

char auth[] = "5dtbvt_Q4LiG-giBxxC6uBMC7yl0GJx8";
char ssid[] = "WE_B8F0A4";
char pass[] = "m3g06682";

Ultrasonic ultrasonic(TRIGGER_PIN, ECHO_PIN);
LiquidCrystal_I2C lcd(LCD_ADDR, 16, 2);


void setup() {
  Serial.begin(115200);
  lcd.begin();
  lcd.backlight();

  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }
  Blynk.begin(auth, ssid, pass);
}

void loop() {
  float distance = ultrasonic.read();

  // Calculate the water level percentage based on the tank's dimensions
  float tank_volume = 50 * 50 * TANK_HEIGHT; // Tank volume in cm^3
  float water_volume = tank_volume - (50 * 50 * distance); // Water volume in cm^3
  float water_level = water_volume / tank_volume * 100; // Water level in percentage

  // Print the water level on the LCD and serial monitor
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Water level: ");
  lcd.print(water_level, 1);
  lcd.print("%");

  Serial.println("Water level: ");
  Serial.println(water_level, 1);
  Serial.println("%");

  // Send the water level data to Blynk app
  Blynk.virtualWrite(V1, water_level);

  delay(1000);
}
