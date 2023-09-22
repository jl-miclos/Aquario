#include <Stepper.h>

unsigned long millisIluminacao = millis();
const unsigned long IntervaloV = 10;
unsigned long tempV= 0;
const unsigned long IntervaloA= 10;
unsigned long tempA= 0;
String msg;
int Aquecedor = 4; //Mudar para relé do aquecedor
int Cooler = 5; //Mudar para relé do cooler
int FV = 2; //Fita de led vermelho
int FA = 3; //Fita de led azul
int SensorSump = 22; //Sensor acima do de seguranca
int SensorSeguranca = 24;  //Sensor mais de baixo do SUMP
int SensorReservatorio = 26; //Sensor do tanque de agua limpa
int BombaSuja = 28; //Bomba que sai do sump para tanque, utiliza so na TPA
int BombaLimpa = 30; //Bomba que sai do tanque para sump, utiliza so na TPA
int BombaSump = 32; //Bomba NF, so abre o rele no durante a TPA
const int stepsPerRevolution = 2050; //Passos em 360
const int stepsPerRevolution2 = 1281; //Passos em 225
const int stepsPerRevolution3 = 1025; //Passos em 180
const int stepsPerRevolution4 = 512; //Passos em 90
const int stepsPerRevolution5 = 256; //Passos em 45
Stepper myStepper(stepsPerRevolution, 6, 8, 7, 9); 

void setup() {
Serial.begin(115200);
Serial3.begin(115200); 
myStepper.setSpeed(4); //Numero de rotações por minuto, SE TIVER LENTO VOCE AUMENTA ATE 60
pinMode(Aquecedor, OUTPUT);
pinMode(Cooler, OUTPUT);
pinMode(FV, OUTPUT);
pinMode(FA, OUTPUT);
pinMode(SensorSump, OUTPUT);
pinMode(SensorSeguranca, OUTPUT);
pinMode(SensorReservatorio, OUTPUT);
pinMode(BombaSuja, OUTPUT);
pinMode(BombaLimpa, OUTPUT);
pinMode(BombaSump, OUTPUT);   
}

void loop() {

if (Serial3.available()) {  //Aguarde dados vindos do ESP8266.
char data = Serial3.read();  //Leitura de um byte.
Serial.print(data);  //Imprima o mesmo dado pela porta usb.
msg += data;  //Acrescente o caractere recebido a string de mensagem.
if (data == 13) {  //Limpa a string ao receber caractere CR(Carriage return).
msg = "";
}  
Iluminacao();

if (msg.indexOf("LC") > 0) { //Receber ordem do ESP8266
digitalWrite(Cooler, HIGH); 
digitalWrite(Aquecedor, LOW); 
}
else if (msg.indexOf("LA") > 0) { //Receber ordem do ESP8266
digitalWrite(Aquecedor, HIGH); 
digitalWrite(Cooler, LOW); 
}
else if(msg.indexOf("LN") > 0) { //Receber ordem do ESP8266
digitalWrite(Aquecedor, LOW);
digitalWrite(Cooler, LOW); 
}

if (msg.indexOf("IA") > 0) { //Receber ordem do ESP8266 
myStepper.step(stepsPerRevolution3);
myStepper.step(stepsPerRevolution5);
myStepper.step(-stepsPerRevolution4);
myStepper.step(stepsPerRevolution2);
}

if (msg.indexOf("ITPA") > 0) { //Receber ordem do ESP8266   
if(digitalRead(SensorSeguranca) == HIGH){ //Se for high, executa todo o processo 
  
if(digitalRead(SensorSump) == HIGH && digitalRead(SensorReservatorio) == HIGH){
digitalWrite(BombaSump, HIGH); 
digitalWrite(BombaSuja, HIGH);
}
else if(digitalRead(SensorSump) == LOW && digitalRead(SensorReservatorio) == HIGH){
digitalWrite(BombaLimpa, HIGH);
digitalWrite(BombaSuja, LOW);
}
else if(digitalRead(SensorSeguranca) == HIGH && digitalRead(SensorReservatorio) == LOW){
digitalWrite(BombaSump, LOW);  
digitalWrite(BombaLimpa, LOW);
}
}
else if(digitalRead(SensorSeguranca) == LOW){ //Se for low, para tudo!(com a intenção de não queimar as bombas)
digitalWrite(BombaSuja, LOW);
digitalWrite(BombaLimpa, LOW);
digitalWrite(BombaSump, LOW);
}}}}


void Iluminacao(){
    
if (msg.indexOf("IL") > 0) { //Receber ordem do ESP8266
int x = 1; 
millisIluminacao = tempV;
if (millisIluminacao- tempV >= IntervaloV)
{
for (int i = 0; i > -1; i = i + x) {
analogWrite(FV, i);
if (i == 255) { //led no maximo
x = -1; }
millisIluminacao = tempV;} //ALTERAR ESSES VALORES
}
int y = 1; 
millisIluminacao = tempA;
if (millisIluminacao - tempA >= IntervaloA)
{
for (int i = 0; i > -1; i = i + y) {
analogWrite(FA, i);
if (i == 255) {
y = -1;
}
} //ALTERAR ESSES VALORES
millisIluminacao = tempA;}
 }
}
