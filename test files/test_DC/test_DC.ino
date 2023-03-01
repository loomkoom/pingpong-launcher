/*

Testen voor DC motoren zonder app zonder aan uit functie

*/

#include <PWMSoft.h>
#include <SoftPWM_timer.h>

// aangesloten pinnen
const byte dcTransmissionPin = 10; // pin aandrijfmotor
const byte dcPropulsionPinUp = 6;  // pinnen afschietmotoren
const byte dcPropulsionPinLeft = 4;
const byte dcPropulsionPinRight = 2;
// standaard PWM grenzen
const byte PwmMin = 0.0;
const byte PwmMax = 255.0;
// De grenzen voor de snelheid van de propulsie
const byte SpeedMin = 0;
const byte SpeedMax = 5.06;

// setup code (eenmalig)
void setup()
{
    SoftPWMBegin();
    delay(100);
    Serial.begin(9600);
    delay(100);
}

// code die altijd uigevoerd wordt
void loop()
{
   // TransmissionControl();
   //PropulsionUpControl();
   //PropulsionLeftControl();
   //PropulsionRightControl();
}


void TransmissionControl() {
    DcControlTransmission(255);
    delay(1000);
    DcControlTransmission(125);
    delay(1000);
    DcControlTransmission(0);
    delay(1000);
    DcControlTransmission(125);
}
void PropulsionUpControl()
{
    DcControlPropulsionUp(255);
    delay(1000);
    DcControlPropulsionUp(125);
    delay(1000);
    DcControlPropulsionUp(0);
    delay(1000);
    DcControlPropulsionUp(125);
}
void PropulsionLeftControl()
{
    DcControlPropulsionLeft(255);
    delay(1000);
    DcControlPropulsionLeft(125);
    delay(1000);
    DcControlPropulsionLeft(0);
    delay(1000);
    DcControlPropulsionLeft(125);
}
void PropulsionRightControl()
{
    DcControlPropulsionRight(255);
    delay(1000);
    DcControlPropulsionRight(125);
    delay(1000);
    DcControlPropulsionRight(0);
    delay(1000);
    DcControlPropulsionRight(125);
}

void DcControlTransmission(int dcPwmT)
{
    if (IsDataGeldig(dcPwmT, PwmMin, PwmMax))
    
    { 
      Serial.println("PWM: " + String(dcPwmT) + "AAN");
        SoftPWMSet(dcTransmissionPin, dcPwmT);
    }
    else
    {
        Serial.println("PWM: " + String(dcPwmT) + " is geen geldige waarde voor de DCmotor!");
    }
}
void DcControlPropulsionUp(int dcPwmU)
{

    if (IsDataGeldig(dcPwmU, PwmMin, PwmMax))
    {

        SoftPWMSet(dcPropulsionPinUp, dcPwmU);
    }
    else
    {

        Serial.println("PWM: " + String(dcPwmU) + " is geen geldige waarde voor de DCmotor!");
    }
}
void DcControlPropulsionLeft(int dcPwmL)
{
    if (IsDataGeldig(dcPwmL, PwmMin, PwmMax))
    {

        SoftPWMSet(dcPropulsionPinLeft, dcPwmL);
    }
    else
    {
        Serial.println("PWM: " + String(dcPwmL) + " is geen geldige waarde voor de DCmotor!");
    }
}
void DcControlPropulsionRight(int dcPwmR)
{
    if (IsDataGeldig(dcPwmR, PwmMin, PwmMax))
    {
        SoftPWMSet(dcPropulsionPinRight, dcPwmR);
    }
    else
    {
        Serial.println("PWM: " + String(dcPwmR) + " is geen geldige waarde voor de DCmotor!");
    }
}


bool IsDataGeldig(float input, float Min, float Max)
{
    bool input_Geldig;
    if (input >= Min && input <= Max)
    {
        input_Geldig = true;
    }
    else
    {
        input_Geldig = false;
    }
    return input_Geldig;
}
