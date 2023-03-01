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
int BallsAmount = 3;
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
const float dcTableMin = 19.0;
const float dcTableMax = 35.0;
// drempelwaardes voor sensormeting
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
// variabelen voor tijdsmetingen
unsigned long StartTimeSensors = 0;
unsigned long EndTimeSensors = 0;
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
  delay(100);
  bluetoothSerial.begin(9600);
  delay(100);
  Serial.begin(9600);
  delay(100);
}

// code die altijd uigevoerd wordt
void loop()
{
  CheckBluetoothInput();
  CheckMonitorInput();
  ExecuteBluetoothInput(CommandReceived);
  MeasureSpeed(phototransistorPin1, phototransistorPin2);
}

// Bluetooth commando communicatie
void CheckBluetoothInput()
{
  //Kijkt of er iets werd verzonden over Bluetooth, ontvangt het en decodeert het ook.
  while (bluetoothSerial.available())
  {
    delay(10);
    char c = bluetoothSerial.read();
    command += c; // voeg karakter c toe aan command string totdat alle verzonden karakters via bluetoothSerial opgeslagen zijn in command
  }
  if (command.length() > 0)
  {
    CommandReceived = true;
  }
  else
  {
    CommandReceived = false;
  }
}
void CheckMonitorInput()
{
  //Kijkt of er iets werd verzonden over Bluetooth, ontvangt het en decodeert het ook.
  while (Serial.available())
  {
    delay(10);
    char c = Serial.read();
    command += c; // voeg karakter c toe aan command string totdat alle verzonden karakters via bluetoothSerial opgeslagen zijn in command
  }
  if (command.length() > 0)
  {
    CommandReceived = true;
  }
  else
  {
    CommandReceived = false;
  }
}
// Bluetooth commando uitvoering
void ExecuteBluetoothInput(bool gotcommand)
{
  if (gotcommand)
  {
    //welk commando werd ontvangen?
    if (command.startsWith("DC"))
    {
      DCreceivedInput(command);
    }
    else if (command.startsWith("SERVO/"))
    {
      SERVOreceivedInput(command);
    }
    else if (command.startsWith("ANGLE/"))
    {
      ANGLEreceivedInput(command);
    }
    else if (command.startsWith("BALLFREQ/"))
    {
      BALLFREQreceivedInput(command);
    }
    else if (command.startsWith("POS/"))
    {
      POSreceivedInput(command);
    }
    else if (command.startsWith("MODE/"))
    {
      MODEreceivedInput(command);
    }
    else if (command.startsWith("RGB/"))
    {
      RGBreceivedInput(command);
    }
    else if (command.startsWith("FEEDBALL"))
    {
      FEEDBALLreceivedInput(command);
    }
  }
}

// Bluetooth commando interpretatie
void DCreceivedInput(String DCcommando)
{
  if (DCcommando == "DCP/UIT")
  {
    dcPropOnOff = 0;
    //DcControlPropulsion(dcPropOnOff, dcPWM);
  }
  else if (DCcommando == "DCP/AAN")
  {
    dcPropOnOff = 1;
    DcControlPropulsion(dcPropOnOff, dcPWM);
  }
  else if (DCcommando == "DCPU/UIT")
  {
    dcPropUpOnOff = 0;
    DcControlPropulsionUp(dcPropUpOnOff, dcPWM);
  }
  else if (DCcommando == "DCPU/AAN")
  {
    dcPropUpOnOff = 1;
    DcControlPropulsionUp(dcPropUpOnOff, dcPWM);
  }
  else if (DCcommando == "DCPL/UIT")
  {
    dcPropLeftOnOff = 0;
    DcControlPropulsionLeft(dcPropLeftOnOff, dcPWM);
  }
  else if (DCcommando == "DCPL/AAN")
  {
    dcPropLeftOnOff = 1;
    DcControlPropulsionLeft(dcPropLeftOnOff, dcPWM);
  }
  else if (DCcommando == "DCPR/UIT")
  {
    dcPropRightOnOff = 0;
    DcControlPropulsionRight(dcPropRightOnOff, dcPWM);
  }
  else if (DCcommando == "DCPR/AAN")
  {
    dcPropRightOnOff = 1;
    DcControlPropulsionRight(dcPropRightOnOff, dcPWM);
  }
  else if (DCcommando == "DCT/UIT")
  {
    dcTransOnOff = 0;
    DcControlTransmission(dcTransOnOff, dcPWM);
  }
  else if (DCcommando == "DCT/AAN")
  {
    dcTransOnOff = 1;
    DcControlTransmission(dcTransOnOff, dcPWM);
  }

  String DCnumber = DCcommando.substring(DCcommando.indexOf("/") + 1); // splits het commando op na de / om de parameter in te lezen.
  if (DCnumber.indexOf("D") > 0)
  {
    DCnumber = DCnumber.substring(0, DCnumber.indexOf("D"));
  }
  dcPWM = DCnumber.toInt();

  if (DCcommando.charAt(DCcommando.indexOf("/") - 1) == 'T')
  {
    DcControlTransmission(dcTransOnOff, dcPWM);
  }
  else if (DCcommando.charAt(DCcommando.indexOf("/") - 1) == 'U')
  {
    DcControlPropulsionUp(dcPropUpOnOff, dcPWM);
  }
  else if (DCcommando.charAt(DCcommando.indexOf("/") - 1) == 'L')
  {
    DcControlPropulsionLeft(dcPropLeftOnOff, dcPWM);
  }
  else if (DCcommando.charAt(DCcommando.indexOf("/") - 1) == 'R')
  {
    DcControlPropulsionRight(dcPropRightOnOff, dcPWM);
  }
  else if (DCcommando.charAt(DCcommando.indexOf("/") - 1) == 'P')
  {
    DcControlPropulsion(dcPropOnOff, dcPWM);
  }
  Serial.println("Uigevoerd commando: " + DCcommando + "state" + "Prop" + String(dcPropOnOff) + "Trans" + String(dcTransOnOff) + "Up" + String(dcPropUpOnOff) + "Left" + String(dcPropLeftOnOff) + "right" + String(dcPropRightOnOff));
  command = ""; // klaar om een nieuw commando te ontvangen
}
void SERVOreceivedInput(String SERVOcommando)
{
  String SERVOnumber = SERVOcommando.substring(SERVOcommando.indexOf("/") + 1); // splits het commando op na de / om de parameter in te lezen.
  if (SERVOnumber.indexOf("S") > 0)
  {
    SERVOnumber = SERVOnumber.substring(0, SERVOnumber.indexOf("S"));
  }
  int servoPWM = SERVOnumber.toInt();
  ServoControl(servoPWM);
  Serial.println("Uigevoerd commando: " + SERVOcommando);
  command = ""; // klaar om een nieuw commando te ontvangen
}
void ANGLEreceivedInput(String ANGLEcommando)
{
  String ANGLEnumber = ANGLEcommando.substring(ANGLEcommando.indexOf("/") + 1); // splits het commando op na de / om de parameter in te lezen.
  if (ANGLEnumber.indexOf("A") > 0)
  {
    ANGLEnumber = ANGLEnumber.substring(0, ANGLEnumber.indexOf("A"));
  }
  int ANGLE = ANGLEnumber.toInt();
  SetAngle(ANGLE);
  Serial.println("Uigevoerd commando: " + ANGLEcommando);
  command = ""; // klaar om een nieuw commando te ontvangen
}
void BALLFREQreceivedInput(String BALLFREQcommando)
{
  String BALLFREQnumber = BALLFREQcommando.substring(BALLFREQcommando.indexOf("/") + 1); // splits het commando op na de / om de parameter in te lezen.
  if (BALLFREQnumber.indexOf("V") > 0)
  {
    BALLFREQnumber = BALLFREQnumber.substring(0, BALLFREQnumber.indexOf("V"));
  }
  int BALLFREQ = BALLFREQnumber.toInt();
  SetBallFreq(BALLFREQ);
  Serial.println("Uigevoerd commando: " + BALLFREQcommando);
  command = ""; // klaar om een nieuw commando te ontvangen
}
void POSreceivedInput(String POScommando)
{
  String POSvariable = POScommando.substring(POScommando.indexOf("/") + 1); // splits het commando op na de / om de parameter in te lezen.
  if (POSvariable.indexOf("P") > 0)
  {
    POSvariable = POSvariable.substring(0, POSvariable.indexOf("P"));
  }
  String POSITION = String(POSvariable);
  SetPosition(POSITION);
  Serial.println("Uigevoerd commando: " + POScommando);
  command = ""; // klaar om een nieuw commando te ontvangen
}
void MODEreceivedInput(String MODEcommando)
{
  String MODEvariable = MODEcommando.substring(MODEcommando.indexOf("/") + 1); // splits het commando op na de / om de parameter in te lezen.
  if (MODEvariable.indexOf("M") > 0)
  {
    MODEvariable = MODEvariable.substring(0, MODEvariable.indexOf("M"));
  }
  String MODE = String(MODEvariable);
  StartMode(MODE);
  Serial.println("Uigevoerd commando: " + MODEcommando);
  command = ""; // klaar om een nieuw commando te ontvangen
}
void RGBreceivedInput(String RGBcommando)
{
  String RGBvariable = RGBcommando.substring(RGBcommando.indexOf("/") + 1); // splits het commando op na de / om de parameter in te lezen.
  if (RGBvariable.indexOf("R") > 0)
  {
    RGBvariable = RGBvariable.substring(0, RGBvariable.indexOf("R"));
  }
  String RGB = String(RGBvariable);
  SetRGB(RGB);
  Serial.println("Uigevoerd commando: " + RGBcommando);
  command = ""; // klaar om een nieuw commando te ontvangen
}
void FEEDBALLreceivedInput(String FEEDBALLcommando)
{
  FeedBall();
  Serial.println("Uigevoerd commando: " + FEEDBALLcommando);
  command = ""; // klaar om een nieuw commando te ontvangen
}

// bediening PPP

void SetRGB(String RGB)
{
  if (RGB == "RED")
  {
    Setcolor(255, 0, 0);
  }
  else if (RGB == "GREEN")
  {
    Setcolor(0, 255, 0);
  }
  else if (RGB == "BLUE")
  {
    Setcolor(0, 0, 255);
  }
}

void Setcolor(int RedValue, int GreenValue, int BlueValue)
{
  if ((IsDataGeldig(RedValue, PwmMin, PwmMax)) && (IsDataGeldig(GreenValue, PwmMin, PwmMax)) && (IsDataGeldig(BlueValue, PwmMin, PwmMax)))
  {
    SoftPWMSet(rgbPinRed, 255 - RedValue);
    SoftPWMSet(rgbPinGreen, 255 - BlueValue);
    SoftPWMSet(rgbPinBlue, 255 - GreenValue);
  }
  else
  {
    Serial.println("PWM: " + String(RedValue) + String(GreenValue) + String(BlueValue) + " PWM moet tussen 0 en 255 zitten! ");
  }
}

void SetPosition(String pos)
{
  if (pos == "LA")
  {
    SetAngle(-12);
    DcControlPropulsion(1, 32);
  }
  else if (pos == "RA")
  {
    SetAngle(10);
    DcControlPropulsion(1, 31);
  }
  else if (pos == "LV")
  {
    SetAngle(-13);
    DcControlPropulsion(1, 23);
  }
  else if (pos == "RV")
  {
    SetAngle(12);
    DcControlPropulsion(1, 23);
  }
}

void FeedBall()
{
  DcControlTransmission(1, 255);
  delay(7000);
  DcControlTransmission(0, 0);
}

void StartMode(String mode)
{
  int Frequency;
  int ANGLE;
  int SPEED;
  int DELAY;
  for (int i = 0; i <= BallsAmount; i++)
  {
    ANGLE = random(angleTableMin, angleTableMax);
    SetAngle(ANGLE);
    if (mode == "MAKKELIJK")
    {
      Setcolor(0, 255, 0); // groen
      Frequency = 220;
      DELAY = 3000;
    }
    else if (mode == "MIDDEN")
    {
      Setcolor(255, 100, 0); // gelig
      Frequency = 230;
      DELAY = 1500;
    }
    else if (mode == "MOEILIJK")
    {
      Setcolor(255, 0, 0); // rood
      Frequency = 255;
      DELAY = 500;
    }
    else if (mode == "WILLEKEURIG")
    {
      Setcolor(255, 0, 255); // paars
      Frequency = random(210, 255);
      DELAY = random(500, 1500);
    }
    SPEED = random(dcTableMin, dcTableMax);
    DcControlPropulsion(1, SPEED);
    SetBallFreq(Frequency);
    delay(8000);
    SetBallFreq(0);
    delay(DELAY);
  }
  Setcolor(255, 255, 255);
  SetBallFreq(0);
  DcControlPropulsion(1, 0);

}
void SetBallFreq(int pwm)
{
  DcControlTransmission(1, pwm);
}
void DcControlTransmission(int onoff, int dcPwmT)
{
  if (IsDataGeldig(dcPwmT, PwmMin, PwmMax))
  {
    SoftPWMSet(dcTransmissionPin, dcPwmT * onoff);
  }
  else
  {
    Serial.println("PWM: " + String(dcPwmT) + " is geen geldige waarde voor de DCmotor!");
  }
}
void DcControlPropulsionUp(int onoff, int dcPwmU)
{
  if (IsDataGeldig(dcPwmU, PwmMin, PwmMax))
  {
    SoftPWMSet(DcPropulsionPinUp, dcPwmU * onoff);
  }
  else
  {
    Serial.println("PWM: " + String(dcPwmU) + " is geen geldige waarde voor de DCmotor!");
  }
}
void DcControlPropulsionLeft(int onoff, int dcPwmL)
{
  if (IsDataGeldig(dcPwmL, PwmMin, PwmMax))
  {

    SoftPWMSet(DcPropulsionPinLeft, dcPwmL * onoff);
  }
  else
  {
    Serial.println("PWM: " + String(dcPwmL) + " is geen geldige waarde voor de DCmotor!");
  }
}
void DcControlPropulsionRight(int onoff, int dcPwmR)
{
  if (IsDataGeldig(dcPwmR, PwmMin, PwmMax))
  {
    SoftPWMSet(DcPropulsionPinRight, dcPwmR * onoff);
  }
  else
  {
    Serial.println("PWM: " + String(dcPwmR) + " is geen geldige waarde voor de DCmotor!");
  }
}
void DcControlPropulsion(int onoff, int dcPwm)
{
  if (IsDataGeldig(dcPwm, PwmMin, PwmMax))
  {
    SoftPWMSet(DcPropulsionPinUp, dcPwm * onoff);
    SoftPWMSet(DcPropulsionPinLeft, dcPwm * onoff);
    SoftPWMSet(DcPropulsionPinRight, dcPwm * onoff);
  }
  else
  {
    Serial.println(String(dcPWM) + " is geen geldige waarde voor de DCmotor!");
  }
}

void SetAngle(int angle)
{
  if (IsDataGeldig(angle, angleMin, angleMax))
  {
    float pwm = ((((105.0 - angle) / 210.0) * 25.0) + 8.0);
    ServoControl(pwm);
  }
  else
  {
    Serial.println("Hoek: " + String(angle) + " De hoek moet tussen -105 en 105 graden liggen! ");
  }
}
void ServoControl(int servoPWM)
{
  if (IsDataGeldig(servoPWM, servoMin, servoMax))
  {
    SoftPWMSet(servoPin, servoPWM);
  }
  else
  {
    Serial.println("PWM: " + String(servoPWM) + " servo PWM moet tussen 8 en 33 zitten! ");
  }
}
// snelheidsmeting
void resetSensors()
{
  // waardes sensor 1
  lastSensor1Value = 0;
  Sensor1Value = 0;
  lastSensor1Sent = 0;
  Sensor1Triggered = false;
  // waardes sensor 2
  lastSensor2Value = 0;
  Sensor2Value = 0;
  lastSensor2Sent = 0;
  Sensor2Triggered = false;
  // verandering meting
  change1 = 0;
  change2 = 0;
  // variabelen voor tijdsmeting
  StartTimeSensors = 0;
  EndTimeSensors = 0;
  elapsed_time = 0;
  // variabelen voor snelheid
  Speed = 0;
}
void checkSensor1(int pin)
{
  if (Sensor1Triggered)
  {
    return;
  }
  Sensor1Value = analogRead(pin);
  if (abs(lastSensor1Sent - Sensor1Value) >= changeThreshold)
  {
    lastSensor1Sent = Sensor1Value;
  }
  if (lastSensor1Value > Sensor1Value && lastSensor1Value > changeThreshold)
  {
    StartTimeSensors =  micros();
    Sensor1Triggered = true;
    //Serial.println("Trip 1:" + String(Sensor1Triggered));
  }
  lastSensor1Value = Sensor1Value;
}
void checkSensor2(int pin)
{
  if (Sensor2Triggered)
  {
    return;
  }
  Sensor2Value = analogRead(pin);
  if (abs(lastSensor2Sent - Sensor2Value) >= changeThreshold)
  {
    lastSensor2Sent = Sensor2Value;
  }
  //check enkel sensor 2 als sensor 1 al afgegaan is
  if (Sensor1Triggered && (lastSensor2Value > Sensor2Value) && lastSensor2Value > changeThreshold)
  {
    EndTimeSensors = micros();
    elapsed_time = EndTimeSensors - StartTimeSensors;
    Sensor2Triggered = true;
    //Serial.println("Trip 2:" + String(Sensor2Triggered));
  }
  lastSensor2Value = Sensor2Value;
}
void CalculateSpeed()
{
  if (Sensor1Triggered && Sensor2Triggered && elapsed_time > 0)
  {
    Speed = distance / float(elapsed_time / 1000.0);
  }
}
void SendSpeed()
{
  if (Speed > 0.0)
  {
    //Serial.println("Tijd (ms):" + String(elapsed_time));
    //Serial.println("Snelheid (m/s):" + String(Speed));
    bluetoothSerial.println(String(Speed) + " m/s");
    resetSensors();
  }
}
void MeasureSpeed(int pin1, int pin2)
{
  checkSensor1(pin1);
  checkSensor2(pin2);
  CalculateSpeed();
  delay(10);
  SendSpeed();
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
