
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ---------- WiFi DETAILS ----------
const char* WIFI_SSID = "xxxxxxxxxx";
const char* WIFI_PASSWORD = "yyyyyyyy";

// ---------- TELEGRAM DETAILS ----------
#define BOT_TOKEN "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
#define CHAT_ID "yyyyyyyyy"

// ---------- GPIO PINS ----------
#define DHT_PIN 4
#define DHT_TYPE DHT11

#define RELAY_PIN 26
#define IR_PIN 27

#define GREEN_LED 18
#define RED_LED 19

// ---------- SETTINGS ----------
#define TEMP_ON 30.0
#define TEMP_OFF 28.0

// Change to HIGH if your IR module output is HIGH on detection
#define IR_DETECTED LOW

// Low-level-trigger relay
#define RELAY_ON LOW
#define RELAY_OFF HIGH

// ---------- OBJECTS ----------
DHT dht(DHT_PIN, DHT_TYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);

WiFiClientSecure secureClient;
UniversalTelegramBot bot(BOT_TOKEN, secureClient);

// ---------- VARIABLES ----------
float temperature = 0;
float humidity = 0;

bool fanState = false;
bool autoMode = true;

unsigned long lastSensorRead = 0;
unsigned long lastTelegramCheck = 0;
unsigned long lastIRAlert = 0;

const unsigned long SENSOR_INTERVAL = 2000;
const unsigned long TELEGRAM_INTERVAL = 1000;
const unsigned long IR_COOLDOWN = 30000;

// ---------- FAN CONTROL ----------
void setFan(bool state) {
  fanState = state;

  digitalWrite(RELAY_PIN,
               state ? RELAY_ON : RELAY_OFF);

  digitalWrite(GREEN_LED, state ? LOW : HIGH);
  digitalWrite(RED_LED, state ? HIGH : LOW);
}

// ---------- TELEGRAM MESSAGE ----------
void sendAlert(String message) {
  bot.sendMessage(CHAT_ID, message, "");
}

// ---------- LCD DISPLAY ----------
void updateLCD() {
  lcd.setCursor(0, 0);
  lcd.print("Temp:");
  lcd.print(temperature, 1);
  lcd.print((char)223);
  lcd.print("C       ");

  lcd.setCursor(0, 1);
  lcd.print("Hum:");
  lcd.print(humidity, 0);
  lcd.print("% Fan:");
  lcd.print(fanState ? "ON " : "OFF");
}

// ---------- SENSOR READING ----------
void readSensor() {
  float t = dht.readTemperature();
  float h = dht.readHumidity();

  if (isnan(t) || isnan(h)) {
    Serial.println("DHT11 Read Error");
    return;
  }

  temperature = t;
  humidity = h;

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" C  Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  // Automatic cooling with hysteresis
  if (autoMode) {
    if (temperature >= TEMP_ON) {
      setFan(true);
    }
    else if (temperature <= TEMP_OFF) {
      setFan(false);
    }
  }

  updateLCD();
}

// ---------- IR SENSOR ----------
void checkIR() {
  bool detected = (digitalRead(IR_PIN) == IR_DETECTED);

  if (detected &&
      millis() - lastIRAlert >= IR_COOLDOWN) {

    sendAlert("ALERT! Intrusion detected by IR sensor.");

    lastIRAlert = millis();
    Serial.println("IR Detection Alert");
  }
}

// ---------- TELEGRAM COMMANDS ----------
void handleTelegram() {
  int count = bot.getUpdates(bot.last_message_received + 1);

  while (count) {
    for (int i = 0; i < count; i++) {

      String text = bot.messages[i].text;
      String chat_id = bot.messages[i].chat_id;

      // Only accept commands from your own chat
      if (chat_id != CHAT_ID) {
        continue;
      }

      if (text == "/start") {
        bot.sendMessage(
          CHAT_ID,
          "ESP32 Industrial Monitoring System\n"
          "/status - Check status\n"
          "/fan_on - Manual fan ON\n"
          "/fan_off - Manual fan OFF\n"
          "/auto - Automatic mode",
          ""
        );
      }

      else if (text == "/status") {
        String msg = "SYSTEM STATUS\n";
        msg += "Temperature: " + String(temperature, 1) + " C\n";
        msg += "Humidity: " + String(humidity, 0) + " %\n";
        msg += "Fan: " + String(fanState ? "ON" : "OFF");
        msg += "\nMode: ";
        msg += autoMode ? "AUTO" : "MANUAL";

        bot.sendMessage(CHAT_ID, msg, "");
      }

      else if (text == "/fan_on") {
        autoMode = false;
        setFan(true);
        bot.sendMessage(CHAT_ID, "Manual Mode: Fan ON", "");
      }

      else if (text == "/fan_off") {
        autoMode = false;
        setFan(false);
        bot.sendMessage(CHAT_ID, "Manual Mode: Fan OFF", "");
      }

      else if (text == "/auto") {
        autoMode = true;

        if (temperature >= TEMP_ON) {
          setFan(true);
        }
        else if (temperature <= TEMP_OFF) {
          setFan(false);
        }

        bot.sendMessage(CHAT_ID, "Automatic Mode Enabled", "");
      }
    }

    count = bot.getUpdates(bot.last_message_received + 1);
  }
}

// ---------- SETUP ----------
void setup() {
  Serial.begin(115200);

  pinMode(RELAY_PIN, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(IR_PIN, INPUT);

  setFan(false);

  dht.begin();

  Wire.begin(21, 22);
  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Industrial IoT");
  lcd.setCursor(0, 1);
  lcd.print("Starting...");

  // Connect WiFi
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected!");
  Serial.println(WiFi.localIP());

  // Prototype TLS setting
  secureClient.setInsecure();

  lcd.clear();
  lcd.print("WiFi Connected");

  sendAlert("ESP32 Industrial IoT System Started");

  delay(1000);
}

// ---------- MAIN LOOP ----------
void loop() {
  unsigned long now = millis();

  if (now - lastSensorRead >= SENSOR_INTERVAL) {
    lastSensorRead = now;
    readSensor();
  }

  if (now - lastTelegramCheck >= TELEGRAM_INTERVAL) {
    lastTelegramCheck = now;

    if (WiFi.status() == WL_CONNECTED) {
      handleTelegram();
    }
  }

  checkIR();
}