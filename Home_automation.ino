#include <ESP8266WiFi.h>
#include<Wire.h>

const char* ssid = "SSID";//type your ssid
const char* password = "PASSWORD";//type your password


WiFiServer ESPserver(80);//Service Port

void setup() 
{
pinMode(0,OUTPUT);
pinMode(2,OUTPUT);
Wire.begin(); // creats Wire
Wire.beginTransmission(0x20); 
Wire.write(0x00); // select adress of IODIRA reg
Wire.write(0x00); // set port as o/p
Wire.endTransmission();  
Serial.begin(115200);

Serial.println();
Serial.println();
Serial.print("Connecting to: ");
Serial.println(ssid);

WiFi.begin(ssid, password);
delay(5000);
// assign static IP
IPAddress ip(192,168,1,254);   
IPAddress gateway(192,168,1,1);   
IPAddress subnet(255,255,255,0);   
WiFi.config(ip, gateway, subnet);
delay(5000);

while (WiFi.status() != WL_CONNECTED) 
{
delay(100);
Serial.print("*");
}
Serial.println("");
Serial.println("WiFi connected");

// Start the server
ESPserver.begin();
Serial.println("Server started");

// Print the IP address
Serial.print("The URL to control ESP8266: ");
Serial.print("http://");
Serial.print(WiFi.localIP());
}

void loop() 
{
Wire.beginTransmission(0x20); //starts talking to slave device
Wire.write(0x09); //selects the GPIO pins
// Check if a client has connected
WiFiClient client = ESPserver.available();
if (!client) 
{
return;
}

// Wait until the client sends some data
Serial.println("New Client");
while(!client.available())
{
delay(1);
}

// Read the first line of the request
String request = client.readStringUntil('\r');
Serial.println(request);
client.flush();

// Match the request

int value = LOW;
if (request.indexOf("/lamp1on") != -1) 
{
Serial.println("LAMP 1 is ON");
Wire.write(00000001);
value = HIGH;
} 
if (request.indexOf("/lamp1off") != -1)
{
Serial.println("LAMP 1 is OFF");
Wire.write(00000000);
value = LOW;
}
if (request.indexOf("/lamp2on") != -1) 
{
Serial.println("LAMP 2 is ON");
Wire.write(00000010);
value = HIGH;
} 
if (request.indexOf("/lamp2off") != -1)
{
Serial.println("LAMP 2 is OFF");
Wire.write(00000000);
value = LOW;
}
if (request.indexOf("/fanon") != -1) 
{
Serial.println("Fan is ON");
Wire.write(00000100);
value = HIGH;
} 
if (request.indexOf("/fanoff") != -1)
{
Serial.println("Fan is OFF");
Wire.write(00000100);
value = LOW;
}

// Return the response in HTML page
client.println("HTTP/1.1 200 OK");
client.println("Content-Type: text/html");
client.println(""); 
client.println("<!DOCTYPE HTML>");
client.println("<html>");

client.print("Status of the Lamp: ");

if(value == HIGH) 
{
client.print("ON");  
} 
else 
{
client.print("OFF");
}
Wire.endTransmission(); 
delay(1);
client.stop();
Serial.println("Client disconnected");
Serial.println("");
}
