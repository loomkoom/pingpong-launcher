/*
  test rgb zonder app
*/

#include <PWMSoft.h>
#include <SoftPWM_timer.h>

// aangesloten pinnen
const byte rgbPinRed = 7; // pinnen voor RGB-led
const byte rgbPinGreen = 5;
const byte rgbPinBlue = 3;
// standaard PWM grenzen
const byte PwmMin = 0.0;
const byte PwmMax = 255.0;

void setup()
{
  pinMode(rgbPinRed, OUTPUT);
  pinMode(rgbPinGreen, OUTPUT);
  pinMode(rgbPinBlue, OUTPUT);
  SoftPWMBegin();
  delay(100);
}
void loop()
{ Setcolor(0.0,0.0,0.0);
  delay(1000);
  Setcolor(255.0, 0.0, 0.0);        // Rood
  delay(1000);
  Setcolor(0.0, 255.0, 0.0);        // Groen
  delay(1000);
  Setcolor(0.0, 0.0, 255.0);        // Blauw
  delay(1000);
  Setcolor(255.0, 255.0, 255.0);    // Wit
  delay(1000);
}

void Setcolor(int RedValue, int GreenValue, int BlueValue)
{
  //    if ((IsDataGeldig(RedValue, PwmMin, PwmMax)) && (IsDataGeldig(GreenValue, PwmMin, PwmMax)) && (IsDataGeldig(BlueValue, PwmMin, PwmMax)))
  //
  SoftPWMSet(rgbPinRed, 255- RedValue);
  SoftPWMSet(rgbPinGreen,255-  GreenValue);
  SoftPWMSet(rgbPinBlue,255-  BlueValue);
  //    }
  //    else
  //    {
  //        Serial.println("PWM: " + String(RedValue) + String(GreenValue) + String(BlueValue) + " PWM moet tussen 0 en 255 zitten! ");
  //    }
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
