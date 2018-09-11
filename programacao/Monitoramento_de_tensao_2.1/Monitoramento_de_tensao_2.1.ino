
/*
 * Instituto Federal de Educação, Ciência e Tecnologia Minas Gerais
 * IFMG - Campus Avançado Conselheiro Lafaiete 
 * 
 * Internet das Vacas código monitoramento de tensão
 * Autor: Jonas Henrique Nascimento
 * PIBIC-Junior
 * 
 * Data de início.................: 06/09/2018
 * Data da ultima atualização.....: 11/09/2018
 * Data de atualização de versão..: 11/09/2018
 * Data de término................: 11/09/2018
 * 
 * 
 *
 *  Este código está disponível sempre no endereço abaixo, para livre aperfeiçoamento. 
 *  Todavia, pede-se por educação, que ao compartilharem o código, mantenham os autores
 *  originais, tão bem quanto o nome da instituição.
 *  
*/


#include <DS3231.h>
#include <SPI.h>
#include <SD.h>
#define chip_select 4

int sensorPin   = A0;
float sensorValue = 0;

int sensorPin2   = A1;
float sensorValue2 = 0;

int sensortensao   = A2;
float sensorValuetensao = 0;

int sensortensao2   = A3;
float sensorValuetensao2 = 0;

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
      dados = texto_marcador + rtc.getDateStr() + ";" + rtc.getTimeStr() + ";" + sensorValue + ";" + sensorValue2 + ";" + erro_marcador_1 + ";" + erro_marcador_2 + ";";
      gravar_dados_cartao();
   }

   sensorValuetensao = analogRead(sensortensao);
   sensorValuetensao =(sensorValuetensao * 3.75 ) / 843;
   if(sensorValuetensao > 1){
      flag = true;
      if( marcador_tempo_1 + 3 < minuto_atual ){
         marcador_tempo_1 = minuto_atual;
         erro_marcador_1++;
         Serial.println("Erro no marcador 1");
         Serial.println(String (erro_marcador_1));
         Serial.println("  ");
      }
   }

   if( (flag == true) && (sensorValuetensao < 1) ) {
      marcador_tempo_1 = minuto_atual;
      flag = false;
   }


   sensorValuetensao2 = analogRead(sensortensao2);
   sensorValuetensao2 =(sensorValuetensao2 * 3.75 ) / 843;
   if(sensorValuetensao2 > 1){
      flag2 = true;
      if( marcador_tempo_2 + 3 < minuto_atual){
         marcador_tempo_2 = minuto_atual;
         erro_marcador_2++;
         Serial.println("Erro no marcador 2");
         Serial.println(String (erro_marcador_2));
         Serial.println("  ");
      }
   }
   if( (flag2 == true) && (sensorValuetensao2 < 1) ) {
      marcador_tempo_2 = minuto_atual;
      flag2 = false;
   }
}


void leitura() {
   int x = 0;
   float sensorValue_soma = 0;
   float sensorValue_soma2 = 0;
  
   while ( x < 10 ) {
      sensorValue = analogRead(sensorPin);
      sensorValue = (sensorValue * 3.75 ) / 843;
      sensorValue_soma = sensorValue + sensorValue_soma;
      
      sensorValue2 = analogRead(sensorPin2);
      sensorValue2 = (sensorValue2 * 3.75 ) / 843;
      sensorValue_soma2 = sensorValue2 + sensorValue_soma2;
      
      x++;

   }
   sensorValue = sensorValue_soma / 10;
   sensorValue2 = sensorValue_soma2 / 10;

}


void marcador(String controle){
   if ( (controle == "LIGADO") && (flag_controle_marcador_ON == false) && (flag_ligado == false) ) {
      flag_controle_marcador_ON = true;
      texto_marcador = "\n \n \n \n \n \n \n \n \n \n Data;Hora;sensorValue;sensorValue2;erro marcador 1;erro marcador 2; ||; \n";
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
   
   datalogger = SD.open("Tensoes3.svc", FILE_WRITE);
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



