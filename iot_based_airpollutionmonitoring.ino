o#include <SoftwareSerial.h>  
   
SoftwareSerial espSerial(2, 3);        
#define DEBUG true
String mySSID = "hotspot_name";      
String myPWD = "hotspot_pwd";
String myAPI = "write_api_thingspeak";  
String myHOST = "api.thingspeak.com";
String myPORT = "80";
String myFIELD = "field1";
int sendVal;


void setup()
{
  Serial.begin(115200);
  espSerial.begin(115200);
 
  espData("AT+RST", 1000, DEBUG);                      
  espData("AT+CWMODE=1", 1000, DEBUG);                
  espData("AT+CWJAP=\""+ mySSID +"\",\""+ myPWD +"\"", 1000, DEBUG);  
 
  delay(1000);
 
}

  void loop()
  {
   
   
    sendVal = analogRead(2);
    String sendData = "GET /update?api_key="+ myAPI +"&"+ myFIELD +"="+String(sendVal);
    espData("AT+CIPMUX=1", 1000, DEBUG);      
    espData("AT+CIPSTART=0,\"TCP\",\""+ myHOST +"\","+ myPORT, 1000, DEBUG);
    espData("AT+CIPSEND=0," +String(sendData.length()+4),1000,DEBUG);  
    espSerial.find(">");
    espSerial.println(sendData);
   
    espData("AT+CIPCLOSE=0",1000,DEBUG);
    Serial.print("Value to be sent: ");
    Serial.println(sendVal);
    delay(1000);
 
    if (sendVal>=0 && sendVal<51){
      Serial.print(" AQI IS GOOD " "\n");}
    else if (sendVal>50 && sendVal<100)
    {
      Serial.print("AQI IS SATISFACTORY" "\n");
      }
    else if (sendVal>50 && sendVal<101)
    {
      Serial.print("AQI IS MODERATE" "\n");
      }
    else if (sendVal>100 && sendVal<251)
    {
      Serial.print("AQI IS POOR" "\n");
      }
    else if (sendVal>250 && sendVal<351)
    {
      Serial.print("AQI IS VERY POOR" "\n");
      }
    else if (sendVal>350 && sendVal<431)
    {
      Serial.print("AQI IS SEVERE" "\n");
      }
     else
    {
      Serial.print("AQI IS" "\n");
      }
  }
  String espData(String command, const int timeout, boolean debug)
{
  Serial.print("AT Command ==> ");
  Serial.print(command);
  Serial.println("     ");
 
  String response = "";
  espSerial.println(command);
  long int time = millis();
  while ( (time + timeout) > millis())
  {
    while (espSerial.available())
    {
      char c = espSerial.read();
      response += c;
    }
  }

  return response;
}