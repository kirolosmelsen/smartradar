#include <IRremote.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>
#define REC 12
#define BUZZER 13
#define SERVO0 3
#define LED 2
#define MORE 64
#define LESS 25
#define BEFORE 7
#define AFTER 9
#define RUN 21
#define PIN 4
#define BLUE 5
#define PURPLE 6
#define YELLOW 7
LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo myservo0;
int pos=90;
int speed=1000;
int tempPin=0;
float tempVal;

void setup() {
  IrReceiver.begin(REC);
  pinMode(BUZZER, OUTPUT);
  pinMode(LED, OUTPUT);
  myservo0.attach(SERVO0);
if (!i2CAddrTest(0x27)) {
 lcd = LiquidCrystal_I2C(0x3F, 16, 2);
}
 lcd.init();
 lcd.backlight();
 startingAnimation();
 pinMode(PIN, INPUT);
 pinMode(BLUE, OUTPUT);
 pinMode(PURPLE, OUTPUT);
 pinMode(YELLOW, OUTPUT);
}

void loop() {
if (IrReceiver.decode()) {
  digitalWrite(LED, HIGH);
  digitalWrite(BUZZER, HIGH);
  delay(75);
  digitalWrite(BUZZER, LOW);
  digitalWrite(LED, LOW);
  IrReceiver.resume();
  int command=IrReceiver.decodedIRData.command;
  switch (command) {
  case MORE: {
  speed=2000;
  break; }
  case LESS: {
  speed=1000;
  break; }
  case BEFORE: {
  pos=myservo0.read();
  if (pos>=5) {
  myservo0.writeMicroseconds(speed);
  pos=pos-5;
  myservo0.write(pos); }
  break; }
  case AFTER: {
  pos=myservo0.read();
  if (pos<=175) {
  myservo0.writeMicroseconds(speed);
  pos=pos+5;
  myservo0.write(pos); }
  break;
  }
  case RUN: {
  digitalWrite(LED, HIGH);
  digitalWrite(BUZZER, HIGH);
  delay(300);
  digitalWrite(BUZZER, LOW);
  digitalWrite(LED, LOW);
  break; }
  default: {} }
}
tempVal=getTemp();
lcdDisplay();
digitalWrite(YELLOW, digitalRead(PIN));
delay(200);
}

void startingAnimation() {
 for (int i = 0; i < 16; i++) {
 lcd.scrollDisplayRight(); }
 lcd.print("JUST A MOMENT");
 for (int i = 0; i < 16; i++) {
 lcd.scrollDisplayLeft();
 delay(300); }
 lcd.clear();
}
bool i2CAddrTest(uint8_t addr) {
 Wire.begin();
 Wire.beginTransmission(addr);
 if (Wire.endTransmission() == 0) {
 return true; }
 return false;
}
void lcdDisplay() {
 lcd.setCursor(2, 0);
 lcd.print("SMART RADAR");
 lcd.setCursor(4, 1);
 lcd.print(tempVal); 
 lcd.print((char)223);
 lcd.print("C");
}
float getTemp() {
 int adcVal = analogRead(tempPin);
 float v = adcVal * 5.0 / 1024;
 float Rt = 10 * v / (5 - v);
 float tempK = 1 / (log(Rt / 10) / 3950 + 1 / (273.15 + 25));
 return tempK - 273.15;
}