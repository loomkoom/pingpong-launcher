/*
  Assignment for P&O II:   PingPongPartner
    The circuit :
      INPUT : - app connected by a bluetooth module
              - phototransistors
      OUTPUT : - app connected by a bluetooth module
               - 4 DC motors
               - 1 srvo motor
               - 2 IR-emitters
               - 1 RGB led

    Created semester 2 2019
    By team 412

*/
#include <PWMSoft.h>
#include <SoftPWM_timer.h>
#include <AltSoftSerial.h>

AltSoftSerial bluetoothSerial; // bluetoothconnectie: RX op pin 9 via spanningsdeler, TX op pin 8
int dcPWM;
int dcPropOnOff = 0;
int dcTransOnOff = 0;
int dcPropUpOnOff = 0;
int dcPropLeftOnOff = 0;
int dcPropRightOnOff = 0;
int BallsAmount = 5;
// variabelen voor commandos
String command;
bool CommandReceived;
// aangesloten pinnen
const byte servoPin = 11;          // pin servoMotor
const byte dcTransmissionPin = 10; // pin aandrijfmotor
const byte DcPropulsionPinUp = 6;  // pinnen afschietmotoren
const byte DcPropulsionPinLeft = 4;
const byte DcPropulsionPinRight = 2;
const byte phototransistorPin1 = A0; // pinnen voor IR-sensor
const byte phototransistorPin2 = A1;
const byte rgbPinRed = 7; // pinnen voor RGB-led
const byte rgbPinGreen = 5;
const byte rgbPinBlue = 3;
// standaard PWM grenzen
const byte PwmMin = 0.0;
const byte PwmMax = 255.0;
// De grenzen van softPWM om een servo aan te sturen
const byte servoMin = 8.0;
const byte servoMax = 33.0;
// De grenzen voor de hoek van de servo
const float angleMin = -105.0;
const float angleMax = 105.0;
const float angleTableMin = -15.0;
const float angleTableMax = 15.0;
// grenzen voor de motoren om op het bord te belanden
const float dcTableMin = 15.0;
const float dcTableMax = 35.0;
// De grenzen voor de snelheid van de propulsie
const byte SpeedMin = 0;
const byte SpeedMax = 5.06;
// drempelwaardes voor sensormeting
//const int tripThreshold = 75;    //percent verandering
const int changeThreshold = 760; //absolute verandering
// waardes sensor 1
int lastSensor1Value = 0;
int Sensor1Value = 0;
int lastSensor1Sent = 0;
bool Sensor1Triggered = false;
// waardes sensor 2
int lastSensor2Value = 0;
int Sensor2Value = 0;
int lastSensor2Sent = 0;
bool Sensor2Triggered = false;
// verandering meting
float change1 = 0;
float change2 = 0;
// variabelen voor tijdsmeting
unsigned long startTime = 0;
unsigned long endtime = 0;
unsigned long elapsed_time = 0;
// variabelen voor snelheid
const float distance = 0.05;
float Speed = 0;


// setup code (eenmalig)
void setup()
{
  pinMode(3, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(7, OUTPUT);
  SoftPWMBegin();
  delay(20);
  SoftPWMSet(3, 0);
  SoftPWMSet(5, 0);
  SoftPWMSet(7, 0);
  delay(100)
      bluetoothSerial.begin(9600);
  delay(100);
  Serial.begin(9600);
  delay(100);
}

// code die altijd uigevoerd wordt
void loop()
{
  CheckBluetoothInput();
  ExecuteBluetoothInput(CommandReceived);
  MeasureSpeed(phototransistorPin1, phototransistorPin2);
}

// Bluetooth commando communicatie (ontvangen)
void CheckBluetoothInput(){}

// Bluetooth commando uitvoering
void ExecuteBluetoothInput(bool gotcommand){}

// Bluetooth commando interpretatie
void DCreceivedInput(String DCcommando){}
void SERVOreceivedInput(String SERVOcommando){}
void ANGLEreceivedInput(String ANGLEcommando){}
void BALLFREQreceivedInput(String BALLFREQcommando){}
void POSreceivedInput(String POScommando){}
void MODEreceivedInput(String MODEcommando){}
void RGBreceivedInput(String RGBcommando){}
void FEEDBALLreceivedInput(String FEEDBALLcommando){}

// bediening PPP
void SetPosition(String pos){}
void FeedBall() {}

void StartMode(String mode) {}
void SetRGB(String RGB) {}
void Setcolor(int RedValue,int GreenValue, int BlueValue) {}

void SetBallFreq(int speedPercent) {}

void DcControlTransmission(int onoff, int dcPwmT) {}
void DcControlPropulsionUp(int onoff, int dcPwmU) {}
void DcControlPropulsionLeft(int onoff, int dcPwmL) {}
void DcControlPropulsionRight(int onoff, int dcPwmR) {}
void DcControlPropulsion(int onoff, int dcPwm) {}

void SetAngle(int angle) {}
void ServoControl(int servoPWM) {}

// snelheidsmeting
void resetSensors() {}

void checkSensor1(int pin) {}
void checkSensor2(int pin) {}
void CalculateSpeed() {}
void SendSpeed() {}
void MeasureSpeed(int pin1, int pin2) {}

// hulp functie
bool IsDataGeldig(float input, float Min, float Max) {}
