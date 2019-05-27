#include <Robot.h>
#include <Arduino.h>
#include <Wire.h>

IRSensor::IRSensor(byte addr){
  IRSensor::address = addr;
}

void IRSensor::init(){
  Wire.begin();
	Wire.beginTransmission(IRSensor::address);
	Wire.write(0x00);
	Wire.endTransmission();
	while(Wire.available() > 0)
	Wire.read();
}

int IRSensor::Read(byte message){
  Wire.beginTransmission(IRSensor::address);
  Wire.write(message);
  Wire.endTransmission();
  Wire.requestFrom(IRSensor::address, 1);
  while (Wire.available() < 1);
  int value = Wire.read();
  while(Wire.available() > 0)
    Wire.read();
  return value;
}

int IRSensor::getStr1200(){
  return Read(IRSensor::str1200);
}

int IRSensor::getStr600(){
  return Read(IRSensor::str600);
}

int IRLocator::getDir1200(){
  return int(Read(IRSensor::dir1200) * 5 + 540) % 360 - 180;
}

int IRLocator::getDir600(){
  return int(Read(IRSensor::dir600) * 5 + 540) % 360 - 180;
}

int IRSeeker::getDir1200(){
  int value = int(Read(IRSensor::dir1200) * 5 + 540) % 360 - 180;
  if (value > 127) value =- 255;
  return -value;
}

int IRSeeker::getDir600(){
  int value = int(Read(IRSensor::dir600) * 5 + 540) % 360 - 180;
  if (value > 127) value =- 255;
  return -value;
}

Motor::Motor(int pwm, int ain1, int ain2){
  pinMode(pwm, OUTPUT);
  pinMode(ain1, OUTPUT);
  pinMode(ain2, OUTPUT);
  _pwm = pwm;
  _ain1 = ain1;
  _ain2 = ain2;
}

void Motor::start(int speed){
  if (speed > 255) speed = 255;
  else if (speed < -255) speed = -255;
  if (speed >= 0){
    analogWrite(_pwm, speed);
    digitalWrite(_ain1, LOW);
    digitalWrite(_ain2, HIGH);
  }
  else {
    analogWrite(_pwm, -speed);
    digitalWrite(_ain1, HIGH);
    digitalWrite(_ain2, LOW);
  }
}

Led::Led(int pin){
  pinMode(pin, OUTPUT);
  _pin = pin;
}

void Led::on(){
  digitalWrite(_pin, HIGH);
}

void Led::off(){
  digitalWrite(_pin, LOW);
}

void Gyro::init(uint32_t time){
  uint32_t t = millis();
  Led ledL(11);
  Led ledM(7);
  Led ledR(3);
  while (millis() - t < time){
    ledL.off();
    ledM.off();
    ledR.on();
    delay(100);
    ledL.off();
    ledM.on();
    ledR.off();
    delay(100);
    ledL.on();
    ledM.off();
    ledR.off();
    delay(100);
  }
  for (int i = 0; i < 3; i++){
    ledL.on();
    ledM.on();
    ledR.on();
    delay(200);
    ledL.off();
    ledM.off();
    ledR.off();
    delay(200);
  }
}

int Gyro::read(){
  static int gyroValue;
  if (Serial3.available()) gyroValue = Serial3.read();
  return gyroValue - 126;
}
