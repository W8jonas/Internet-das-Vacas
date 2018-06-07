
#define Output_1 D4
#define Output_2 0
#define Output_3 4
#define Output_4 5
#define Output_5 15
#define Output_6 13
#define Output_7 12
#define Output_8 14
#define entrada_botao 16

void funcao__();
void funcao_0();
void funcao_1();
void funcao_2();
void funcao_3();
void funcao_4();
void funcao_5();
void funcao_6();
void funcao_7();

int operacao = 0;

void setup() {
   pinMode (Output_1, OUTPUT);
   pinMode (Output_2, OUTPUT);
   pinMode (Output_3, OUTPUT);
   pinMode (Output_4, OUTPUT);
   pinMode (Output_5, OUTPUT);
   pinMode (Output_6, OUTPUT);
   pinMode (Output_7, OUTPUT);
   pinMode (Output_8, OUTPUT);
   pinMode (entrada_botao, INPUT_PULLUP);
}


void loop() {
   switch (operacao){
      case 0: funcao__(); break;  // desliga todos os LEDs
      case 1: funcao_0(); break;  // liga 1 LED 
      case 2: funcao_1(); break;  // liga todos os 8 LEDs
      case 3: funcao_2(); break;  // executa operações aritméticas
      case 4: funcao_3(); break;  // executa as operações aritméticas com 1 LED ligado
      case 5: funcao_4(); break;  // executa as operações aritméticas com todos os LEDs ligados
      case 6: funcao_5(); break;  // entra em modo SLEEP
      case 7: funcao_6(); break;  // entra em modo DEEP-SLEEP
      case 8: funcao_7(); break;
   }
   delay(500);
   operacao++;
   if ( operacao > 8 ) operacao = 0;
   
}
void funcao__ (){
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
   digitalWrite(Output_1, HIGH);
}

void funcao_1() {
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

void funcao_5() {
   
   
}

void funcao_6() {
   
   
}

void funcao_7() {
   
   
}


