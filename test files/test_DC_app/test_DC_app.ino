/*

Testen voor DC motoren met app zonder aan uit functie

*/

#include <PWMSoft.h>
#include <SoftPWM_timer.h>
#include <AltSoftSerial.h>

AltSoftSerial bluetoothSerial; // bluetoothconnectie: RX op pin 9 via spanningsdeler, TX op pin 8
int dcPWM;
int BallsAmount = 10;
// variabelen voor commandos
String command;
bool CommandReceived;
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
        else if (command.startsWith("VELOCITY/"))
        {
            VELOCITYreceivedInput(command);
        }
        else if (command.startsWith("BALLFREQ/"))
        {
            BALLFREQreceivedInput(command);
        }
    }
}

// Bluetooth commando interpretatie
void DCreceivedInput(String DCcommando)
{
    String DCnumber = DCcommando.substring(DCcommando.indexOf("/") + 1); // splits het commando op na de / om de parameter in te lezen.

    if (DCnumber.indexOf("D") > 0)
    {
        DCnumber = DCnumber.substring(0, DCnumber.indexOf("D"));
    }

    int dcPWM = DCnumber.toInt();
    if (DCcommando.charAt(DCcommando.indexOf("/") - 1) == 'T')
    {
        DcControlTransmission(dcPWM);
    }
    else if (DCcommando.charAt(DCcommando.indexOf("/") - 1) == 'U')
    {
        DcControlPropulsionUp(dcPWM);
    }
    else if (DCcommando.charAt(DCcommando.indexOf("/") - 1) == 'L')
    {
      Serial.println("test");
        DcControlPropulsionLeft(dcPWM);
    }
    else if (DCcommando.charAt(DCcommando.indexOf("/") - 1) == 'R')
    {
        DcControlPropulsionRight(dcPWM);
    }
    else
    {
        DcPropulsion(dcPWM);
    }
    Serial.println("Uigevoerd commando: " + DCcommando);
    command = ""; // klaar om een nieuw commando te ontvangen
}
void VELOCITYreceivedInput(String VELOCITYcommando)
{
    String VELOCITYnumber = VELOCITYcommando.substring(VELOCITYcommando.indexOf("/") + 1); // splits het commando op na de / om de parameter in te lezen.
    if (VELOCITYnumber.indexOf("V") > 0)
    {
        VELOCITYnumber = VELOCITYnumber.substring(0, VELOCITYnumber.indexOf("V"));
    }
    int VELOCITY = VELOCITYnumber.toInt();
    SetVelocity(VELOCITY);
    Serial.println("Uigevoerd commando: " + VELOCITYcommando);
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

void SetVelocity(float velocity)
{
    if (IsDataGeldig(velocity, SpeedMin, SpeedMax))
    {
        int pwm = (velocity / 5.06) * 255;
        DcPropulsion(pwm);
    }
    else
    {
        Serial.println("speed: " + String(velocity) + " is geen geldige waarde voor de snelheid!");
    }
}
void SetBallFreq(int speed)
{
    //float pwm = (speed / maxspeed) * 255;
    //DcControlTransmission(pwm);
}
void DcControlTransmission(int dcPwmT)
{
    if (IsDataGeldig(dcPwmT, PwmMin, PwmMax))
    {
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
void DcPropulsion(int dcPwm)
{
    if (IsDataGeldig(dcPwm, PwmMin, PwmMax))
    {
        SoftPWMSet(dcPropulsionPinUp, dcPwm);
        SoftPWMSet(dcPropulsionPinLeft, dcPwm);
        SoftPWMSet(dcPropulsionPinRight, dcPwm);
    }
    else
    {
        Serial.println(String(dcPWM) + " is geen geldige waarde voor de DCmotor!");
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
