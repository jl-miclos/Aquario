#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include<LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>

IPAddress ip(192, 168, 0, 100);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress dns(192, 168, 0, 1);

const char* ssid = "Nome da rede  "; //Nome da rede 
const char* password = "Senha da rede "; //Senha da rede
WiFiServer server(80); 

LiquidCrystal_I2C lcd(0x27,16,2); 
OneWire onewire(0); // GPIO where the DS18B20 is connected to
DallasTemperature TemperaturaIdeal(&onewire);
float temperatura;

void setup() {
Wire.begin(4, 5); //SDA = 4; SCL = 5.
lcd.init();
TemperaturaIdeal.begin();
Serial.begin(115200);
delay(10);
Serial.println(); // Connect to WiFi network
Serial.println();
Serial.print("Conectando ");
Serial.println(ssid);

WiFi.config(ip, dns, gateway, subnet);

WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}
Serial.println("");
Serial.println("WiFi conectado");
Serial.println(WiFi.localIP());
server.begin();
}

void loop() {
   
TemperaturaIdeal.requestTemperatures(); 
temperatura = TemperaturaIdeal.getTempCByIndex(0);
lcd.setBacklight(HIGH);
lcd.setCursor(0,0);
lcd.print("  Temperatura:");
delay(100);
lcd.setCursor(5,1);
lcd.print(temperatura);
delay(100);
 
WiFiClient client = server.available();  // Check if a client has connected
if (!client) { //Se o cliente não se conectar, voltar até ele se conectar
return; 
}
while(!client.available()){
delay(1);
}
String req = client.readStringUntil('\r');
client.flush();

if (req.indexOf("FT1") >= 0) {  
 if(temperatura> 23) {
 Serial.println("LC");
 }
 else if(temperatura< 21) {
 Serial.println("LA");
 }
 else if(temperatura>= 21 && temperatura<= 23){
 Serial.println("LN");
}}  
 
if (req.indexOf("FT2") >= 0) {  
 if(temperatura> 25) {
 Serial.println("LC");
 }
 else if(temperatura< 23) {
 Serial.println("LA");
 }
 else if(temperatura>= 23 && temperatura<= 25){
 Serial.println("LN");
}}
 
if (req.indexOf("FT3") >= 0) {
 if(temperatura> 27) {
 Serial.println("LC");
 }
 else if(temperatura< 25) {
 Serial.println("LA");
 }
 else if(temperatura>= 25 && temperatura<= 27){
 Serial.println("LN");
}}
 
if (req.indexOf("FT4") >= 0) {   
 if(temperatura> 29) {
 Serial.println("LC");
 }
 else if(temperatura< 27) {
 Serial.println("LA");
}
 else if(temperatura>= 27 && temperatura<= 29){
 Serial.println("LN");
}}
 
if (req.indexOf("FT5") >= 0) {    
 if(temperatura> 31) {
 Serial.println("LC");
 }
 else if(temperatura< 29) {
 Serial.println("LA");
}
 else if(temperatura>= 29 && temperatura<= 31){
 Serial.println("LN");
}}

if (req.indexOf("FL") >=0) {  //FITA LED
Serial.println("IL");
}
  
if (req.indexOf("PA") >=0) {  //ALIMENTADOR
Serial.println("IA");
}

if (req.indexOf("TA") >=0) {  //TPA
Serial.println("ITPA");
}
client.flush();
delay(1);
}
