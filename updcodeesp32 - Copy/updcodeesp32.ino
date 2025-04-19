#include <SPI.h>
#include <LoRa.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define ss 5
#define rst 14
#define dio0 2
#define ledLoRa 27  
#define ledAlert 26  

const char* ssid = "SAYANTAN 1117";
const char* password = "12345678";
const char* thingspeakUrl = "http://api.thingspeak.com/update";
const char* apiKey = "ROABR0X5J6IF581U";

float tempLow = 15, tempHigh = 35;
float humLow = 30, humHigh = 80;
float moistLow = 300, moistHigh = 1200;

// Initialize LCD (I2C Address, Columns, Rows)
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
    Serial.begin(115200);
    
    pinMode(ledLoRa, OUTPUT);
    pinMode(ledAlert, OUTPUT);
    digitalWrite(ledLoRa, LOW);
    digitalWrite(ledAlert, LOW);

    lcd.init();  // Initialize LCD
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("WiFi Connecting...");
    
    connectWiFi();

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("LoRa Initializing...");
    initLoRa();
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("System Ready");
    delay(2000);
}

void loop() {
    if (LoRa.parsePacket()) {
        String receivedData = LoRa.readString();
        receivedData.trim();
        Serial.println("Received: " + receivedData);
        
        blinkLED(ledLoRa, 2);

        float temperature, humidity, soilMoisture;
        if (parseSensorData(receivedData, temperature, humidity, soilMoisture)) {
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("T: " + String(temperature) + "C");
            lcd.setCursor(0, 1);
            lcd.print("H: " + String(humidity) + "% M:" + String(soilMoisture));
            
            if (isUnhealthy(temperature, humidity, soilMoisture)) {
                digitalWrite(ledAlert, HIGH);
                lcd.setCursor(10, 1);
                lcd.print("ALERT!");
            } else {
                digitalWrite(ledAlert, LOW);
            }
            sendToThingSpeak(temperature, humidity, soilMoisture);
        }
    }
}

void connectWiFi() {
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        lcd.setCursor(0, 1);
        lcd.print(".");
        delay(1000);
    }
    Serial.println("\nWiFi Connected!");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("WiFi Connected!");
    delay(2000);
}

void initLoRa() {
    LoRa.setPins(ss, rst, dio0);
    if (!LoRa.begin(433E6)) {
        Serial.println("LoRa Initialization Failed!");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("LoRa Failed!");
        while (1);
    }
    Serial.println("LoRa Initialized!");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("LoRa Initialized!");
    delay(2000);
}

bool parseSensorData(String data, float &temperature, float &humidity, float &moisture) {
    int tIndex = data.indexOf("T:");
    int hIndex = data.indexOf(",H:");
    int mIndex = data.indexOf(",M:");

    if (tIndex != -1 && hIndex != -1 && mIndex != -1) {
        temperature = data.substring(tIndex + 2, hIndex).toFloat();
        humidity = data.substring(hIndex + 3, mIndex).toFloat();
        moisture = data.substring(mIndex + 3).toFloat();
        return true;
    }
    return false;
}

bool isUnhealthy(float temp, float hum, float moist) {
    return (temp < tempLow || temp > tempHigh || hum < humLow || hum > humHigh || moist < moistLow || moist > moistHigh);
}

void sendToThingSpeak(float temperature, float humidity, float moisture) {
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        String url = thingspeakUrl;
        url += "?api_key=" + String(apiKey);
        url += "&field1=" + String(temperature);
        url += "&field2=" + String(humidity);
        url += "&field3=" + String(moisture);

        http.begin(url);
        int httpResponseCode = http.GET();
        http.end();
        Serial.println("ThingSpeak Response: " + String(httpResponseCode));

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Data Sent:");
        lcd.setCursor(0, 1);
        lcd.print("Code: " + String(httpResponseCode));
        delay(2000);
    }
}

void blinkLED(int pin, int times) {
    for (int i = 0; i < times; i++) {
        digitalWrite(pin, HIGH);
        delay(200);
        digitalWrite(pin, LOW);
        delay(200);
    }
}
