#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Ultrasonic Sensor
const int trigPin = 10;
const int echoPin = 11;
long duration;
int distance;

// Servo
Servo myServo;

// LCD (change 0x27 to 0x3F if needed)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Buzzer Pin
const int buzzerPin = 8;

// Timing
const long updateInterval = 500;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  Serial.begin(9600);

  myServo.attach(12);
  lcd.init();
  lcd.backlight();
  lcd.clear();
}

void loop() {
  for (int i = 15; i <= 165; i++) {
    myServo.write(i);
    delay(30);

    distance = calculateDistance();
    Serial.print(i);
    Serial.print(",");
    Serial.print(distance);
    Serial.print(".");
    updateLCD(distance, i);
    controlBuzzer(distance);
  }

  for (int i = 165; i > 15; i--) {
    myServo.write(i);
    delay(30);

    distance = calculateDistance();
    Serial.print(i);
    Serial.print(",");
    Serial.print(distance);
    Serial.print(".");
    updateLCD(distance, i);
    controlBuzzer(distance);
  }
}

int calculateDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
  return distance;
}

void updateLCD(int dist, int angle) {
  static unsigned long lastUpdateTime = 0;
  static int lastDist = -1;
  static int lastAngle = -1;

  unsigned long currentMillis = millis();

  if (currentMillis - lastUpdateTime >= updateInterval) {
    lastUpdateTime = currentMillis;

    if (dist != lastDist || angle != lastAngle) {
      lastDist = dist;
      lastAngle = angle;

      lcd.setCursor(0, 0);
      if (dist > 0 && dist <= 100) {
        lcd.print("Object Detected  ");
      } else {
        lcd.clear();
        lcd.print("Searching...     ");
      }

      lcd.setCursor(0, 1);
      if (dist > 0 && dist <= 100) {
        String direction = getDirection(angle);
        lcd.print("Dist:");
        lcd.print(dist);
        lcd.print("cm Dir:");
        lcd.print(direction);
        lcd.print("");
      } else {
        lcd.print("");
      }
    }
  }
}

String getDirection(int angle) {
  if (angle >= 0 && angle < 45) return "S";
  else if (angle >= 45 && angle < 90) return "SE";
  else if (angle >= 90 && angle < 135) return "E";
  else if (angle >= 135 && angle < 180) return "NE";
  else return "N";
}

void controlBuzzer(int dist) {
  if (dist > 0 && dist <= 100) {
    digitalWrite(buzzerPin, HIGH);
  } else {
    digitalWrite(buzzerPin, LOW);
  }
}
