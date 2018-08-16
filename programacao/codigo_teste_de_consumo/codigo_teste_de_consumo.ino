
/*
 * Instituto Federal de Educação, Ciência e Tecnologia Minas Gerais
 * IFMG - Campus Avançado Conselheiro Lafaiete 
 * 
 *  Internet das Vacas código 3
 *  Autor: Jonas Henrique Nascimento
 *  PIBIC-Junior
 * 
 *  Data de início: 13/08/2018
 *  Data da ultima atualização: 13/08/2018
 *  Data de término: XX/XX/2018
 *  
 *  Tem o objetivo de estabelecer modelos de operação de economia de energia.
 *  Sendo, no total, 6 funções. A primeira, abilita o ESP em modo standby, a 
 *  qual nao realiza nenhuma função. Já a segunda, abilita o ESP a trabalhar
 *  em modo client e server ao mesmo tempo. De modo semelhante, a função 3
 *  abilita o ESP a trabalhar somente em modo client. A partir da modo_light_sleep
 *  o ESP recebe sua configuração voltada realmente a economia de energia. 
 *  Sendo o primeiro modelo, o light sleep com a cpu ainda ativa para ser 
 *  inicializada rapidamente. Em contrapartida a próxima função realiza
 *  o light sleep, todavia com a dpu desativada, sendo a unica maneira de
 *  retornar ao estado de funcionamento do ESP por uma interrupção externa.
 *  Por fim, o modelo mais economico, o Deep sleep, a qual deixa o ESP quase
 *  que inteiramente desligado por um período de tempo definido pelo programador.
 *  Toda via, esse modelo requer uma pequena modificação na placa, a qual deverá
 *  constar um jump que interlique a GPIO D0 ao pino Reset do ESP.
 * 
 *  Este código está disponível sempre no endereço abaixo, para livre aperfeiçoamento. 
 *  Todavia, pede-se por educação, que ao compartilharem o código, mantenham os autores
 *  originais, tão bem quanto o nome da instituição.
 *  
*/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#define tempo_para_desligar 30000
#define tempo_modo_funcionamento 15000
#define tempo_modo_funcionamento2 30000

extern "C" {
  #include "user_interface.h"
}


void modo_DEEP_SLEEP();
void handleNotFound();
void handleRoot();
void MAX_CPU();

const char* ssid = "esp8266";
const char* password = "123";

ESP8266WebServer server(80);

void setup() {
   Serial.println("Inicializando o setup");
   WiFi.setPhyMode(WIFI_PHY_MODE_11N);
   WiFi.setOutputPower(14);
   WiFi.mode(WIFI_STA);
   WiFi.begin(ssid, password);
   wifi_set_user_fixed_rate(1, 54);
   int contador = 0;
   
//   while (WiFi.status() != WL_CONNECTED) {
//      contador ++;
//      delay(50);
//   }

   server.on("/", handleRoot);
   server.on("/inline", [](){
      server.send(200, "text/plain", "this works as well");
   });

   server.onNotFound(handleNotFound);
   server.begin();
   delay(500);
}


void loop() {
   unsigned long valor_atual_contador = millis();
   
   if (valor_atual_contador < tempo_modo_funcionamento) {
      MAX_CPU();
   }
   
   if ((valor_atual_contador >= tempo_modo_funcionamento) && (valor_atual_contador < tempo_modo_funcionamento2)) {
      server.handleClient();
   }
   
   if (valor_atual_contador >= tempo_para_desligar) {
      modo_DEEP_SLEEP();
   }
   
}


void MAX_CPU() {
   int cont = 1;
   float resp = 1;
   float resp2 = 1;
   for(int AA = 0; AA < 500; AA++){
      resp = 3 + sin(resp)/cos(resp*resp/2) * sqrt(sqrt(resp*resp));
      resp = resp * 0.5;
      resp2 = sqrt(AA);
      yield();
  }
}


void handleRoot() {
   String textoHTML;
   textoHTML = "TESTE DE ENVIO ### Desconsiderar mensagem ###";
   server.send(200, "text/html", textoHTML);
}


void handleNotFound(){
   String message = "Sem arquivo\n\n";
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


void modo_DEEP_SLEEP() {
   delay(500);
   ESP.deepSleep(40000000 , WAKE_RF_DEFAULT);
   delay(500);
}




