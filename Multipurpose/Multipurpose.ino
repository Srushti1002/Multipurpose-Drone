
//--------------------------------------------------------------------LED Declaration
int LED_White=12;
int LED_Green=35;
int LED_Yellow=36;
int LED_Red=37;

//---------------------------------------------------------------------Temperature & Humidity
#include <dht11.h>
#define DHT11PIN 40
dht11 DHT11;
//---------------------------------------------------------------------Gas Sensor
int buzzer = 47;
int smokeA5 = A5;
int sensorThres = 400;
//---------------------------------------------------------------------Vibration Sensor
int vib_pin=46;
//---------------------------------------------------------------------LDR (Light detecting Sensor)
int ldrPin = 41;
int ledPin= 12;
int val = 0;
//--------------------------------------------------------------------Ultrasonic sensor
const int trigPin = 22;
const int echoPin = 23;
float duration, distance;
//--------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------
void setup() 
{
  pinMode(buzzer, OUTPUT);
  pinMode(smokeA5, INPUT);
  pinMode(vib_pin,INPUT);
  pinMode(ldrPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(LED_White, OUTPUT);
  pinMode(LED_Green, OUTPUT);
  pinMode(LED_Yellow, OUTPUT);
  pinMode(LED_Red, OUTPUT);
  
  
  Serial.begin(9600);
  

}
//---------------------------------------------------------------------------------------------------
void loop() 
{   
  digitalWrite(LED_Green, HIGH);
  DHT_Reading();
  MQ2_Reading();
  Vibration_Reading(); 
  Light_Reading(); 
  Ultrasonic_Reading();
  delay(1000);
}
//---------------------------------------------------------------------------------------------------
void DHT_Reading()
{
  
  int chk = DHT11.read(DHT11PIN);

  Serial.print("Humidity (%): ");
  Serial.println((float)DHT11.humidity, 2);

  Serial.print("Temperature (C): ");
  Serial.println((float)DHT11.temperature, 2);
  

}
//---------------------------------------------------------------------------------------------------
void MQ2_Reading()
{
  
  int analogSensor = analogRead(smokeA5);

  Serial.print("Smoke reading: ");
  Serial.println(analogSensor);
  // Checks if it has reached the threshold value
  
  if (analogSensor > sensorThres)
  {
    tone(buzzer, 1500, 1000);
  }
  else
  {
    noTone(buzzer);
  }
  
}
//---------------------------------------------------------------------------------------------------
void Vibration_Reading()
{
  int val;
  val=digitalRead(vib_pin);
  if(val==1)
  {
    Serial.println("Vibration detected");    
  }
   
}
//---------------------------------------------------------------------------------------------------
void Light_Reading()
{
  int val = digitalRead(ldrPin);
  //Serial.println(val);
 
  if(val == HIGH )
  {
    digitalWrite(LED_White, HIGH);
    Serial.println("Light off");
  }
  else
  {
    digitalWrite(LED_White, LOW);
    Serial.println("Light ON");
  }

}
//---------------------------------------------------------------------------------------------------
void Ultrasonic_Reading()
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = (duration*.0343)/2;
  Serial.print("Object Distance: ");
  Serial.println(distance);
  Serial.println("-----------------------------------------------");

  if(distance < 30 )
  {
    digitalWrite(LED_Red, LOW);
    delay(100);
    digitalWrite(LED_Red, HIGH);
    delay(100);   
  }
  
  
  //delay(100);  
}
