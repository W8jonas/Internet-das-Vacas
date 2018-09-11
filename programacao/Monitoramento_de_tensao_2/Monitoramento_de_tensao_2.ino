
/*
 * Instituto Federal de Educação, Ciência e Tecnologia Minas Gerais
 * IFMG - Campus Avançado Conselheiro Lafaiete 
 * 
 * Internet das Vacas código monitoramento de tensão
 * Autor: Jonas Henrique Nascimento
 * PIBIC-Junior
 * 
 * Data de início: 06/09/2018
 * Data da ultima atualização: 08/09/2018
 * Data de término: 08/09/2018
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

String erro = "sem erros ate agora";

void leitura();

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
   
   if ( minuto_antigo != minuto_atual ){  
      minuto_antigo = minuto_atual;
      leitura();
      datalogger = SD.open("Tensoes2.svc", FILE_WRITE);
      if ( datalogger ) {
         Serial.println("Atualizando datalogger");
         Serial.print(rtc.getDateStr());
         Serial.print("  ");
         Serial.println(rtc.getTimeStr());
         Serial.print("Sensor 1 - ESP 8266 bateria de 750 mAh......: ");
         Serial.println(sensorValue);
         Serial.print("Sensor 2 - ESP 32 bateria de 750 mAh...: ");
         Serial.println(sensorValue2);
         Serial.println(" ");
//         datalogger.println("Data,Hora,sensorValue,sensorValue2,sensorValue3");
         datalogger.print(rtc.getDateStr());
         datalogger.print(",");
         datalogger.print(rtc.getTimeStr());
         datalogger.print(",");
         datalogger.print(sensorValue);
         datalogger.print(",");
         datalogger.print(sensorValue2);
         datalogger.print(",");
         datalogger.print(erro);
         datalogger.println(",  ");
         datalogger.close();
      } else {
         Serial.println("Erro ao abrir datalogger");
      }
      Serial.println("Atualizado");
   }

   sensorValuetensao = analogRead(sensortensao);
   sensorValuetensao =(sensorValuetensao * 3.75 ) / 843;
   if(sensorValuetensao > 1){
      flag = true;
      if( marcador_tempo_1 + 3 < minuto_atual ){
         marcador_tempo_1 = minuto_atual;
         Serial.println("Erro no marcador 1 xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
         Serial.println("  ");
         Serial.println("Erro no marcador 1 xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
         Serial.println("  ");
         Serial.println("Erro no marcador 1 xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
         Serial.println("  ");
         erro += " ,Erro no marcador 1, ";
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
         Serial.println("Erro no marcador 2 xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
         Serial.println("  ");
         Serial.println("Erro no marcador 2 xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
         Serial.println("  ");
         Serial.println("Erro no marcador 2 xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
         Serial.println("  ");
         erro += " ,Erro no marcador 1, ";
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
      Serial.print(sensorValue);
      Serial.print(" | ");
      Serial.println(sensorValue2);

   }
   sensorValue = sensorValue_soma / 10;
   sensorValue2 = sensorValue_soma2 / 10;

   
   Serial.print(sensorValue);
   Serial.print(" | ");
   Serial.print(sensorValue2);
   Serial.print(" | ");

}



