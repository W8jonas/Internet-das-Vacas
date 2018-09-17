
/*
 * Instituto Federal de Educação, Ciência e Tecnologia Minas Gerais
 * IFMG - Campus Avançado Conselheiro Lafaiete 
 * 
 * Internet das Vacas código 2
 * Autor: Jonas Henrique Nascimento
 * PIBIC-Junior
 * 
 * Data de início: 06/04/2018
 * Data da ultima atualização: 13/05/2018
 * Data de término: 13/05/2018
 * 
 * Tem o objetivo de estabelecer um servidor dentro do ESP que possibilite ligar 8 LEDs independentes e optar por 2 estados de processamento;
 * O primeiro, a qual não será realizado nenhuma operação adicional além do próprio servidor.
 * No segundo, o processador, além de manter o próprio servidor, executará contas aritméticas adicinais afim de se usar 100% da CPU.
 * O propósito do programa é a relaziação dos teste com relação ao consumo da placa.
 *
 *  Este código está disponível sempre no endereço abaixo, para livre aperfeiçoamento. 
 *  Todavia, pede-se por educação, que ao compartilharem o código, mantenham os autores
 *  originais, tão bem quanto o nome da instituição.
 *
 * https://github.com/W8jonas/Internet-das-Vacas/blob/master/programacao/codigo_do_servidor---consumov2/codigo_do_servidor---consumov2.ino
 *  
*/


#include <ESP8266WiFi.h>

const char* ssid = "esp8266";
const char* password = "1234567890";

WiFiServer server(80); //Shield irá receber as requisições das páginas (o padrão WEB é a porta 80)

String HTTP_req; 
String URLValue;

void processaPorta(byte porta, byte posicao, WiFiClient cl);
void lePortaDigital(byte porta, byte posicao, WiFiClient cl);
void lePortaAnalogica(byte porta, byte posicao, WiFiClient cl);

void conta_1();
  
String getURLRequest(String *requisicao);
bool mainPageRequest(String *requisicao);

const byte qtdePinosDigitais = 9;
byte pinosDigitais[qtdePinosDigitais] = {2      , 0      , 4      , 5      , 15     , 13     , 12      , 14      , 16     };
byte modoPinos[qtdePinosDigitais]     = {OUTPUT , OUTPUT , OUTPUT , OUTPUT , OUTPUT , OUTPUT , OUTPUT  , OUTPUT  , OUTPUT };
boolean flag = true;
int condicao = 2;



void setup(){
    Serial.begin(115200);

    Serial.println();
    Serial.print("Conectando a ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi conectado!");


    server.begin();
    Serial.println("Server iniciado");

    Serial.println(WiFi.localIP());
    
    for (int nP=0; nP < qtdePinosDigitais; nP++) {
        pinMode(pinosDigitais[nP], modoPinos[nP]);
    }
}


void loop(){

    WiFiClient  client = server.available();

    if (client) { 
        boolean currentLineIsBlank = true;
        while (client.connected()) {
            if (client.available()) { 
                char c = client.read(); 
                HTTP_req += c;  
                
                if (c == '\n' && currentLineIsBlank ) { 

                    if ( mainPageRequest(&HTTP_req) ) {
                        URLValue = getURLRequest(&HTTP_req);
                        Serial.println(HTTP_req);
                                                 
                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-Type: text/html");
                        client.println("Connection: keep-alive");              
                        client.println();
                        
                        //Conteudo da Página HTML
                        client.println("<!DOCTYPE html>");
                        client.println("<html>");

                        client.println("<head>");
                        client.println("<title>Servidor de teste</title>");

                        client.println("<script>");
                        client.println("function LeDadosDoArduino() {");
                        client.println("nocache = \"&nocache=\" + Math.random() * 1000000;");
                        client.println("var request = new XMLHttpRequest();");
                        client.println("var posIni;");
                        client.println("var valPosIni;");
                        client.println("var valPosFim;");
                        client.println("request.onreadystatechange = function() {");
                        client.println("if (this.readyState == 4) {");
                        client.println("if (this.status == 200) {");
                        client.println("if (this.responseText != null) {");

                        for (int nL=0; nL < qtdePinosDigitais; nL++) {                                                 
                            client.print("posIni = this.responseText.indexOf(\"PD");
                            client.print(pinosDigitais[nL]);
                            client.println("\");");
                            client.println("if ( posIni > -1) {");
                            client.println("valPosIni = this.responseText.indexOf(\"#\", posIni) + 1;");
                            client.println("valPosFim = this.responseText.indexOf(\"|\", posIni);");
                            client.print("document.getElementById(\"pino");
                            client.print(pinosDigitais[nL]);
                            client.println("\").checked = Number(this.responseText.substring(valPosIni, valPosFim));");
                            client.println("}");
                        }


                        client.println("}}}}");
                        client.println("request.open(\"GET\", \"solicitacao_via_ajax\" + nocache, true);");
                        client.println("request.send(null);");
                        client.println("setTimeout('LeDadosDoArduino()', 5000);");
                        client.println("}");
                        client.println("</script>");
                        
                        client.println("</head>");

                        client.println("<body onload=\"LeDadosDoArduino()\">");                                     
                        client.println("<h1>Porta analogica</h1>");

                        client.println("<br/>");                        
                        client.println("<h1>PORTAS digitais</h1>");
                        client.println("<form method=\"get\">");

                        for (int nL=0; nL < qtdePinosDigitais; nL++) {
                            processaPorta(pinosDigitais[nL], nL, client);
                            client.println("<br/>");
                        }
                        
                        client.println("<button type=\"submit\">Envia as opcoes para o ESP8266</button>");
                        client.println("</form>");                      
                        
                        client.println("</body>");
                        
                        client.println("</html>");
                    
                    } else if (HTTP_req.indexOf("solicitacao_via_ajax") > -1) {  

                        Serial.println(HTTP_req);

                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-Type: text/html");
                        client.println("Connection: keep-alive");      
                        client.println();                      


                        for (int nL=0; nL < qtdePinosDigitais; nL++) {
                            lePortaDigital(pinosDigitais[nL], nL, client);
                        }
                            
                    } else {

                        Serial.println(HTTP_req);
                        client.println("HTTP/1.1 200 OK");
                    }
                    HTTP_req = "";    
                    break;
                }
                
                if (c == '\n') {
                    currentLineIsBlank = true;
                } 
                else if (c != '\r') {
                    currentLineIsBlank = false;
                }
            }
        } 
        delay(1);     
        client.stop(); 
    } 

  if(condicao == 1)conta_1();
  
}


void processaPorta(byte porta, byte posicao, WiFiClient cl){
static boolean LED_status = 0;
String cHTML;
   
    cHTML = "P";
    cHTML += porta;
    cHTML += "=";
    cHTML += porta;

    if (modoPinos[posicao] == OUTPUT) { 
        
        if (URLValue.indexOf(cHTML) > -1) { 
           LED_status = HIGH;
        } else {
           LED_status = LOW;
        }
        if((porta == 16) && (LED_status == HIGH)){
          condicao = 1;
        } else{
          condicao = 0;
        }
        digitalWrite(porta, LED_status);
    } else {

        LED_status = digitalRead(porta);
    }

    cl.print("<input type=\"checkbox\" name=\"P");
    cl.print(porta);
    cl.print("\" value=\"");
    cl.print(porta);
    
    cl.print("\"");

    cl.print(" id=\"pino");        
    cl.print(porta);
    cl.print("\"");
    
    if (LED_status) { 
        cl.print(" checked ");
    }

    if (modoPinos[posicao] != OUTPUT) { 
        cl.print(" disabled ");
    }
    
    cl.print(">Porta ");
    cl.print(porta);

    cl.println();
}


void lePortaDigital(byte porta, byte posicao, WiFiClient cl){
    if (modoPinos[posicao] != OUTPUT) { 
       cl.print("PD");
       cl.print(porta);
       cl.print("#");
       
       if (digitalRead(porta)) {
          cl.print("1");
       } else {
          cl.print("0");
       }
       cl.println("|");
    }
}


void lePortaAnalogica(byte porta, byte posicao, WiFiClient cl){
   cl.print("PA");
   cl.print(porta);
   cl.print("#");
   
   cl.print(analogRead(porta));

   //especifico para formatar o valor da porta analogica A0
   if (porta == A0) { 
      cl.print(" (");
      cl.print(map(analogRead(A0),0,1023,0,179)); 
      cl.print("&deg;)");
   }
   
   cl.println("|");   
}


String getURLRequest(String *requisicao) {
int inicio, fim;
String retorno;

  inicio = requisicao->indexOf("GET") + 3;
  fim = requisicao->indexOf("HTTP/") - 1;
  retorno = requisicao->substring(inicio, fim);
  retorno.trim();

  return retorno;
}


bool mainPageRequest(String *requisicao) {
  
  
String valor;
bool retorno = false;

  valor = getURLRequest(requisicao);
  valor.toLowerCase();

  if (valor == "/") {
     retorno = true;
  }

  if (valor.substring(0,2) == "/?") {
     retorno = true;
  }  

  if (valor.substring(0,10) == "/index.htm") {
     retorno = true;
  }  

  return retorno;
}


void conta_1(){  
  int cont = 1;
  float resp = 1;
  float resp2 = 1;
  for(int AA = 0; AA < 500; AA++){
     resp = 3 + sin(resp)/cos(resp*resp/2) * sqrt(sqrt(resp*resp));
     resp = resp * 0.5;
     Serial.println(resp);
     Serial.print("   ");
     resp2 = sqrt(AA);
     Serial.print(resp2);
     Serial.print("   ");
     yield();
  }
}


