/*

  test snelheidsmeting

*/
const byte phototransistorPin1 = A0; // pinnen voor IR-sensor
const byte phototransistorPin2 = A1;
const byte IRemitterPin1 = 1; // pinnen voor IR-emitter
const byte IRemitterPin2 = 2;
// drempelwaardes voor sensormeting
const int tripThreshold = 80;    //percent verandering
const int changeThreshold = 400; //absolute verandering
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
  pinMode(phototransistorPin1, INPUT); // pinnen voor ir-detector
  pinMode(phototransistorPin2, INPUT);
  pinMode(IRemitterPin1, OUTPUT); // pinnen voor ir-led
  pinMode(IRemitterPin2, OUTPUT);
  Serial.begin(9600);
  delay(100);
}

// code die altijd uigevoerd wordt
void loop()
{
  MeasureSpeed(phototransistorPin1, phototransistorPin2);
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
  startTime = 0;
  endtime = 0;
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
    Serial.println("Sensor 1:" + String(lastSensor1Sent));
  }
  if (lastSensor1Value > Sensor1Value)
  {
    change1 = ((float)(lastSensor1Value - Sensor1Value) / (float)Sensor1Value) * 100;
    if (change1 > tripThreshold)
    {
      Serial.println("Change 1:" + String(change1));
      startTime = millis();
      Sensor1Triggered = true;
      Serial.println("Trip 1:" + String(Sensor1Triggered));
    }
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
    Serial.println("Sensor 2:" + String(lastSensor2Sent));
  }
  //check enkel sensor 2 als sensor 1 al afgegaan is
  if (Sensor1Triggered && (lastSensor2Value > Sensor2Value))
  {
    change2 = ((float)(lastSensor2Value - Sensor2Value) / (float)Sensor2Value) * 200;

    if (change2 > tripThreshold)
    {
      Serial.println("Change 2:" + String(change2));
      endtime = millis();
      elapsed_time = endtime - startTime;
      Sensor2Triggered = true;
      Serial.println("Trip 2:" + String(Sensor2Triggered));
    }
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
  if (Speed > 0)
  {
    Serial.println(Speed);
    Serial.println("Tijd (ms):" + String(elapsed_time));
    Serial.println("Snelheid (m/s):" + String(Speed));
    resetSensors();
  }
}
void MeasureSpeed(int pin1, int pin2)
{
  checkSensor1(pin1);
  checkSensor2(pin2);
  CalculateSpeed();
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
