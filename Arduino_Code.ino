//past your Blynk IOT template_ID, TEMPLATE_NAME and AUTH_TOKEN here
#define BLYNK_TEMPLATE_ID "TMPL6w81mPBp8"
#define BLYNK_TEMPLATE_NAME "DHT22"
#define BLYNK_AUTH_TOKEN "Bf7Ds8CXCREH-nn_NxV0apGAEElso7mu"

#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <PID_v1.h>
#include <HTTPClient.h>

// WiFi credentials
char ssid[] = "V2027";
char pass[] = "fahim231";

// DHT sensor pin
#define DHTPIN 4
#define Buzzer 16
#define Fan 18
#define Humidifier 17
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
DHT dht(DHTPIN, DHTTYPE);

// Threshold temperature for notification
#define NOTIFICATION_THRESHOLD 38.0

// PID settings
double Setpoint = 37.5, Input, Output;
double Kp = 15, Ki = 5, Kd = 7; // Initial PID parameters
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

// OLED display settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET     -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Timer variables
unsigned long startTime;
unsigned long elapsedTime;
unsigned long days;
unsigned long hours;
unsigned long minutes;
unsigned long seconds;

// Your Google Script Web App URL
const char* googleScriptURL = "https://script.google.com/macros/s/AKfycbwHriSS3SVPDDItg7uV6vYMiXFMxwGG4QKXFv6q7U2MhGf93gO0k9aJ9k8R5T4KZtig/exec";

void setup() {
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  dht.begin();
  
  // Initialize pins for motor control
  pinMode(32, OUTPUT);
  pinMode(25, OUTPUT);
  pinMode(26, OUTPUT);
  pinMode(23, OUTPUT);
  pinMode(Buzzer, OUTPUT);
  pinMode(Fan, OUTPUT);
  pinMode(Humidifier, OUTPUT);
  pinMode(DHTPIN, INPUT); 

  digitalWrite(25, HIGH);  // Motor direction
  digitalWrite(26, LOW);   // Motor direction

  // Initialize the PID
  myPID.SetMode(AUTOMATIC);
  myPID.SetOutputLimits(0, 255);  // PWM range

  // Initialize the OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.display();
  delay(2000); // Pause for 2 seconds
  display.clearDisplay();

  // Initialize the timer
  startTime = millis();
}

void loop() {
  // Read temperature and humidity
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Print temperature to Serial Monitor
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" *C");

  // Check if the temperature exceeds the threshold
  if (temperature > NOTIFICATION_THRESHOLD) {
    Blynk.logEvent("temperature_alert", "Temperature exceeded 38 degrees Celsius!");
    digitalWrite(Buzzer, HIGH);
    digitalWrite(Fan, LOW);
  } 
  else 
  {
    digitalWrite(Buzzer, LOW);
    digitalWrite(Fan, HIGH);
  }

  // Control the humidifier based on humidity readings
  if (humidity < 55) {
    digitalWrite(Humidifier, LOW);
  } else {
    digitalWrite(Humidifier, HIGH);
  }

  Blynk.virtualWrite(V0, temperature); // Virtual pin V0 for temperature
  Blynk.virtualWrite(V1, humidity);    // Virtual pin V1 for humidity

  Blynk.run();

  // Update the PID input
  Input = temperature;

  // Compute the PID output
  myPID.Compute();

  // Scale the output based on distance from setpoint
  double distanceFromSetpoint = abs(Input - Setpoint);
  if (distanceFromSetpoint < 1.0) {  // Adjust as needed
    Output *= 0.5;  // Reduce output speed when close to setpoint
  }

  // Use the scaled PID output to control the bulb intensity
  analogWrite(32, Output);

  // Print PID output for debugging
  Serial.print("Analog Output: ");
  Serial.println(Output);

  // Calculate elapsed time in milliseconds
  elapsedTime = millis() - startTime;

  // Convert elapsed time to days, hours, minutes, and seconds
  seconds = elapsedTime / 1000;
  minutes = seconds / 60;
  hours = minutes / 60;
  days = hours / 24;
  seconds = seconds % 60;
  minutes = minutes % 60;
  hours = hours % 24;

  // Display temperature, humidity, analog output, and elapsed time on the OLED
  display.clearDisplay();
  display.setTextSize(1.7);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Temp: ");
  display.print(temperature);
  display.println(" C");
  display.print("Humidity: ");
  display.print(humidity);
  display.println(" %");
  display.print("Output: ");
  display.print(Output);
  display.println();
  display.print("Time: ");
  display.print(days);
  display.print("d ");
  display.print(hours);
  display.print("h ");
  display.print(minutes);
  display.print("m ");
  display.print(seconds);
  display.print("s");
  display.display();

  // Send data to Google Sheets
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    
    String url = String(googleScriptURL) + "?temperature=" + temperature + "&humidity=" + humidity;
    http.begin(url);
    int httpResponseCode = http.GET();
    
    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println(httpResponseCode);
      Serial.println(response);
    } else {
      Serial.print("Error on sending GET request: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  }

  // Wait a bit before the next loop
  delay(2000);
}
