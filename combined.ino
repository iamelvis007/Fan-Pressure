#include <Wire.h>
#include <Adafruit_MPL3115A2.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_MPL3115A2 baro = Adafruit_MPL3115A2();
Adafruit_BME280 bme; // use I2C interface
word fan = 3;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Adafruit_MPL3115A2 test!");
 if (!bme.begin(0x76)) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }
pinMode(fan, OUTPUT);
pwm25kHzBegin();
}
void pwm25kHzBegin() {
  TCCR2A = 0;                               // TC2 Control Register A
  TCCR2B = 0;                               // TC2 Control Register B
  TIMSK2 = 0;                               // TC2 Interrupt Mask Register
  TIFR2 = 0;                                // TC2 Interrupt Flag Register
  TCCR2A |= (1 << COM2B1) | (1 << WGM21) | (1 << WGM20);  // OC2B cleared/set on match when up/down counting, fast PWM
  TCCR2B |= (1 << WGM22) | (1 << CS21);     // prescaler 8
  OCR2A = 79;                               // TOP overflow value (Hz)
  OCR2B = 0;
}

void pwmDuty(byte ocrb) {
  OCR2B = ocrb;                             // PWM Width (duty)
}
void loop() {
  // put your main code here, to run repeatedly:
   if (! baro.begin()) {
    Serial.println("Couldnt find sensor");
    return;
    }
float pressBlue = (baro.getPressure()/100);
Serial.println(pressBlue);
float PressPurp = (bme.readPressure() / 100);
Serial.println(PressPurp);
if (pressBlue - PressPurp <0){
  pwmDuty(10);
  Serial.println("lowest");
}
if (2> pressBlue - PressPurp >=0){
  pwmDuty(29);
  Serial.println("low-mid");
}
if (3>pressBlue - PressPurp >=2){
  pwmDuty(59);
  Serial.println("high-mid");
}
if (pressBlue-PressPurp>=3){
  pwmDuty(79);
  Serial.println("high");
}
Serial.println(pressBlue-PressPurp);
}
