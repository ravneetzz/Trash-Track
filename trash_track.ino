#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <HCSR04.h>
#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "Param wifi";  //add your wifi name
const char* password = "Ravneet2929"; //add your wifi password
const char* mqtt_server = "192.168.0.103"; //add wifi ip address 
const char* mqtt_username = "ravneetzz"; //add mqtt username
const char* mqtt_password = "password2929"; // add mqtt password

WiFiClient espClient; 
PubSubClient client(espClient);

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT);

int triggerPin = 13;
int echoPin = 12;
int countt = 0;
UltraSonicDistanceSensor distanceSensor(triggerPin, echoPin);

float initialDistance = 0; // Initial distance recorded by the sensor
const int pingThreshold = 75; // Set the threshold for 75% full

void setup() {
  Serial.begin(115200);

  /* OLED Setup */
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
  }
  delay(2000);
  display.clearDisplay();
  display.setTextColor(WHITE);

  // WiFi and MQTT setup
  setup_wifi();
  client.setServer(mqtt_server, 1883);

  // Record initial distance
  recordInitialDistance();
}

void setup_wifi() {
  delay(10);
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
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("UltrasonicSensor", mqtt_username, mqtt_password)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void recordInitialDistance() {
  Serial.println("Recording initial distance. Please wait...");

  // Allow the sensor to stabilize for a brief period
  delay(2000);

  // Record the initial distance as an average of readings over 1 minute
  unsigned long startTime = millis();
  unsigned long durationSum = 0;
  unsigned int readingsCount = 0;

  while (millis() - startTime < 60000) { // Record for 1 minute
    long distance = distanceSensor.measureDistanceCm();
    if (distance > 0) { // Ignore invalid readings
      durationSum += distance;
      readingsCount++;
    }
    delay(5000); // Delay between readings
  }

  if (readingsCount > 0) {
    initialDistance = durationSum / readingsCount;
    Serial.print("Initial distance recorded: ");
    Serial.println(initialDistance);
  } else {
    Serial.println("Error recording initial distance. Please check the sensor.");
  }
}

void sendMQTTNotification(float distance) {
  // Format the message
  String message = "Dustbin is ";
  message += ((initialDistance - distance) / initialDistance) * 100;
  message += "% full. Please empty it.";

  // Publish the message to the MQTT topic
  client.publish("ultrasonic/notification", message.c_str());
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }

  client.loop();

  long distance = distanceSensor.measureDistanceCm();

  // Clear display
  display.clearDisplay();

  // Calculate the percentage of dustbin full
  int percentFull = int(((initialDistance - distance) / initialDistance) * 100);

  // Display percentage on OLED
  display.setTextSize(2);
  display.setCursor(0, 10);
  //display.print("Full %:");
  display.setCursor(0, 30);
  display.print(percentFull);
  display.print("% Full");
  display.display();

  // Display current distance on Serial Monitor
  Serial.print("Current Depth: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Publish the distance to the MQTT topic
  client.publish("ultrasonic/distance", String(distance).c_str());

  // Check if dustbin is 75% full and send MQTT notification
  if (percentFull > pingThreshold) {
    countt = countt + 1;
    if (countt > 5) {
      Serial.println("Sending MQTT Notification");
      sendMQTTNotification(distance);
      delay(60000);
    }
  } else {
    countt = 0;
  }

  delay(5000); // Adjust the delay based on your needs
}
