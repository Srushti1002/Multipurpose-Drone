//Vibration Sensor Module (Code)Arduino with relay and external 5V power source.
  
int vib_pin=2;
int led_pin=13;


// put your setup code here, to run once:
void setup() { 
  pinMode(vib_pin,INPUT);
  pinMode(led_pin,OUTPUT);
}


  // put your main code here, to run repeatedly:
void loop() {
  int val;
  val=digitalRead(vib_pin);
  if(val==1)
  {
    digitalWrite(led_pin,LOW);
    delay(2000);
    digitalWrite(led_pin,HIGH);
    delay(1000);
    digitalWrite(led_pin,LOW);
    delay(2000);
    digitalWrite(led_pin,HIGH);
    delay(1000);
    digitalWrite(led_pin,LOW);
    delay(2000);
    digitalWrite(led_pin,HIGH);
    delay(1000);
    
   }
   else
   digitalWrite(led_pin,HIGH);
}


