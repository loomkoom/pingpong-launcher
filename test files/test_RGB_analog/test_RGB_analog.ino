

void setup() {
  // put your setup code here, to run once:
  pinMode(3,OUTPUT); //b
  pinMode(5,OUTPUT);  // g
  pinMode(7,OUTPUT);  // r
}

void loop() {
  // put your main code here, to run repeatedly:
  analogWrite(3,255);
  delay(1000);
  analogWrite(5,255);
  analogWrite(3,0);
  delay(1000);
  digitalWrite(7,HIGH);
  analogWrite(5,0);
  delay(1000);
  analogWrite(5,255);
  analogWrite(3,255);
  digitalWrite(7,HIGH);
  delay(1000);
  
}
