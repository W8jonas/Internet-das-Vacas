
/*
 * Instituto Federal de Educação, Ciência e Tecnologia Minas Gerais
 * IFMG - Campus Avançado Conselheiro Lafaiete 
 * 
 *  Internet das Vacas código 3
 *  Autor: Jonas Henrique Nascimento
 *  PIBIC-Junior
 * 
 *  Data de início: 13/08/2018
 *  Data da ultima atualização: 15/09/2018
 *  Data de término: 02/09/2018
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
 *  https://github.com/W8jonas/Internet-das-Vacas/blob/master/programacao/codigo_teste_de_consumo/codigo_teste_de_consumo.ino
 *  
 */


#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#define tempo_para_desligar 250000
#define tempo_modo_funcionamento 200000
#define tempo_modo_funcionamento2 250000

#define Pino_transmit D8

extern "C" {
  #include "user_interface.h"
}

void modo_DEEP_SLEEP();
void handleNotFound();
void handleRoot();
void MAX_CPU();

const char* ssid = "82W8JH";
const char* password = "w8989jonas";

ESP8266WebServer server(80);

void setup() {
  
   Serial.begin(115200);
   Serial.println("Inicializando o setup");
   WiFi.setPhyMode(WIFI_PHY_MODE_11N);
   WiFi.setOutputPower(14);
   WiFi.mode(WIFI_STA);
   WiFi.begin(ssid, password);
   wifi_set_user_fixed_rate(1, 54);
   int contador = 0;

   pinMode(Pino_transmit, OUTPUT);
   digitalWrite(Pino_transmit, HIGH);
   
   while (WiFi.status() != WL_CONNECTED) {
      contador ++;
      delay(50);
   }

   server.on("/", handleRoot);
   server.on("/inline", [] () { server.send(200, "text/plain", "this works as well"); } );

   server.onNotFound(handleNotFound);
   server.begin();
   delay(500);
   
   Serial.println("");
   Serial.print("Conectando em: ");
   Serial.println(ssid);
   Serial.print("IP: ");
   Serial.println(WiFi.localIP());
}


void loop() {
   unsigned long valor_atual_contador = millis();
   Serial.println(valor_atual_contador);
   digitalWrite(Pino_transmit, LOW);
   if (valor_atual_contador < tempo_modo_funcionamento) {
      MAX_CPU();
      digitalWrite(Pino_transmit, LOW);
   }
   
   if ((valor_atual_contador >= tempo_modo_funcionamento) && (valor_atual_contador < tempo_modo_funcionamento2)) {
      server.handleClient();
      handleRoot();
   }
   
   if (valor_atual_contador >= tempo_para_desligar) {
      modo_DEEP_SLEEP();
   }
   
}


void MAX_CPU() {
   Serial.println("MAX_CPU");
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
   Serial.println("HandleRoot");
   String textoHTML;
   textoHTML = " PARA DE ME COPIAR DANIELLE ";
   server.send(200, "text/html", textoHTML);
}


void handleNotFound(){
   Serial.println("handleNotFound");
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
   digitalWrite(Pino_transmit, LOW);
   digitalWrite(Pino_transmit, HIGH);
   delay(800);
   digitalWrite(Pino_transmit, LOW);
   Serial.println("Deep-Sleep");
   delay(500);
   ESP.deepSleep(100000000 , WAKE_RF_DEFAULT);
   delay(500);
}




