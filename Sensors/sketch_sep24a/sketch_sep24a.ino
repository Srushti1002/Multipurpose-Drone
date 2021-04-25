void setup() {
  pinMode(53, INPUT); //Pin 2 as INPUT
  pinMode(13, OUTPUT); //PIN 3 as OUTPUT
}

void loop() {
  if (digitalRead(53) == HIGH)
  {
  digitalWrite(13, HIGH);   // turn the LED/Buzz ON
  delay(100);                       // wait for 100 msecond 
  digitalWrite(13, LOW);   // turn the LED/Buzz OFF
  delay(100);                       // wait for 100 msecond 
  }
}
