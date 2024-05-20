#include <ESP8266WiFi.h>
//#include <ESP8266HTTPClient.h>
//#include <WiFiClient.h>
//#define ACCEPT_TYPE "text/csv"
WiFiClient client;

#include "DHT.h"
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "Le_mauj_kaat";  
const char* password = "aesikitaisi";
const char* host="thingworx.scetngp.com";  
const int Port=80;

const char appKey[]= "4a0934b4-6c96-44c8-a302-1d3010462e05";
const char Thing[] = "Arjit_Thing";
const char Property1[] = "thing1";
const char Property2[] = "thing2";

void setup()
{
  dht.begin();
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
  if (!client.connect(host, Port))
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
}
