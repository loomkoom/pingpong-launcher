#include <AltSoftSerial.h>

/*
   Dit bestand initialiseert de HC-06 mySerial module. Je hoeft dit enkel te runnen indien je de naam en/of pincode van de module wilt aanpassen,
   nadien heb je dit niet meer nodig. Je kan de naam en pincode enkel wijzigen indien er geen mySerial connectie is met een ander apparaat. Je kan het resultaat
   en foutmeldingen bekijken via de seriële monitor, druk eventueel op reset om het process opnieuw op te starten en foutmeldingen te zien.

    5V --> Supply
    GND --> GND
    RX --> pinRX pin 9 VIA spanningsconversie!!!
    TX --> pinTX pin 8 (vb. 3)

*/

// Variabelen
String moduleName = "P&02-Groep412";  // De gewenste naam van je mySerialmodule (initieel HC-06)
String modulePIN = "5436";  // De gewenste pincode, exact 4 cijfers!


AltSoftSerial mySerial;
String result;
bool failure;


void setup() {

  mySerial.begin(9600); //HC-06 communicatie
  delay(100);
  Serial.begin(9600); //Seriële monitor
  delay(100);
  // enable for AT commands
  mySerial.println("AT");
  delay(1000);

  // change name
  mySerial.print("AT+NAME" + moduleName);
  delay(1000);
  result = "";
  while (mySerial.available()) {
    char c = mySerial.read();
    result += c;
  }
  Serial.println(result);
  if (result == "OKsetname") {
    Serial.println("Naam: " + moduleName);
    failure = false;
  }
  else {
    failure = true;
  }

  delay(100);
  // change PIN
  mySerial.print("AT+PIN" + modulePIN);
  delay(1000);
  result = "";
  while (mySerial.available()) {
    char c = mySerial.read();
    result += c;
  }

  Serial.println(result);
  if (result == "OKsetPIN") {
    Serial.println("PIN: " + modulePIN);
  }
  else {
    failure = true;
  }

  // bug
  if (failure) {
    Serial.println("Er ging ergens iets mis, mogelijke oorzaken:");
    Serial.println("1. Fout in de bedrading. (RX/TX omwisselen, SoftSerial pinnen, supply, ...)");
    Serial.println("2. Er is nog een apparaat verbonden via mySerial.");
    Serial.println("3. Pincode bestaat niet uit 4 cijfers.");
  }
  else {
    Serial.println("HC-06 Setup voltooid");
  }
}

void loop() {}
