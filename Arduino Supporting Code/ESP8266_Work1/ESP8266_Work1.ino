
//--------------------------------------------------------------------LED Declaration
int LED_White=53;
int LED_Green=35;
int LED_Yellow=36;
int LED_Red=37;

//---------------------------------------------------------------------GPS Module
#include <TinyGPS++.h>
TinyGPSPlus gps;
float Latitude_Status=0,Longitude_Status=0;
//---------------------------------------------------------------------Temperature & Humidity
#include <dht11.h>
int DHT11_Pin=40;
dht11 DHT11;
float DHT_Temp, DHT_Humidity;
//---------------------------------------------------------------------Gas Sensor
int Buzzer_Pin=47;
int MQ2_Pin=A5;
int Smoke_Alarm_Setpoint=400;
int Smoke_Reading;
//---------------------------------------------------------------------Vibration Sensor
int Vibration_Pin=46;
int Vibration_Status;
//---------------------------------------------------------------------LDR (Light detecting Sensor)
int Light_Sensor_Pin=41;
int Light_Status=0;
//--------------------------------------------------------------------Ultrasonic sensor
const int Ultrasonic_Trigger_Pin=22;
const int Ultrasonic_Echo_Pin=23;
float Ultrasonic_Wave_Travel_Duration, Obstacle_Distance;
//--------------------------------------------------------------------WiFi module (ESP8266) supporting declaration

String server=""; //variable for sending data to webpage
boolean No_IP=false; //variable to check for ip Address
String IP=""; //variable to store ip Address
char temp1='0';

int a=0;
int b=0;

//----------------------------------------------------------------------------------------------------------------

void setup() 
{

  pinMode(Buzzer_Pin, OUTPUT);
  pinMode(LED_White, OUTPUT);
  pinMode(LED_Green, OUTPUT);
  pinMode(LED_Yellow, OUTPUT);
  pinMode(LED_Red, OUTPUT);
  
  pinMode(DHT11_Pin, INPUT);  
  pinMode(MQ2_Pin, INPUT);  
  pinMode(Vibration_Pin,INPUT);
  pinMode(Light_Sensor_Pin, INPUT);
  
  pinMode(Ultrasonic_Trigger_Pin, OUTPUT);
  pinMode(Ultrasonic_Echo_Pin, INPUT);  
  
  
  Serial.begin(9600); //Serial monitor initiate
  Serial2.begin(9600); //GPS module initiate
  Serial3.begin(115200); //ESP8266 initiate
  wifi_init();
  Serial.println("System Ready..");
}
//----------------------------------------------------------------------------------------------------------------
void loop() 
{
  //--------------------------------------------
  b=0;
  Serial.println("Refresh Page");
  while(b<1000)
  {
    b++;
    while(Serial3.available())
    {
      if(Serial3.find("0,CONNECT"))
      {
        Serial.println("Starting");
        sendToServer();
        Serial.println("Finished");
        delay(1000);
      }
    }
    delay(1);
  }
  //---------------------------------------------
  digitalWrite(LED_Green, HIGH);
  DHT_Reading();
  MQ2_Reading();
  Vibration_Reading(); 
  Light_Reading(); 
  Ultrasonic_Reading();
  GPS_Reading();
  //---------------------------------------------
  delay(1000);
}
//----------------------------------------------------------------------------------------------------------------

void DHT_Reading()
{
  
  int chk = DHT11.read(DHT11_Pin); 

  DHT_Temp=DHT11.temperature;
  DHT_Humidity=DHT11.humidity;

}

//---------------------------------------------------------------------------------------------------
void MQ2_Reading()
{
  
  Smoke_Reading = analogRead(MQ2_Pin);  
  
  if (Smoke_Reading > Smoke_Alarm_Setpoint)
  {
    tone(Buzzer_Pin, 1500, 1000);
    digitalWrite(LED_Red, HIGH);
  }
  else
  {
    noTone(Buzzer_Pin);
    digitalWrite(LED_Red, LOW);
  }
  
}
//---------------------------------------------------------------------------------------------------
void Vibration_Reading()
{
  
  Vibration_Status=digitalRead(Vibration_Pin);
  if(Vibration_Status==1)
  {
    digitalWrite(LED_Yellow, HIGH);    
  }
  else
  {
    digitalWrite(LED_Yellow, LOW);    
  }
   
}
//---------------------------------------------------------------------------------------------------
void Light_Reading()
{
  Light_Status = digitalRead(Light_Sensor_Pin);  
 
  if(Light_Status == HIGH )
  {
    digitalWrite(LED_White, HIGH);    
  }
  else
  {
    digitalWrite(LED_White, LOW);    
  }

}
//---------------------------------------------------------------------------------------------------
void Ultrasonic_Reading()
{
  digitalWrite(Ultrasonic_Trigger_Pin, LOW);
  delayMicroseconds(2);
  digitalWrite(Ultrasonic_Trigger_Pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(Ultrasonic_Trigger_Pin, LOW);

  Ultrasonic_Wave_Travel_Duration = pulseIn(Ultrasonic_Echo_Pin, HIGH);
  Obstacle_Distance = (Ultrasonic_Wave_Travel_Duration*.0343)/2;  

  if(Obstacle_Distance < 30 )
  {
    digitalWrite(LED_Green, LOW);
    delay(100);
    digitalWrite(LED_Green, HIGH);
    delay(100);   
  }  
   
}
//---------------------------------------------------------------------------------------------------
void GPS_Reading()
{
  while (Serial2.available() > 0)
  {
    gps.encode(Serial2.read());
    if (gps.location.isUpdated())
    {
      Latitude_Status=(gps.location.lat(), 6);
      Longitude_Status=(gps.location.lng(), 6);
      Serial.print(gps.location.lat(), 6);
      Serial.println(gps.location.lng(), 6);      
    }
    
  }
}

//----------------------------------------------------------------------------------------------------------------

void findIp(int time1) //check for the availability of IP Address
{
  int time2=millis();
  while(time2+time1>millis())
  {
    while(Serial3.available()>0)
    {
      if(Serial3.find("IP has been read"))
      {
        No_IP=true;
      }
    }
  }
}
//----------------------------------------------------------------------------------------------------------------
void showIP()//Display the IP Address 
{
  IP="";
  char ch=0;
  while(1)
  {
    Serial3.println("AT+CIFSR");
    while(Serial3.available()>0)
    {
      if(Serial3.find("STAIP,"))
      {
        delay(1000);
        Serial.print("IP Address:");
        while(Serial3.available()>0)
        {
          ch=Serial3.read();
          if(ch=='+')
          break;
          IP+=ch;
        }
      }
      if(ch=='+')
      break;
    }
    if(ch=='+')
    break;
    delay(1000);
  }
  Serial.print(IP);
  Serial.print("Port:");
  Serial.println(80);
}
//----------------------------------------------------------------------------------------------------------------

void establishConnection(String command, int timeOut) //Define the process for sending AT commands to module
{
  int q=0;
  while(1)
  {
    Serial.println(command);
    Serial3.println(command); 
    while(Serial3.available())
    {
      if(Serial3.find("OK"))
      q=8;
    }
    delay(timeOut);
    if(q>5)
    break;
    q++;
  }
  if(q==8)
  Serial.println("OK");
  else
  Serial.println("Error");
}
//----------------------------------------------------------------------------------------------------------------

void wifi_init() //send AT commands to module
{
  establishConnection("AT",100);
  delay(1000);
  establishConnection("AT+CWMODE=3",100);
  delay(1000);
  establishConnection("AT+CWQAP",100); 
  delay(1000); 
  establishConnection("AT+RST",5000);
  delay(1000);
  findIp(5000);
  if(!No_IP)
  {
    Serial.println("Connecting Wifi....");
    establishConnection("AT+CWJAP=\"MPR\",\"mpr@1234\"",7000); //provide your WiFi username and password here
  }
  else
  {
    
  }
  Serial.println("Wifi Connected"); 
  showIP();
  establishConnection("AT+CIPMUX=1",100);
  establishConnection("AT+CIPSERVER=1,80",100);
}
//----------------------------------------------------------------------------------------------------------------

void sendData(String server1)//send data to module
{
  int p=0;
  while(1)
  {
    unsigned int l=server1.length();
    Serial.print("AT+CIPSEND=0,");
    Serial3.print("AT+CIPSEND=0,");
    Serial.println(l+2);
    Serial3.println(l+2);
    delay(100);
    Serial.println(server1);
    Serial3.println(server1);
    while(Serial3.available())
    {
      //Serial.print(Serial.read());
      if(Serial3.find("OK"))
      {
        p=11;
        break;
      }
    }
    if(p==11)
    break;
    delay(100);
  }
}
//----------------------------------------------------------------------------------------------------------------

void sendToServer()//send data to webpage
{
  server = "<h1>Multipurpose Robot</h1>";  
  server+="<p>================================================================================</p>";  
  server+="<p>Temperature (Deg.C): " + String(DHT_Temp)+ "<p>";
  server+="<p>Humidity (%): " + String(DHT_Humidity)+ "<p>";
  server+="<p>Smoke reading: " + String(Smoke_Reading)+ "<p>"; 
  server+="<p>Vibration status: " + String(Vibration_Status)+ "<p>";
  server+="<p>Light status: " + String(Light_Status)+ "<p>";
  server+="<p>Obstacle distance (cm): " + String(Obstacle_Distance)+ "<p>";
  server+="<p>GPS (Latitude,Longitude): " + String(Latitude_Status)+","+ String(Longitude_Status)+"<p>";
  server+="<p>================================================================================</p>";  
  sendData(server);
  delay(1000);
  Serial3.println("AT+CIPCLOSE=0"); 
}
//----------------------------------------------------------------------------------------------------------------
