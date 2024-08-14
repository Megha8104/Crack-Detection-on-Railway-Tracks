#include <SoftwareSerial.h>

// Define software serial pins for the GSM module
SoftwareSerial gsmSerial(8, 9); // RX, TX

// Define pins for the IR sensor and Ultrasonic sensor
const int irSensorPin = 2;
const int trigPin = 10;
const int echoPin = 11;
const int buzzerPin = 3;

void setup() {
  // Initialize serial communication for debugging
  Serial.begin(9600);+
  
  // Initialize software serial communication for the GSM module
  gsmSerial.begin(9600);
  
  // Set the IR sensor pin as input
  pinMode(irSensorPin, INPUT);

  // Set the Ultrasonic sensor pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Set the Buzzer pin as output
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW); // Ensure the buzzer is off initially

  // Wait for the GSM module to initialize
  delay(10000);
  
  // Send an initialization message
  sendSMS("Security System Initialized");
}

void loop() {
  // Read the value from the IR sensor
  int irSensorValue = digitalRead(irSensorPin);

  // Check the IR sensor value
  if (irSensorValue == HIGH) {
    // Motion detected, send an SMS alert
     activateBuzzer();
    sendSMS("Crack Detected!");
    delay(1000); // Wait for 10 seconds to avoid multiple alerts
  }

  // Measure the distance using the Ultrasonic sensor
  long duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  // Check the distance and send SMS if the distance is less than a threshold
  if (distance <= 50) { // Adjust the threshold as needed
    sendSMS("Crack Detected by Ultrasonic Sensor!");
    delay(1000); // Wait for 10 seconds to avoid multiple alerts
  }

  // Add a small delay before the next check
  delay(500);
}

void sendSMS(String message) {
  gsmSerial.println("AT+CMGF=1"); // Set SMS mode to text
  delay(1000);
  gsmSerial.println("AT+CMGS=\"+918431639618\""); // Replace with the recipient's phone number
  delay(1000);
  gsmSerial.println(message); // The message to send
  delay(1000);
  gsmSerial.write(26); // ASCII code of CTRL+Z to send the message
  delay(5000); // Wait for the SMS to be sent
}

void activateBuzzer() {
  digitalWrite(buzzerPin, HIGH); // Turn on the buzzer
  delay(1000); // Buzzer on duration
  digitalWrite(buzzerPin, LOW); // Turn off the buzzer
}