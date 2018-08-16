
/*
 * Instituto Federal de Educação, Ciência e Tecnologia Minas Gerais
 * IFMG - Campus Avançado Conselheiro Lafaiete 
 * 
 *  Internet das Vacas código 3
 *  Autor: Jonas Henrique Nascimento
 *  PIBIC-Junior
 * 
 *  Data de início: 05/06/2018
 *  Data da ultima atualização: 13/08/2018
 *  Data de término: 10/06/2018
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

extern "C" {
  #include "user_interface.h"
}

#define entrada_botao D6

void modo_standby();
void modo_server_and_client();
void modo_only_client();
void modo_modem_sleep();
void modo_light_sleep();
void modo_light_sleep_CPU_OFF();
void modo_DEEP_SLEEP();

int operacao = 0;
boolean leitura = true;

void setup() {
   pinMode(LED_BUILTIN, OUTPUT); 
   pinMode (entrada_botao, INPUT);
   Serial.begin(115200);
}

void loop() {
  leitura = digitalRead(entrada_botao);
  Serial.println(leitura);
  if (leitura == LOW ){
     operacao++;
     delay(500);
  }
   switch (operacao){
      case 0: 
         modo_standby(); 
         break;  // ESP em modo standby
      case 1: 
         modo_server_and_client(); 
         break;  // ESP em modo server and client
      case 2: 
         modo_only_client(); 
         break;  // Esp em modo only client
      case 3: 
         modo_modem_sleep(); 
         break;  // Esp em modem sleep
      case 4: 
         modo_light_sleep(); 
         break;  // Esp em modo light - sleep com cpu ativa
      case 5: 
         modo_light_sleep_CPU_OFF(); 
         break;  // Esp em light - sleep cpu desligada OBS: Para que se possa chegar no deep sleep é preciso comentar a chamada da modo_light_sleep_CPU_OFF.
      case 6:
         modo_DEEP_SLEEP();
         break;  // Esp em deep sleep
      case 7:
      default:
         operacao=0; 
         break;  // 
   }
}

void modo_standby (){
   Serial.println("ESP em modo standby");
}

void modo_server_and_client() {
   Serial.println("ESP em modo server and client");
   WiFi.mode(WIFI_AP);
}

void modo_only_client() {
   Serial.println("Esp em modo only client");
   WiFi.mode(WIFI_STA);
}

void modo_modem_sleep() {
   Serial.println("Esp em modem sleep");
   WiFi.mode(WIFI_OFF);
}

void modo_light_sleep() {
   Serial.println("Esp em modo forçado de sleep");
   wifi_fpm_open();
   WiFi.forceSleepBegin();
   delay(3000);
   WiFi.forceSleepWake();
   wifi_fpm_close();
   wifi_fpm_do_wakeup();
   digitalWrite(LED_BUILTIN, HIGH); delay(200); digitalWrite(LED_BUILTIN, LOW); delay(200);
   digitalWrite(LED_BUILTIN, HIGH); delay(200); digitalWrite(LED_BUILTIN, LOW); delay(200);
}

void modo_light_sleep_CPU_OFF() {
   Serial.println("Esp em modo Light - sleep");
   wifi_station_disconnect();
   wifi_set_opmode(NULL_MODE);
   wifi_fpm_set_sleep_type(LIGHT_SLEEP_T); 
   wifi_fpm_open();
   gpio_pin_wakeup_enable(GPIO_ID_PIN(15), GPIO_PIN_INTR_HILEVEL); 
   Serial.flush();
   wifi_fpm_do_sleep(0xFFFFFFF);
   delay(100);
}

void modo_DEEP_SLEEP() {
   Serial.println("Esp em deep sleep");
   ESP.deepSleep(10000000 , WAKE_RF_DEFAULT);
   delay(100);
}


