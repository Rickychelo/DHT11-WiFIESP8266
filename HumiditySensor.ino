gra//Librerías
#include <ESP8266WiFi.h>
#include <DHT.h>

//Variables 
#define DHTTYPE DHT11 //Tipo de sensor de humedad
#define DHTPIN 14 //GPIO2 - Pin donde se va a conectar

//Threshold ESP8266 (10 - 30)
DHT dht(DHTPIN, DHTTYPE, 27);


String ssid = "Claro_MARIALUZURI";
String password = "1103048649";
WiFiServer server(80); //Puerto 80


float temp, hume;
int pause = 800;
unsigned long start_time;

//Convertir una dirección IP a un String
String IpAddress2String(const IPAddress& ipAddress){
  return String(ipAddress [0] ) + String(".") +\
  String (ipAddress[1]) + String(".") +\
  String (ipAddress[2]) + String(".") +\
  String (ipAddress[3])  ;
}


//Leer la temperatura y de la humedad
void leerTemperatura(){
  hume = dht.readHumidity();
  temp = dht.readTemperature();
  Serial.println("Temperatura = "+String(temp)+"°C, Humedad = "+ String(hume)+"%");
}


void setup() {
  Serial.begin(115200);
  dht.begin();
  start_time = millis();

  //Conexión WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED){
    delay (500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("********************************************************");
  Serial.print("Conectado a la Red WiFi: ");
  Serial.print(WiFi.SSID());
  Serial.println("");
  Serial.print("IP: ");
  Serial.print(WiFi.localIP());
  Serial.println("");
  Serial.print("MAC ADDRESS: ");
  Serial.print(WiFi.macAddress());
  Serial.println("");
  Serial.println("********************************************************");

  server.begin();
}

void loop() {
  if (millis() - start_time >= pause){
    leerTemperatura();
    start_time = millis(); 
  }
  WiFiClient client = server.available (); //objeto de la clase WiFiClient
  if(!client){
     return;
  }
  Serial.println("Nuevo cliente...");
  while (!client. available ()){ //espera a un cliente diponible
     delay (1);
  }
  String peticion = client.readStringUntil('\r'); //lee la peticion del cliente
  //Serial.println (peticion);
  client.flush (); //limpia la peticion del cliente
  client.println("HTTP/1.1 200 OK");
  client.println("");
  client.println("");
  client.println("");
  client.println("");
  //Inicio de la página web
  client.println ("<!DOCTYPE html><html lang='en'><head><meta charset='UTF-8'>");
  client.println ("<meta name='viewport' content='width=device-width, initial-scale=1.0'>");
  client.println ("<title>Temperatura y Humedad</title></head>");
  client.println("<body style='font-family: Century gothic; width: 800;'><center>");
  client.println("<div style='box-shadow: 0px 0px 20px 8px rgba (0,0,0,0.22); padding: 20px; width: 300px; display: inline-block: margin: 30px:'>");
  
  client.println("<h2>Temperatura = " + String(temp) + " °C</h2>");
  client.println("<h2>Humedad = " + String(hume) + " %<h2>");
  
  client.println("</div></center></body></html>");
  client.println("");
  //Fin de la página web
  delay(10);    
  Serial.println("Peticion finalizada");
  

  
}
