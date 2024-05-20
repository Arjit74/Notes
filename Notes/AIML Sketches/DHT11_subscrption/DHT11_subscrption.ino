#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#define ACCEPT_TYPE "text/csv"
WiFiClient client;
int Led_Control=2;

#include "DHT.h"
#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "Le_mauj_kaat";  
const char* password = "aesikitaisi";
const char* host="thingworx.scetngp.com";
const int httpPort=80;

const char appKey[]= "4a0934b4-6c96-44c8-a302-1d3010462e05";
const char Thing[] = "sub_thing";
const char Property1[] = "Temperature";
const char Property2[] = "Humidity";
const char get_Property[] = "light";

String Get(String get_Thing, String get_Property)
{              
        HTTPClient http;
        int httpCode = -1;
        String fullRequestURL = "http://"+String(host)+":"+String(httpPort)+"/Thingworx/Things/"
                                +get_Thing+"/Properties/"+get_Property +"?appKey="+appKey;
                               
        Serial.println(fullRequestURL);
        http.begin(client, fullRequestURL);
        http.addHeader("Accept",ACCEPT_TYPE,false,false);        
        httpCode = http.GET();
        Serial.println(httpCode);
       
        String responses;
        if(httpCode > 0)        
        {            
           responses= http.getString();
             Serial.println(responses);          
        }
        else        
        {
            Serial.printf("[httpGetPropertry] failed, error: %s\n\n", http.errorToString(httpCode).c_str());
        }
        http.end();
        return responses;        
}

void setup()
{
  dht.begin();
  pinMode(Led_Control,OUTPUT);
  Serial.begin(115200);
  Serial.println();
  Serial.print("connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
{
    delay(500);
    Serial.print(".");
}
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
void Put(String ThingName, String ThingProperty, float Value)
  {
    Serial.println(host);
  if (!client.connect(host, httpPort))
{
    Serial.println("connection failed");
    return;
  } else
{
    Serial.println("Connected to ThingWorx.");
  }
  String url = "/Thingworx/Things/"+ThingName+"/Properties/"+ThingProperty;
  Serial.print("requesting URL: ");
  Serial.println(url);
  String strPUTReqVal = "{\""+ThingProperty+"\":\""+Value+"\"}";  
  Serial.print("PUT Value: ");
  Serial.println(strPUTReqVal);
  client.print(String("PUT ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "appKey:"+ appKey + "\r\n" +
               "x-thingworx-session: false" + "\r\n" +
               "Accept: application/json" + "\r\n" +
               "Connection: close" + "\r\n" +
               "Content-Type: application/json" + "\r\n" +
               "Content-Length: " + String(strPUTReqVal.length()) + "\r\n\r\n" +
               strPUTReqVal + "\r\n\r\n");  
while (client.connected())
{
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  client.stop();
}
void loop()
{
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    Put(Thing,Property1,h);
    delay(1000) ;
    Put(Thing,Property2,t);
    delay(1000) ;
    String res = Get(Thing,get_Property);  
 //Serial.println(res); // "Switch":"1.0"/r/n
 Serial.println(res[11]);

  if(res[11] == 't')
  {
    digitalWrite(2,HIGH);
    Serial.println("relay on");
  }
  else if(res[11] == 'f')
  {
    digitalWrite(2,LOW);
    Serial.println("relay off");
  }
  delay(2000);
}
