#include<ESP8266WiFi.h>
#include<ESP8266WebServer.h>

const char* ssid ="NODE";
const char* password ="123456";

ESP8266WebServer server(80);

bool Light1status= LOW;
bool Light2status= LOW;
bool Fanstatus= LOW;
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
pinMode(D0,OUTPUT);
pinMode(D1,OUTPUT);
pinMode(D2,OUTPUT);
WiFi.softAP(ssid,password);
IPAddress myIP= WiFi.softAPIP();
Serial.println("Access point IP:");
Serial.println(myIP);

server.on("/",handle_Onconnect);
server.on("/l1on", handle_light1on);
server.on("/l1off",handle_light1off);

server.on("/l2on", handle_light2on);
server.on("/l2off",handle_light2off);

//server.on("/fanon", handle_fanon);
//server.on("/fanoff",handle_fanoff);
server.onNotFound(handle_NotFound);

server.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();

  if(Light1status)
  digitalWrite(D0,HIGH);
  else
  digitalWrite(D0,LOW);

  if(Light2status)
  digitalWrite(D1,HIGH);
  else
  digitalWrite(D1,LOW);  

}

void handle_Onconnect(){
  Light1status=LOW;
  Light2status=LOW;
  Serial.println("Light1:OFF");
  Serial.println("Light2:OFF");  
  server.send(200,"text/html", updateWebpage(Light1status));
  server.send(200,"text/html", updateWebpage(Light2status));
}

void handle_light1on(){
  Light1status= HIGH;
  Serial.println("Light1:ON");
  server.send(200,"text/html", updateWebpage(Light1status));  
}

void handle_light1off(){
  Light1status= LOW;
  Serial.println("LED:OFF");
  server.send(200,"text/html", updateWebpage(Light1status)); 
}

void handle_light2on(){
  Light2status= HIGH;
  Serial.println("Light2:ON");
  server.send(200,"text/html", updateWebpage(Light2status));  
}

void handle_light2off(){
  Light2status= LOW;
  Serial.println("Light2:OFF");
  server.send(200,"text/html", updateWebpage(Light2status)); 
}





void handle_NotFound(){
  server.send(404,"text/plain","Not Found");
}

  String updateWebpage(uint8_t LEDstatus){
  String ptr= "<!DOCTYPE html> <html>\n";
  ptr += "<head><meta name= \"viewport\"content =\" width=device-width, initial-scale=1.0,user-scalable=no\">\n";
  ptr += "<title>Home Appliances Control</title>\n";
  ptr += "<style> html{font-family: Helvetica; display: inline-block; margin 0px auto; text-align: center;}\n";
  ptr += "body{background-color: yellow;  margin-top: 50px;} h1{color: #444444; margin 50px auto 30px;} h3{color:#444444; margin-bottom:50px;}\n";
  ptr += ".button{display:block; width:80px; background-color:#1abc9c; border:none; color:white; padding: 13px 30px;text-decoration:none; font-size:25px; margin:0px auto 35px; cursor:pointer;border-radius:4px;}\n";
  ptr += ".button-on{background-color: #3498db;}\n";
  ptr += ".button-on:active{background-color: #3498db;}\n";
  ptr += ".button-off{background-color: #34495e;}\n";
  ptr += ".button-off:active{background-color: #2c3e50;}\n";
  ptr += "p{font-size:14px;color:#888; margin-bottom:10px;}\n";
  ptr += "</style>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";
  ptr += "<h1> Home Automation</h1>\n";
  
  if(Light1status){
    ptr += "<a class=\"button button-off\"href=\"/l1off\">LED OFF</a>\n";
  }
  else{
    ptr += "<a class=\"button button-on\"href=\"/l1on\">LED ON</a>\n";
  }

  if(Light2status){
     ptr += "<a class=\"button button-off\"href=\"/l2off\">Night Bulb OFF</a>\n";
  }
  else{
    ptr += "<a class=\"button button-on\"href=\"/l2on\">Night Bulb ON</a>\n";
  }

  ptr += "</body>\n";
  ptr += "</html>\n";
  return ptr;
}
