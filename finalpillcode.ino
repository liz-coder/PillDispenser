#include <Wire.h>
#include <RTClib.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>

// RTC module
RTC_DS3231 rtc;

// I2C LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Servo motor for pill dispenser
Servo pillMotor;

// Motor pin
const int pillMotorPin = A0;

// Water Pump pin
const int waterPumpPin = 6; // Gate of the IRF3710 is connected to this pin

// Buzzer and LED pin
const int buzzerPin = 5;
const int ledPin = 13;

// Button pins
const int setTimeButtonPin = 2;
const int increaseTimeButtonPin = 3;
const int decreaseTimeButtonPin = 4;
const int increaseQuantityButtonPin = 8;
const int toggleButtonPin = 7; 
const int dispenseButtonPin = 9; // Button to manually dispense a pill

// Pill dispensing time and quantity
DateTime pillTime;
int pillQuantity = 1;

// Set pill time flag and toggle hour/minute flag
bool setTimeFlag = false;
bool toggleFlag = true; 

// Button debounce delay
const int debounceDelay = 100;

void setup() {
  Serial.begin(9600);

  // Initialize LCD
  lcd.init();
  lcd.backlight();

  // Initialize RTC
  if (!rtc.begin()) {
    lcd.print("RTC not found");
    while (1);
  }

  // Set servo pin
  pillMotor.attach(pillMotorPin);
  pillMotor.write(0); 

  // Set water pump pin as output
  pinMode(waterPumpPin, OUTPUT);

  // Set buzzer and LED pin as output
  pinMode(buzzerPin, OUTPUT);
  pinMode(ledPin, OUTPUT);

  // Button inputs with pull-up resistors
  pinMode(setTimeButtonPin, INPUT_PULLUP);
  pinMode(increaseTimeButtonPin, INPUT_PULLUP);
  pinMode(decreaseTimeButtonPin, INPUT_PULLUP);
  pinMode(increaseQuantityButtonPin, INPUT_PULLUP);
  pinMode(toggleButtonPin, INPUT_PULLUP);
  pinMode(dispenseButtonPin, INPUT_PULLUP);

  // Set default pill dispense time
  pillTime = DateTime(rtc.now().year(), rtc.now().month(), rtc.now().day(), 8, 0, 0);
}

void loop() {
  DateTime now = rtc.now();

  // Display current time and pill dispensing info
  lcd.setCursor(0, 0);
  lcd.print("Time: ");
  if (now.hour() < 10) lcd.print('0');
  lcd.print(now.hour());
  lcd.print(":");
  if (now.minute() < 10) lcd.print('0');
  lcd.print(now.minute());
  lcd.print(":");
  if (now.second() < 10) lcd.print('0');
  lcd.print(now.second());

  lcd.setCursor(0, 1);
  lcd.print("Pill time: ");
  if (pillTime.hour() < 10) lcd.print('0');
  lcd.print(pillTime.hour());
  lcd.print(":");
  if (pillTime.minute() < 10) lcd.print('0');
  lcd.print(pillTime.minute());

  // Check if it's time to dispense a pill
  if (now.hour() == pillTime.hour() && now.minute() == pillTime.minute() && now.second() == pillTime.second()) {
    dispensePills(pillQuantity);
  }

  // Check button states and perform respective functions
  if (!digitalRead(setTimeButtonPin)) {
    setTimeFlag = !setTimeFlag;
    delay(debounceDelay);
  }
  if (!digitalRead(toggleButtonPin)) {
    toggleFlag = !toggleFlag;
    delay(debounceDelay);
  }
  if (!digitalRead(increaseTimeButtonPin)) {
    if (setTimeFlag) {
      if (toggleFlag) {
        pillTime = DateTime(pillTime.year(), pillTime.month(), pillTime.day(), (pillTime.hour() + 1) % 24, pillTime.minute(), pillTime.second());
      } else {
        pillTime = DateTime(pillTime.year(), pillTime.month(), pillTime.day(), pillTime.hour(), (pillTime.minute() + 1) % 60, pillTime.second());
      }
      delay(debounceDelay);
    }
  }
  if (!digitalRead(decreaseTimeButtonPin)) {
    if (setTimeFlag) {
      if (toggleFlag) {
        pillTime = DateTime(pillTime.year(), pillTime.month(), pillTime.day(), (pillTime.hour() - 1 + 24) % 24, pillTime.minute(), pillTime.second());
      } else {
        pillTime = DateTime(pillTime.year(), pillTime.month(), pillTime.day(), pillTime.hour(), (pillTime.minute() - 1 + 60) % 60, pillTime.second());
      }
      delay(debounceDelay);
    }
  }
  if (!digitalRead(increaseQuantityButtonPin)) {
    pillQuantity++;
    delay(debounceDelay);
  }
  if (!digitalRead(dispenseButtonPin)) {
    dispensePills(pillQuantity);
    delay(debounceDelay);
  }
}

void dispensePills(int quantity) {
  for (int i = 0; i < quantity; i++) {
    pillMotor.write(180);
    delay(500);
    pillMotor.write(0);
    delay(500);
  }

  // Turn on water pump
  analogWrite(waterPumpPin, 255); // Fully turn on the pump
  delay(2000); // Pump runs for 2 seconds
  analogWrite(waterPumpPin, 0); // Turn off the pump
   
// Play an exciting alert
for(int i = 0; i < 50; i++) {
  digitalWrite(ledPin, HIGH);
  tone(buzzerPin, 2000 - i*50); // Descending pitch tones
  delay(50); // Faster blinks and tones
  digitalWrite(ledPin, LOW);
  noTone(buzzerPin);
  delay(50); // Faster blinks and tones
}
}
