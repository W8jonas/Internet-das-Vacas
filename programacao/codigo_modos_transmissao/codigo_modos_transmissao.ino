
/*
 * Instituto Federal de Educação, Ciência e Tecnologia Minas Gerais
 * IFMG - Campus Avançado Conselheiro Lafaiete 
 * 
 *  Internet das Vacas código 4
 *  Autor: Jonas Henrique Nascimento
 *  PIBIC-Junior
 * 
 *  Data de início: 11/06/2018
 *  Data da ultima atualização: 22/07/2018
 *  Data de término: 17/06/2018
 *  
 *  Tem o objetivo de estabelecer modelos de transmissão de dados via WiFi.
 *  Cada modelo foi alterado separadamente, sendo necessário o re-upload do
 *  código ao ESP. Cada modo possui vantagens e desvantagens, sendo essas 
 *  incorporadas em sua velocidade de transmissão, potência de alcance ou
 *  consumo de energia.
 *  O código tem a especial necessidade de aferição do consumo energético 
 *  para a futura comparação entre eles. Sendo assim, não foi preocupado 
 *  os dados que seriam transmitidos. A distância entre os nós de toda a 
 *  rede permaneceram constantes durante todos os experimentos.
 *  
 *  Inicialmente, foram testados o modo 802.11b, e seus respectivos valores
 *  de potência de sinal.
 *  Após, testado o 802.11g, e novamente, com seus respectivos valores de 
 *  potência, +20.5 dBm, +18.5 dBm, + 16 dBm e +14 dBm.
 *  Por fim, foi testado o modelo 802.11n. Também com os mesmos valores de
 *  potência de saída.
 * 
 *  Este código está disponível sempre no endereço abaixo, para livre aperfeiçoamento. 
 *  Todavia, pede-se por educação, que ao compartilharem o código, mantenham os autores
 *  originais, tão bem quanto o nome da instituição.
*/


#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

extern "C" {
  #include "user_interface.h"
}

const char* ssid = "esp8266";
const char* password = "1234567890";

ESP8266WebServer server(80);

void handleRoot() {

  String textoHTML;

  textoHTML = "Ola!! Aqui &eacute; o <b>ESP8266</b> falando! ";
  textoHTML += "Porta A0: ";
  textoHTML += analogRead(A0);
   
  server.send(200, "text/html", textoHTML);
}

void handleNotFound(){
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void setup(void){
  Serial.begin(115200);
  
  WiFi.setPhyMode(WIFI_PHY_MODE_11N);
  WiFi.setOutputPower(14);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

//wifi_set_user_fixed_rate;
//wifi_set_user_sup_rate;
//wifi_set_user_rate_limit;

   wifi_set_user_fixed_rate(1, 54);

  Serial.println("");
  int contador = 0;
  while (WiFi.status() != WL_CONNECTED) {
    contador ++;
    delay(50);
    Serial.print("tentativa numero: ");
    Serial.println(contador);
  }
  Serial.println("");
  Serial.print("Conectando em: ");
  Serial.println(ssid);
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);

  server.on("/inline", [](){
    server.send(200, "text/plain", "this works as well");
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("Servidor iniciado");
}

void loop(void){
  server.handleClient();
}

