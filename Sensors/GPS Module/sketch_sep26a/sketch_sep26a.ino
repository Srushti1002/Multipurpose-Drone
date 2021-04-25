
#include <TinyGPS.h> 

float lat = 28.5458,lon = 77.1703; // create variable for latitude and longitude object  
TinyGPS gps; // create gps object 

void setup()
{ 
Serial.begin(9600); // connect serial 
Serial.println("The GPS Received Signal:"); 
Serial2.begin(9600); // connect gps sensor 

} 
void loop(){ 
  while(Serial2.available()){ // check for gps data 
  if(gps.encode(Serial2.read()))// encode gps data 
  {  
  gps.f_get_position(&lat,&lon); // get latitude and longitude 
  // display position 
  
  Serial.print("Position: "); 
  Serial.print("Latitude:"); 
  Serial.print(lat,6); 
  Serial.print(";"); 
  Serial.print("Longitude:"); 
  Serial.println(lon,6);  
 
  Serial.print(lat); 
  Serial.print(" "); 
  
 } 
} 
String latitude = String(lat,6); 
  String longitude = String(lon,6); 
Serial.println(latitude+";"+longitude); 
delay(1000); 
} 
