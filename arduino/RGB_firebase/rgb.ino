#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>

const char* ssid = "Robert iphone";
const char* password = "0926656000";
const char* host="iostest-64ed7.firebaseio.com";
const int httpsPort = 443;
#define D2 4
String section;

void setup()
{
  pinMode(D2,OUTPUT);
  Serial.begin(115200);
  Serial.println();
  //wifi連線
  WiFi.begin("Robert iphone", "0926656000");
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if(WiFi.status() == WL_CONNECTED){
    Serial.println("wifi connected");
    WiFiClientSecure client;
    //連線到firebase
    client.connect(host,httpsPort);
    Serial.print("connecting to");
    Serial.println(host);
    while(client.connected() == false){
      Serial.println("not connect to host");
    }
    Serial.println("connect to host");
    
    String url = "/RGB.json";
    client.print(
      "GET " + url + " HTTP/1.1\r\n" +
      "Host: " + host + "\r\n" +      
      "Connection: close\r\n\r\n"  
    );
    Serial.println("request send");
    while(client.available() == 0){
      Serial.print("wait data. ");
    }

    Serial.println("------------------------data--------------------");
    section = "header";
    while(client.available()){
      String line = client.readStringUntil('\r');
      parseJSON(line);
    }
    Serial.println();
    Serial.println("============================");
    Serial.println("closing connection");
  }
  delay(100);
}

void parseJSON(String line){
    if(section == "header"){
      //Serial.print(".");
      Serial.print(line);
      if(line == "\n"){
        section = "json";
      }
      return;
    }

    Serial.println(line);
    String result = line.substring(1);
    int size = result.length() + 1;
    char json[size];
    result.toCharArray(json,size);
    StaticJsonBuffer<200> jsonBuffer;
    JsonObject& json_parsed = jsonBuffer.parseObject(json);
    if(!json_parsed.success()){
      Serial.println();
      Serial.println("parseObject() failed");
      return;
    }

    int Rvalue = int(json_parsed["R"]);
    int Gvalue = int(json_parsed["G"]);
    int Bvalue = int(json_parsed["B"]);
    Serial.println(Rvalue);
    Serial.println(Gvalue);
    Serial.println(Bvalue);
    
}