
//  Internet das Vacas código 3
//  Autor: Jonas Henriquee Nascimento
//  PIBIC-Junior
//
//  Data de início: 04/06/2018
//  Data da ultima atualização: 19/06/2018
//  Data de término: 08/06/2018
//  
//  O código tem como objetivo estabelecer 6 funções diferentes para o arduino
//  executar cada uma delas de moto paralelo. Essas funções são escolhidas através 
//  de um botão que toda que apertado faz com que o código execute a função seguinte
//  A primeira função, que é executada junto ao ESP quando é ligado deixa todos os
//  LEDs desligados e o ESP em modo de standby. A segunda função, liga somente um
//  dos LEDs. A terceira, por sua vez, liga todos os 8 LEDs. A próxima função 
//  executa uma série de operações aritméticas, com todos os LEDs desligados. Já a 
//  função 5 executa as mesmas operações aritméticas, mas com 1 dos LEDs ligados. 
//  De tal forma é feito na 6 função, no qual são executadas as operações matemáticas,
//  mas com todos os LEDs ligados.

//  Este código está disponível sempre no endereço abaixo, para livre aperfeiçoamento. 
//  Todavia, pede-se por educação, que ao compartilharem o código, mantenham os autores
//  originais, tão bem quanto o nome da instituição.
//  https://github.com/W8jonas/Internet-das-Vacas/blob/master/programacao/codigo_modos_de_operacao/codigo_modos_de_operacao.ino




#define Output_1 D8
#define Output_2 D7
#define Output_3 D6
#define Output_4 D5
#define Output_5 D4
#define Output_6 D3
#define Output_7 D2
#define Output_8 D1
#define entrada_botao D0

void funcao__();
void funcao_0();
void funcao_1();
void funcao_2();
void funcao_3();
void funcao_4();

int operacao = 0;
boolean leitura = true;

void setup() {
   pinMode (Output_1, OUTPUT);
   pinMode (Output_2, OUTPUT);
   pinMode (Output_3, OUTPUT);
   pinMode (Output_4, OUTPUT);
   pinMode (Output_5, OUTPUT);
   pinMode (Output_6, OUTPUT);
   pinMode (Output_7, OUTPUT);
   pinMode (Output_8, OUTPUT);
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

  funcao_2(); 

}

void funcao__ (){
   Serial.println("funcao 00 ");
   digitalWrite(Output_1, LOW);
   digitalWrite(Output_2, LOW);
   digitalWrite(Output_3, LOW);
   digitalWrite(Output_4, LOW);
   digitalWrite(Output_5, LOW);
   digitalWrite(Output_6, LOW);
   digitalWrite(Output_7, LOW);
   digitalWrite(Output_8, LOW);
}

void funcao_0() {
   Serial.println("funcao 0 ");
   digitalWrite(Output_1, HIGH);
}

void funcao_1() {
   Serial.println("funcao 1 ");
   digitalWrite(Output_1, HIGH);
   digitalWrite(Output_2, HIGH);
   digitalWrite(Output_3, HIGH);
   digitalWrite(Output_4, HIGH);
   digitalWrite(Output_5, HIGH);
   digitalWrite(Output_6, HIGH);
   digitalWrite(Output_7, HIGH);
   digitalWrite(Output_8, HIGH);
}

void funcao_2() {
   Serial.println("funcao 2 ");
   digitalWrite(Output_1, LOW);
   digitalWrite(Output_2, LOW);
   digitalWrite(Output_3, LOW);
   digitalWrite(Output_4, LOW);
   digitalWrite(Output_5, LOW);
   digitalWrite(Output_6, LOW);
   digitalWrite(Output_7, LOW);
   digitalWrite(Output_8, LOW);
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

void funcao_3() {
   Serial.println("funcao 3 ");
   digitalWrite(Output_1, HIGH);
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

void funcao_4() {
   Serial.println("funcao 4 ");
   digitalWrite(Output_1, HIGH);
   digitalWrite(Output_2, HIGH);
   digitalWrite(Output_3, HIGH);
   digitalWrite(Output_4, HIGH);
   digitalWrite(Output_5, HIGH);
   digitalWrite(Output_6, HIGH);
   digitalWrite(Output_7, HIGH);
   digitalWrite(Output_8, HIGH);
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

