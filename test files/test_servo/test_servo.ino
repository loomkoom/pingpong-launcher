/*

test servo zonder app

*/

#include <PWMSoft.h>
#include <SoftPWM_timer.h>

const byte servopin = 11; // pin servoMotor
// standaard PWM grenzen
const byte PwmMin = 0.0;
const byte PwmMax = 255.0;
// De grenzen van softPWM om een servo aan te sturen
const byte servoMin = 8.0;
const byte servoMax = 33.0;
// De grenzen voor de hoek van de servo
const float angleMin = -105.0;
const float angleMax = 105.0;

void setup()
{
    SoftPWMBegin();
    delay(100);
    Serial.begin(9600);
    delay(200);
    Serial.println("Typ een waarde 8 tem 33: ");
}

void loop()
{
//    if (Serial.available())
//    {
//        float ANGLE = Serial.parseFloat();
//        ServoControlInput(ANGLE);
//    }
    ServoControlAutomaticAll();
    // ServoControlAutomatic3();
}

void ServoControlInput(int angle)
{
    if ((angle >= angleMin) && (angle <= angleMax))
    {
        float pwm = ((((105 - angle) / 210.0) * 25.0) + 8.0);
        SoftPWMSet(servopin, pwm);
        delay(100);
    }
    else
    {
        Serial.println("Waarde niet in range");
    }
}
void ServoControlAutomatic3()
{

    SoftPWMSet(servopin, 8);
    Serial.println(8);
    delay(3000);
    SoftPWMSet(servopin, 20);
    Serial.println(20);
    delay(3000);
    SoftPWMSet(servopin, 33);
}

void ServoControlAutomaticAll()
{
    SoftPWMSet(servopin, 8);
    delay(1000);
    SoftPWMSet(servopin, 9);
    delay(1000);
    SoftPWMSet(servopin, 10);
    delay(1000);
    SoftPWMSet(servopin, 11);
    delay(1000);
    SoftPWMSet(servopin, 12);
    delay(1000);
    SoftPWMSet(servopin, 13);
    delay(1000);
    SoftPWMSet(servopin, 14);
    delay(1000);
    SoftPWMSet(servopin, 15);
    delay(1000);
    SoftPWMSet(servopin, 16);
    delay(1000);
    SoftPWMSet(servopin, 17);
    delay(1000);
    SoftPWMSet(servopin, 18);
    delay(1000);
    SoftPWMSet(servopin, 19);
    delay(1000);
    SoftPWMSet(servopin, 20);
    delay(1000);
    SoftPWMSet(servopin, 21);
    delay(1000);

    SoftPWMSet(servopin, 22);
    delay(1000);
    SoftPWMSet(servopin, 23);
    delay(1000);
    SoftPWMSet(servopin, 24);
    delay(1000);
    SoftPWMSet(servopin, 25);
    delay(1000);
    SoftPWMSet(servopin, 26);
    delay(1000);
    SoftPWMSet(servopin, 27);
    delay(1000);
    SoftPWMSet(servopin, 28);
    delay(1000);
    SoftPWMSet(servopin, 29);
    delay(1000);
    SoftPWMSet(servopin, 30);
    delay(1000);
    SoftPWMSet(servopin, 31);
    delay(1000);
    SoftPWMSet(servopin, 32);
    delay(1000);
    SoftPWMSet(servopin, 33);
}
