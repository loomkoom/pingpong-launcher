/*

test rgb met app

*/

#include <PWMSoft.h>
#include <SoftPWM_timer.h>
#include <AltSoftSerial.h>

AltSoftSerial bluetoothSerial; // bluetoothconnectie: RX op pin 9 via spanningsdeler, TX op pin 8
// variabelen voor commandos
String command;
bool CommandReceived;
// aangesloten pinnen
const byte rgbPinRed = 7; // pinnen voor RGB-led
const byte rgbPinGreen = 5;
const byte rgbPinBlue = 3;
// standaard PWM grenzen
const byte PwmMin = 0.0;
const byte PwmMax = 255.0;

// setup code (eenmalig)
void setup()
{
    pinMode(rgbPinRed, OUTPUT); // pinnen voor rgb pwm
    pinMode(rgbPinGreen, OUTPUT);
    pinMode(rgbPinBlue, OUTPUT);
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
        if (command.startsWith("MODE/"))
        {
            MODEreceivedInput(command);
        }
        if (command.startsWith("RGB/"))
        {
            RGBreceivedInput(command);
        }
    }
}

// Bluetooth commando interpretatie
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
    int RGB = RGBvariable.toInt();
    SetRGB(RGB);
    Serial.println("Uigevoerd commando: " + RGBcommando);
    command = ""; // klaar om een nieuw commando te ontvangen
}

// bediening PPP
void StartMode(String mode)
{

    if (mode == "MAKKELIJK")
    {
        Setcolor(45, 201, 55); // groen
    }
    else if (mode == "GEMMIDELD")
    {
        Setcolor(231, 180, 22); // oranje
    }
    else if (mode == "MOEILIJK")
    {
        Setcolor(45, 201, 55); // rood
    }
    else if (mode == "WILLEKEURIG")
    {
        Setcolor(0, 0, 255); // blauw
    }
}

void SetRGB(int RGB)
{
    if (RGB == 1)
    {
        Setcolor(255, 0, 0);
    }
    else if (RGB == 2)
    {
        Setcolor(0,255, 0);
    }
    else if (RGB == 3)
    {
        Setcolor(0,0,255);
    }
}

void Setcolor(int RedValue, int GreenValue, int BlueValue)
{
    if ((IsDataGeldig(RedValue, PwmMin, PwmMax)) && (IsDataGeldig(GreenValue, PwmMin, PwmMax)) && (IsDataGeldig(BlueValue, PwmMin, PwmMax)))
    {
        SoftPWMSet(rgbPinRed, RedValue);
        SoftPWMSet(rgbPinGreen, GreenValue);
        SoftPWMSet(rgbPinBlue, BlueValue);
    }
    else
    {
        Serial.println("PWM: " + String(RedValue) + String(GreenValue) + String(BlueValue) + " PWM moet tussen 0 en 255 zitten! ");
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