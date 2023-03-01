/*

test servo met app

*/

#include <PWMSoft.h>
#include <SoftPWM_timer.h>
#include <AltSoftSerial.h>

AltSoftSerial bluetoothSerial; // bluetoothconnectie: RX op pin 9 via spanningsdeler, TX op pin 8
// variabelen voor commandos
String command;
bool CommandReceived;
// aangesloten pinnen
const byte servoPin = 11; // pin servoMotor
// standaard PWM grenzen
const byte PwmMin = 0.0;
const byte PwmMax = 255.0;
// De grenzen van softPWM om een servo aan te sturen
const byte servoMin = 8.0;
const byte servoMax = 33.0;
// De grenzen voor de hoek van de servo
const float angleMin = -105.0;
const float angleMax = 105.0;

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
        if (command.startsWith("SERVO/"))
        {
            SERVOreceivedInput(command);
        }
        else if (command.startsWith("ANGLE/"))
        {
            ANGLEreceivedInput(command);
        }
    }
}
// Bluetooth commando interpretatie
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

void SetAngle(int angle)
{
    if (IsDataGeldig(angle, angleMin, angleMax))
    {
        float pwm = ((((105 - angle) / 210.0) * 25.0) + 8.0);
        Serial.println(pwm);
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