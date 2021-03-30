#include <WiFi.h>
#include <HTTPClient.h>
#define pinTemperatura 34
#define pinPositivo 13
#define pinButton 12
#define pinVermelho 27
#define pinVerde 26
#define pinAzul 25
short int httpResponseCode;
const char* ssid = "";
const char* password =  "";
String url;
String httpRequestData,payload;
float temperatura;
bool button=false,aux=false;
void Mudar() {
  button = !button;
  if (button) aux = !aux;
}
void Conectar(){
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("IP: " + (String)WiFi.localIP());  
  
};
void setup() {
  pinMode(pinTemperatura,INPUT);
  pinMode(pinButton,INPUT);
  pinMode(pinPositivo,OUTPUT);
  pinMode(pinVermelho,OUTPUT);
  pinMode(pinVerde,OUTPUT);
  pinMode(pinAzul,OUTPUT);
  attachInterrupt(digitalPinToInterrupt(pinButton), Mudar, CHANGE);
  Serial.begin(115200);
  Conectar();
}
void Enviar(){
  HTTPClient http;
  http.addHeader("Content-Type", "text/plain");
  url = "http://192.168.15.32/Enviar?Temperatura="+(String)temperatura+"&Senha=admin";//IP NodeJS
  http.begin(url.c_str());
  httpResponseCode = http.POST("");
  delay(300);
  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
  }
  http.end();
}
void Led(){
  if (temperatura > 40) {
      digitalWrite(pinVermelho, 1);
      digitalWrite(pinVerde, 0);
      digitalWrite(pinAzul, 0);
   }
  else if (temperatura > 30) {
    digitalWrite(pinVermelho, 0);
    digitalWrite(pinVerde, 1);
    digitalWrite(pinAzul, 0);
   }
  else if (temperatura > 20) {
    digitalWrite(pinVermelho, 0);
    digitalWrite(pinVerde, 0);
    digitalWrite(pinAzul, 1);
   }
  else(temperatura > 10) {
    digitalWrite(pinVermelho, 1);
    digitalWrite(pinVerde, 1);
    digitalWrite(pinAzul, 1);
   }

}
void Temperatura(){
    temperatura = analogRead(pinTemperatura) / 9.31;
    Serial.println("Temperatura: " + (String)temperatura);  
}
void loop() {
  digitalWrite(pinPositivo,HIGH);
  if(aux){
    Temperatura();
    Enviar();
    Led();
  }
  else{
    Serial.println("Pause");
    digitalWrite(pinVermelho, 0);
    digitalWrite(pinVerde, 0);
    digitalWrite(pinAzul, 0);
   }
  delay(1000);
}
