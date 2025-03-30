#include <LiquidCrystal.h>
#include <Servo.h>

#define LDR_PIN A0
#define SERVO_PIN A1
#define BUZZER_PIN 8
#define BUTTON_PIN 9
#define LED_PIN 10
#define RGB_RED_PIN 13
#define RGB_GREEN_PIN 12
#define RGB_BLUE_PIN 11

LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
Servo myServo;

bool systemPaused = false;
bool lastButtonState = HIGH;
bool buttonProcessed = false;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 100;

void setup() {
  pinMode(LDR_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  pinMode(RGB_RED_PIN, OUTPUT);
  pinMode(RGB_GREEN_PIN, OUTPUT);
  pinMode(RGB_BLUE_PIN, OUTPUT);

  lcd.begin(16, 2);
  myServo.attach(SERVO_PIN);


}

void loop() {
  handleButton();
  if (systemPaused) return; 

  int lightValue = analogRead(LDR_PIN);
  int lightPercentage = map(lightValue, 1023, 0, 0, 100); 

  lcd.setCursor(0, 0);
  lcd.print("Light: ");
  lcd.print(lightPercentage);
  lcd.print("%     ");

  if (lightValue >= 650) { 
    lcd.setCursor(0, 1);
    lcd.print("Dark          "); 
    digitalWrite(RGB_RED_PIN, HIGH);
    digitalWrite(RGB_GREEN_PIN, HIGH);
    digitalWrite(RGB_BLUE_PIN, LOW);   
    digitalWrite(BUZZER_PIN, LOW);     
    myServo.write(0);                
  } else if (lightValue > 300 && lightValue <= 650) { 
    lcd.setCursor(0, 1);
    lcd.print("Normal        ");
    digitalWrite(RGB_RED_PIN, HIGH);
    digitalWrite(RGB_GREEN_PIN, LOW); 
    digitalWrite(RGB_BLUE_PIN, HIGH);
    digitalWrite(BUZZER_PIN, LOW);    
    myServo.write(100);                 
  } else { 
    lcd.setCursor(0, 1);
    lcd.print("Very Bright   ");
    digitalWrite(RGB_RED_PIN, LOW);   
    digitalWrite(RGB_GREEN_PIN, HIGH);
    digitalWrite(RGB_BLUE_PIN, HIGH);
    digitalWrite(BUZZER_PIN, HIGH);  
    myServo.write(180);         

  }

  delay(200); 
}

void handleButton() {
  int reading = digitalRead(BUTTON_PIN);

 
  if (reading != lastButtonState) {
    lastDebounceTime = millis(); 
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading == LOW && !buttonProcessed) {
      buttonProcessed = true;
      if (!systemPaused) {
        pauseSystem();
      } else {
        resumeSystem();
      }
    }
  }

  if (reading == HIGH) {
    buttonProcessed = false; 
  }

  lastButtonState = reading; 
}

void pauseSystem() {
  systemPaused = true;

  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(RGB_RED_PIN, HIGH);
  digitalWrite(RGB_GREEN_PIN, HIGH);
  digitalWrite(RGB_BLUE_PIN, HIGH);
  digitalWrite(LED_PIN, HIGH); 
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("System Paused ");
}

void resumeSystem() {
  systemPaused = false;
  digitalWrite(LED_PIN, LOW);
  lcd.clear();
}