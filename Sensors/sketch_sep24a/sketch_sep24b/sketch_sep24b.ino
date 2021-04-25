int ldrPin = 49;
int ledPin= 13;
int val = 0;
void setup() {
  pinMode(ldrPin, INPUT);
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
}
 
void loop() {
  int val = digitalRead(ldrPin);
  Serial.println(val);
 
  if(val == HIGH ){
    digitalWrite(ledPin, HIGH);
    Serial.println("LED ON");
  }
  else{
    digitalWrite(ledPin, LOW);
    Serial.println("LED OFF");
  }
}
