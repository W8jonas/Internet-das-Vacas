
/*
 * Instituto Federal de Educação, Ciência e Tecnologia Minas Gerais
 * IFMG - Campus Avançado Conselheiro Lafaiete 
 * 
 *  Internet das Vacas código 3
 *  Autor: Jonas Henrique Nascimento
 *  PIBIC-Junior
 * 
 *  Data de início: 13/08/2018
 *  Data da ultima atualização: 02/09/2018
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
 *  https://github.com/W8jonas/Internet-das-Vacas/blob/master/programacao/codigo_teste_de_consumo_ESP32/codigo_teste_de_consumo_ESP32.ino
 *  
*/


#include <WiFi.h>

#define tempo_para_desligar 25000
#define tempo_modo_funcionamento 20000
#define tempo_modo_funcionamento2 25000
#define Pino_transmit 4

void modo_DEEP_SLEEP();
void Servidor_ON(unsigned long contador_interno);
void MAX_CPU();

const char* ssid = "82W8JH";
const char* password = "w8989jonas";

WiFiServer server(80);



void setup() {
  
   Serial.begin(115200);
   Serial.println("Inicializando o setup");
   
 //  WiFi.setPhyMode(WIFI_PHY_MODE_11N);
 //  WiFi.setOutputPower(14);
 //  WiFi.mode(WIFI_STA);
   
   WiFi.begin(ssid, password);
 //  wifi_set_user_fixed_rate(1, 54);
   
   int contador = 0;
   
   while (WiFi.status() != WL_CONNECTED) {
      contador ++;
      delay(50);
   }

   server.begin();
   delay(500);
   
   Serial.println("");
   Serial.print("Conectando em: ");
   Serial.println(ssid);
   Serial.print("IP: ");
   Serial.println(WiFi.localIP());
   esp_sleep_enable_timer_wakeup(100000000);

   pinMode(Pino_transmit, OUTPUT);
   digitalWrite(Pino_transmit, HIGH);
}


void loop() {
   unsigned long valor_atual_contador = millis();
   Serial.println(valor_atual_contador);
   
   if (valor_atual_contador < tempo_modo_funcionamento) {
      MAX_CPU();
   } 
   
   if ((valor_atual_contador >= tempo_modo_funcionamento) && (valor_atual_contador < tempo_modo_funcionamento2)) {
      Servidor_ON(valor_atual_contador);
   }
   
//   client.stop();
   
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

void Servidor_ON(unsigned long valor_cont){
    
    WiFiClient client = server.available();
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            client.print("Contador");
            client.print(valor_cont);

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

      }
    }


}

void modo_DEEP_SLEEP() {
   digitalWrite(Pino_transmit, LOW);
   digitalWrite(Pino_transmit, HIGH);
   delay(800);
   digitalWrite(Pino_transmit, LOW);
  
   Serial.println("Deep-Sleep");
   delay(500);
   esp_deep_sleep_start();
   delay(500);
}




