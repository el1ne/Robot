#ifndef Robot_h
#define Robot_h

#include <Arduino.h>

class IRSensor{
  public:
    void init();
    int getStr1200();
    int getStr600();
  protected:
    byte address;
    IRSensor(byte addr);
    const byte dir1200 = 0x04;
    const byte dir600 = 0x06;
    const byte str1200 = 0x05;
    const byte str600 = 0x07;
    int Read(byte message);
};

class IRLocator : public IRSensor{
  public:
    IRLocator() : IRSensor(0x0E) {}
    int getDir1200();
    int getDir600();
};

class IRSeeker : public IRSensor{
  public:
    IRSeeker() : IRSensor(0x1C) {}
    int getDir1200();
    int getDir600();
};

class Motor{
  public:
    Motor(int pwm, int ain1, int ain2);
    void start(int speed);
  private:
    int _pwm, _ain1, _ain2;
};

class Led{
  public:
    Led(int pin);
    void on();
    void off();
  private:
    int _pin;
};

class Gyro{
  public:
    void init();
    int read();
};

#endif
