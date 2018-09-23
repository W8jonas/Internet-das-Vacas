
/*
 * Instituto Federal de Educação, Ciência e Tecnologia Minas Gerais
 * IFMG - Campus Avançado Conselheiro Lafaiete 
 * 
 * Internet das Vacas código monitoramento de tensão
 * Autor: Jonas Henrique Nascimento
 * PIBIC-Junior
 * 
 * Data de início.................: 06/09/2018
 * Data da ultima atualização.....: 23/09/2018
 * Data de atualização de versão..: 23/09/2018
 * Data de término................: 23/09/2018
 * 
 * 
 * 
 *  Este código está disponível sempre no endereço abaixo, para livre aperfeiçoamento. 
 *  Todavia, pede-se por educação, que ao compartilharem o código, mantenham os autores
 *  originais, tão bem quanto o nome da instituição.
 *  
 *  https://github.com/W8jonas/Internet-das-Vacas/blob/master/programacao/Monitoramento_de_tensao_2.1/Monitoramento_de_tensao_2.1.ino
 *  
*/


#include <DS3231.h>
#include <SPI.h>
#include <SD.h>
#define chip_select 4


int pino_sensor_de_tensao_ESP32 = A0;             // pino do arduino utilizado para medir a tensao da bateria ligada ao ESP32
float valor_do_sensor_de_tensao_ESP32 = 0;        // variavel de ponto flutuante que armazena o valor de tensao lido do ESP32
                                                  //
int pino_sensor_de_tensao_ESP8266 = A8;           // pino do arduino utilizado para medir a tensao da bateria ligada ao ESP8266
float valor_do_sensor_de_tensao_ESP8266 = 0;      // variavel de ponto flutuante que armazena o valor de tensao lido do ESP8266


int pino_sensor_controle_ESP32 = A1;              // Valor lido de tensao para converter em estado de HIGH ou LOW
float valor_do_sensor_de_controle_ESP32 = 0;      // Valor armazenado de tensao
                                                  //
int pino_sensor_controle_ESP8266 = A9;            // Valor lido de tensao para converter em estado de HIGH ou LOW
float valor_do_sensor_de_controle_ESP8266 = 0;    // Valor armazenado de tensao


unsigned int minuto_antigo = 0;
unsigned int minuto_atual = 0;
unsigned int marcador_tempo_1 = 0;
unsigned int marcador_tempo_2 = 0;

bool flag = false;
bool flag2 = false;
bool flag_controle_marcador_ON = false;
bool flag_controle_marcador_OFF = false;
bool flag_ligado = false;

String texto_marcador = "string marcador";
String dados = "string dados";
String condicao_ = "condicao";

unsigned char erro_marcador_1 = 0;
unsigned char erro_marcador_2 = 0;

void leitura();
void marcador(String marcador);
void gravar_dados_cartao();

DS3231  rtc(SDA, SCL);
Time  t;
File datalogger;

void setup() {
  
   Serial.begin(115200);
   rtc.begin();
   while (!Serial) {;}
   
   if (!SD.begin(chip_select)) {
     Serial.println("Erro ao ler cartao de memoria");
     return;
  }
  
}


void loop() {
   t = rtc.getTime();
   minuto_atual = t.min;
   
   if ( digitalRead(10) == HIGH) {
      condicao_ = "LIGADO";
      marcador(condicao_);
   } else {
      condicao_ = "DESLIGADO";
      marcador(condicao_);
   }
   
   if ( minuto_antigo != minuto_atual ){  
      minuto_antigo = minuto_atual;
      leitura();
      dados = texto_marcador + rtc.getDateStr() + ";" + rtc.getTimeStr() + ";" + valor_do_sensor_de_tensao_ESP8266 + ";" + valor_do_sensor_de_tensao_ESP8266 + ";" + erro_marcador_1 + ";" + erro_marcador_2 + ";*";
      gravar_dados_cartao();
   }

   valor_do_sensor_de_controle_ESP32 = analogRead(pino_sensor_controle_ESP32);
   valor_do_sensor_de_controle_ESP32 =(valor_do_sensor_de_controle_ESP32 * 3.75 ) / 843;
   if(valor_do_sensor_de_controle_ESP32 > 2){
      flag = true;
   }
   if( (valor_do_sensor_de_controle_ESP32 < 1) && (flag == true) ){
      erro_marcador_1++;
      Serial.println("Erro no marcador 2 (ESP32)");
      Serial.println(String (erro_marcador_1));
      Serial.println("  ");
      flag = false;
   }



   valor_do_sensor_de_controle_ESP8266 = analogRead(pino_sensor_controle_ESP8266);
   valor_do_sensor_de_controle_ESP8266 =(valor_do_sensor_de_controle_ESP8266 * 3.75 ) / 843;
   if(valor_do_sensor_de_controle_ESP8266 > 2){
      flag2 = true;
   }
   if( (flag2 == true) && (valor_do_sensor_de_controle_ESP8266 < 1) ) {
      erro_marcador_2++;
      Serial.println("Erro no marcador 1 (ESP8266)");
      Serial.println(String (erro_marcador_2));
      Serial.println("  ");
      flag2 = false;
   }
   
}


void leitura() {
   int x = 0;
   float valor_do_sensor_de_tensao_ESP32_soma = 0;
   float valor_do_sensor_de_tensao_ESP8266_soma2 = 0;
  
   while ( x < 10 ) {
      valor_do_sensor_de_tensao_ESP32 = analogRead(pino_sensor_de_tensao_ESP32);
      valor_do_sensor_de_tensao_ESP32 = (valor_do_sensor_de_tensao_ESP32 * 3.75 ) / 843;
      valor_do_sensor_de_tensao_ESP32_soma = valor_do_sensor_de_tensao_ESP32 + valor_do_sensor_de_tensao_ESP32_soma;
      
      valor_do_sensor_de_tensao_ESP8266 = analogRead(pino_sensor_de_tensao_ESP8266);
      valor_do_sensor_de_tensao_ESP8266 = (valor_do_sensor_de_tensao_ESP8266 * 3.75 ) / 843;
      valor_do_sensor_de_tensao_ESP8266_soma2 = valor_do_sensor_de_tensao_ESP8266 + valor_do_sensor_de_tensao_ESP8266_soma2;
      
      x++;

   }
   valor_do_sensor_de_tensao_ESP32 = valor_do_sensor_de_tensao_ESP32_soma / 10;
   valor_do_sensor_de_tensao_ESP8266 = valor_do_sensor_de_tensao_ESP8266_soma2 / 10;

}


void marcador(String controle){
   if ( (controle == "LIGADO") && (flag_controle_marcador_ON == false) && (flag_ligado == false) ) {
      flag_controle_marcador_ON = true;
      texto_marcador = "\n \n \n \n \n \n \n \n \n \n Data;Hora;valor_do_sensor_de_tensao_ESP32;valor_do_sensor_de_tensao_ESP8266;erro marcador 1 (ESP8266);erro marcador 2 (ESP32); ||; \n";
      flag_ligado = true;
      Serial.println(texto_marcador);
      delay(1000);
   } else {
      if ( (controle == "DESLIGADO") && (flag_controle_marcador_OFF == false) && (flag_ligado == true) ) {
         flag_controle_marcador_OFF = true;
         texto_marcador = "\n \n \n \n \n \n \n \n \n \n  ; ; ; ; ; || \n";
         Serial.println(texto_marcador);
         delay(1000);
      } 
   }
}


void gravar_dados_cartao() {
   
   datalogger = SD.open("2x4.svc", FILE_WRITE);
      if ( datalogger ) {
         Serial.println("Atualizando datalogger");
         Serial.print(dados);
         Serial.println("\n");

         unsigned int tamanho_char = 100;
         char vetor_dados [tamanho_char] = {""};
         dados.toCharArray(vetor_dados, tamanho_char);

         for ( int i = 0; i <= tamanho_char; i++){
            if( vetor_dados[i] == '\n' ){
               datalogger.println(" ");
            }
            if ( vetor_dados[i] == '*' ) {
               break;
            }
            datalogger.write(vetor_dados[i]);
         }
         datalogger.println(" ||;");
         datalogger.close();
      } else {
         Serial.println("Erro ao abrir datalogger");
      }
      Serial.println("Atualizado");
      texto_marcador = "";
      dados = "";
}



