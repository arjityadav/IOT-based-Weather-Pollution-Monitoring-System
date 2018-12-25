#include <dht.h>
#include <DHT.h>
#include "DHT.h"
#define dht_apin A3 
dht DHT;
int c;
const int sensorMin = 0;     // sensor minimum
const int sensorMax = 1024;  // sensor maximum
#include <SoftwareSerial.h>
#define RX 10
#define TX 11
String AP = "wifi";       // CHANGE ME
String PASS = "Reddevil"; // CHANGE ME
String API = "L5FK848QNKBZPDXM";   // CHANGE ME
String HOST = "api.thingspeak.com";
String PORT = "80";
String field = "field2";
String field3 = "field3";
String field4 = "field4";
String field5 = "field5";
int countTimeCommand; 
int countTrueCommand;
boolean found = false; 
int valSensor = 1;
int valSensor1 = 1;
int valSensor2 = 1;
int valSensor3 = 1;
SoftwareSerial esp8266(RX,TX); 
 
void setup() {
  Serial.begin(9600);
  esp8266.begin(115200);
  sendCommand("AT",5,"OK");
  sendCommand("AT+CWMODE=1",5,"OK");
  sendCommand("AT+CWJAP=\""+ AP +"\",\""+ PASS +"\"",20,"OK");
}
void loop() {
 valSensor = getSensorData();
 valSensor1=getSensorData1();
 valSensor2=getSensorData2();
 valSensor3=getSensorData3();
 String getData = "GET /update?api_key="+ API +"&"+ field +"="+String(valSensor)+"&"+ field3 +"="+String(valSensor1)+"&"+ field4 +"="+String(valSensor2)+"&"+ field5 +"="+String(valSensor3);
sendCommand("AT+CIPMUX=1",5,"OK");
 sendCommand("AT+CIPSTART=0,\"TCP\",\""+ HOST +"\","+ PORT,15,"OK");
 sendCommand("AT+CIPSEND=0," +String(getData.length()+4),4,">");
 esp8266.println(getData);delay(1500);countTrueCommand++;
 sendCommand("AT+CIPCLOSE=0",5,"OK");
}
int getSensorData(){
  int val;
  val = analogRead(1); //Read Gas value from analog 1
  Serial.println("Carbon Value is:");
  Serial.println(val, DEC); //Print the value to serial port
  return (val); // Replace with 
}
int getSensorData1(){
  unsigned int AnalogValue;
  AnalogValue = analogRead(0);//Read Light value from analog 0
  Serial.println(AnalogValue);
  return (AnalogValue); // Replace with 
}
int getSensorData2(){
  int sensorReading = analogRead(2);//Read Rain value from analog 2
  int range = map(sensorReading, sensorMin, sensorMax, 0, 3);
  switch (range) {
    case 0:    // Sensor getting wet
      Serial.println("Flood");
      break;
    case 1:    // Sensor getting wet
      Serial.println("Rain Warning");
      break;
    case 2:    // Sensor dry - To shut this up delete the " Serial.println("Not Raining"); " below.
      Serial.println("Not Raining");
      break;
  }
return (range);
}
int getSensorData3(){
  DHT.read11(dht_apin);
c=DHT.temperature;
  Serial.print("Current humidity = ");
  Serial.print(DHT.humidity);
  Serial.print("%  ");
  Serial.print("temperature = ");
  Serial.print(c);
  Serial.println("C  ");
  delay(5000);
  return(c);
}
void sendCommand(String command, int maxTime, char readReplay[]) {
  Serial.print(countTrueCommand);
  Serial.print(". at command => ");
  Serial.print(command);
  Serial.print(" ");
  while(countTimeCommand < (maxTime*1))
  {
    esp8266.println(command);//at+cipsend
    if(esp8266.find(readReplay))//ok
    {
      found = true;
      break;
    }
  
    countTimeCommand++;
  }
  
  if(found == true)
  {
    Serial.println("OYI");
    countTrueCommand++;
    countTimeCommand = 0;
  }
  
  if(found == false)
  {
    Serial.println("Fail");
    countTrueCommand = 0;
    countTimeCommand = 0;
  }
  
  found = false;
 }
