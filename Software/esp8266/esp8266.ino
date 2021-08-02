/* Software para Módulo Esp8266 NodeMCU 12E para apresentação do trabalho de graduação
   
 Este software objetiva-se em criar uma integração entre a placa esp8266 e a assistente virtual alexa,
 assim como adicionar os ddispositivos correspondentes a todos os circuitos de iluminação instalados
 e configurados na placa mega ADK.

   Após o esp8266 receber o comando através da assistente virtual, ele envia um dado através do protocolo
   de comunicação i2c para a placa mega ADK, que por sua vez analisa a informação recebida e toma sua decisão
   baseado em suas próprias configurações.
 

Autoria: Lucas Mosca
         Luigi Cavalieri

         Julho de 2021
         
===============================================================================================================================

Descritivo dos circuitos de iluminação:

Circuito 1:  L1, L2, L3 -> Área de Serviço

Circuito 2:  L4 -> Sala de Estar Lateral
Circuito 3:  L5 -> Sala de Estar Entrada
Circuito 4:  L6 -> Sala de Estar TV

Circuito 5:  L7 -> WC Social

Circuito 6:  L8 -> Sala de Jantar Mesa
Circuito 7:  L9 -> Sala de jantar Pendente

Circuito 8:  L10 -> Cozinha Pia
Circuito 9:  L11 -> Cozinha Balcão

Circuito 10: L12, L13, L14 -> Hall Íntimo

Circuito 11: L15, L16, L17, L18, L19, L20, L21 -> Jardim Lateral

Circuito 12: L22 -> Suíte 1 Armário
Circuito 13: L23 -> Suíte 1 WC Chuveiro
Circuito 14: L24 -> Suíte 1 WC Pia
Circuito 15: L25 -> Suíte 1 TV
Circuito 16: L26 -> Suíte 1 Cama

Circuito 17: L27 -> Suíte Master WC Pia
Circuito 18: L28 -> Suíte Master WC Chuveiro
Curcuito 19: L30, L31 -> Suíte Master Closet
Circuito 20: L29, L33 -> Suíte Master Cama
Circuito 21: L34 -> Suíte Master Entrada
Circuito 22: L32 -> Suíte Master TV

Circuito 23: L35, L36, L37, L38, L39 -> Jardim de Entrada 

===============================================================================================================================
*/

// Seleciona a biblioteca correspondente a placa reconhecida (esp32 ou esp8266)
#ifdef ARDUINO_ARCH_ESP32
#include <WiFi.h>                                               // Inclui a biblioteca Wifi
#else
#include <ESP8266WiFi.h>                                      
#endif 

#define ESPALEXA_MAXDEVICES 50                                  // Define a quantidade maxima de dispositivos reconhecidos pela assistente
#include <Espalexa.h>                                           // Inclui a bibliotec Espalexa
#include <Wire.h>                                               // Inclui a biblioteca Wire

//===============================================================================================================================

// --- Declaração das Funções utilizadas ---

boolean connectWifi();                                          // Rotina para conexão Wifi

void lavanderia(uint8_t brightness);                            // Rotina para seccionar o circuito da lavanderia

// Rotinas para seccionar os circuitos da sala de estar
void seLateral(uint8_t brightness);
void seEntrada(uint8_t brightness);
void seTv(uint8_t brightness);

// Rotinas para secconar o circuito do banheiro social
void banheiroSocial(uint8_t brightness);

// Rotinas para seccionar os circuitos da sala de jantar
void sjLustre(uint8_t brightness);
void sjPendente(uint8_t brightness);

// Rotinas para seccionar os circuitos da cozinha
void cozinhaPia(uint8_t brightness);
void cozinhaBalcao(uint8_t brightness);

// Rotina para seccionar o circuito do hall
void hall(uint8_t brightness);

// Rotina para seccionar o circuito do jardim lateral
void jardimLateral(uint8_t brightness);

// Rotinas para seccionar os circuitos da suíte 1
void s1WcChuveiro(uint8_t brightness);
void s1WcPia(uint8_t brightness);
void s1Tv(uint8_t brightness);
void s1Cama(uint8_t brightness);
void s1Armario(uint8_t brightness);

// Rotinas para seccionar os circuitos da suíte master
void smWcPia(uint8_t brightness);
void smWcChuveiro(uint8_t brightness);
void smCloset(uint8_t brightness);
void smCama(uint8_t brightness);
void smEntrada(uint8_t brightness);
void smTv(uint8_t brightness);

// Rotina para seccionar o circuito do jardim de entrada
void jardimEntrada(uint8_t brightness);

// Rotina para seccionar o relé da maleta de apresentação
void relay(uint8_t brightness);

//===============================================================================================================================

// --- Definições do projeto ---

#define SlaveAdress 0x08                                              // Define o endereço da placa escravo no protocolo i2c
#define LED D0                                                        // Define a label 'LED' para o terminal D0 da placa

//===============================================================================================================================

// --- Declaração de Constantes ---
const char* ssid = "Nome da sua rede wifi";                           // Armazena o nome  da rede wifi em 'ssid'
const char* password = "Senha da sua rede wifi";                      // Armazena a senha da rede wifi em 'password'

//===============================================================================================================================

// --- Declaração de variáveis globais ---
boolean wifiConnected = false;
unsigned int numberTransmission = 0x00;

//===============================================================================================================================

// Cria uma instância do tipo Espalexa chamada espalexa
Espalexa espalexa;

EspalexaDevice* device3; //this is optional

//===============================================================================================================================
              
void setup() {
 pinMode(LED, OUTPUT);                                              // Define o terminal 'LED' como saída              
 Serial.begin(9600);                                                // Baud Rate
 Wire.begin(D1, D2);                                                // Inicia o protocolo i2c: SDA = D1, SCL = D2

  while (! connectWifi()){                                          // Enquanto não houver conexão com a rede wifi, 
    }                                                               // continua tentando...
  digitalWrite(LED, HIGH);                                          // Aciona a saída 'LED', indicando que houve conexão
    
// --- Definição dos dispositivos a serem adicionados na alexa --- 
    espalexa.addDevice("Lavanderia", lavanderia);                   // Adiciona dispositivo 'Lavanderia', comandado por 'lavanderia'
    
    espalexa.addDevice("Sala de Estar Lateral", seLateral);         // Adiciona dispositivo 'Sala de Estar Lateral', comandado por 'seLateral'
    espalexa.addDevice("Sala de Estar Entrada", seEntrada);         // Adiciona dispositivo 'Sala de Estar Entrada', comandado por 'seEntrada'
    espalexa.addDevice("Sala de Estar TV", seTv);                   // Adiciona dispositivo 'Sala de Estar TV', comandado por 'seTv'

    espalexa.addDevice("Banheiro Social", banheiroSocial);          // Adiciona dispositivo 'Banheiro Social', comandado por 'banheiroSocial'
    
    espalexa.addDevice("Sala de Jantar", sjPendente);               // Adiciona dispositivo 'Sala de Jantar', comandado por 'sjPendente'
    espalexa.addDevice("Lustre", sjLustre);                         // Adiciona dispositivo 'Lustre', comandado por 'sjLustre'
     
    espalexa.addDevice("Cozinha Pia", cozinhaPia);                  // Adiciona dispositivo 'Cozinha Pia', comandado por 'cozinhaPia'
    espalexa.addDevice("Cozinha Balcão", cozinhaBalcao);            // Adiciona dispositivo 'Cozinha Balcão', comandado por 'cozinhaBalcao'

    espalexa.addDevice("hall", hall);                               // Adiciona dispositivo 'hall', comandado por 'hall'

    espalexa.addDevice("Jardim Lateral", jardimLateral);            // Adiciona dispositivo 'Jardim Lateral', comandado por 'jardimLateral'
    
    espalexa.addDevice("Chuveiro da Suíte 1", s1WcChuveiro);        // Adiciona dispositivo 'Chuveiro da Suíte 1', comandado por 's1WcChuveiro'
    espalexa.addDevice("Pia da Suíte 1", s1WcPia);                  // Adiciona dispositivo 'Pia da Suíte 1', comandado por 's1WcPia'
    espalexa.addDevice("TV Suíte 1", s1Tv);                         // Adiciona dispositivo 'TV da Suíte 1', comandado por 's1Tv'
    espalexa.addDevice("Cama da Suíte 1", s1Cama);                  // Adiciona dispositivo 'Cama da Suíte 1', comandado por 's1Cama'
    espalexa.addDevice("Closet da Suíte 1", s1Armario);             // Adiciona dispositivo 'Closet da Suíte 1', comandado por 's1Closet'

    espalexa.addDevice("Pia da Suíte Master", smWcPia);             // Adiciona dispositivo 'Pia da Suíte Master', comandado por 'smWcPia'
    espalexa.addDevice("Chuveiro da Suíte master", smWcChuveiro);   // Adiciona dispositivo 'Chuveiro da Suíte Master', comandado por 'smWcChuveiro'
    espalexa.addDevice("Closet Master", smCloset);                  // Adiciona dispositivo 'Closet Master', comandado por 'smCloset'
    espalexa.addDevice("Cama da Suíte Master", smCama);             // Adiciona dispositivo 'Cama da Suíte Master', comandado por 'smCama'
    espalexa.addDevice("Entrada da Suíte Master", smEntrada);       // Adiciona dispositivo 'Entrada da Suíte Master', comandado por 'smEntrada'
    espalexa.addDevice("TV Suíte Master", smTv);                    // Adiciona dispositivo 'TV Suíte Master', comandado por 'smTv'

    espalexa.addDevice("Jardim Entrada", jardimEntrada);            // Adiciona dispositivo 'Jardim Entrada', comandado por 'jardimEntrada'

    espalexa.addDevice("Relay 1", relay);                           // Adiciona dispositivo 'Relay 1', comandado por 'relay'
    
    espalexa.begin();
    
}                                                                   // Fim - Setup
//====================================================================================================================

void loop() {
  
  espalexa.loop();
  delay(1);                                                         // Aguarda 1ms

  if (WiFi.status() != WL_CONNECTED){                               // Se não estiver conectado a rede Wifi 
    digitalWrite(LED, LOW);                                         // Desaciona 'LED'
    while(! connectWifi()){                                         // Enquanto não houver conexão,
      }                                                             // espera se conectar...
      digitalWrite(LED, HIGH);                                      // Liga 'LED'
    }                                                               // Fim - While
  
}                                                                   // Fim - Loop

//====================================================================================================================

// Rotina para seccionar o rele
void relay(uint8_t brightness){
  
  if (brightness){                                          // Se pedir para alexa ligar,
    numberTransmission = 0x32;                              // 'numberTransmission' recebe um valor inteiro
    Wire.beginTransmission(SlaveAdress);                    // Inicia a tansmissaão de dados i2c para o endereço 0x08
    Wire.write(numberTransmission);                         // Envia o valor contido em 'numberTransmission'
    Wire.endTransmission();                                 // Encera a comunicação
    numberTransmission = 0x00;                              // Reseta a variável
    }

  else{                                                     // Se não,
      numberTransmission = 0x33;                            // 'numberTransmission' recebe um valor inteiro
      Wire.beginTransmission(SlaveAdress);                  // Inicia a transmissão para o endereço 0x08
      Wire.write(numberTransmission);                       // Envia o valor contido em 'numberTransmission'
      Wire.endTransmission();                               // Encerra a comunicação
      numberTransmission = 0x00;                            // Reseta a variável
    }                                                       // Fim - else
  }                                                         // Fim - rotina

//====================================================================================================================

// Rotina para seccionar a lavanderia
void lavanderia(uint8_t brightness){

  if(brightness){                                         // se pedir para alexa ligar
    numberTransmission = 0x01;                            // 'numberTransmission' recebe um valor inteiro 
    Wire.beginTransmission(SlaveAdress);                  // Inicia a transmissão de dados i2c para o endereço 0x08
    Wire.write(numberTransmission);                       // Envia o valor contido em 'numberTransmission'
    Wire.endTransmission();                               // Encerra a comunicação
    numberTransmission = 0x00;                            // Reseta a variável
  
  }                                                       // Fim do if
  
  else{                                                   // Se não
      numberTransmission = 0x02;                          // 'numberTransmission' recebe um valor inteiro
      Wire.beginTransmission(SlaveAdress);                // Inicia a transmissão de dados i2c para o endereço 0x08
      Wire.write(numberTransmission);                     // Envia o valor contido em 'numberTransmission'
      Wire.endTransmission();                             // Encerra a comunicação
      numberTransmission = 0x00;                          // Reseta a variável
      
      }                                                   // Fim do else
      
  }                                                       // Fim da rotina

//====================================================================================================================

// Rotina para seccionar um circuito da sala de estar
void seLateral(uint8_t brightness){

  if(brightness){                                         // se pedir para alexa ligar
    numberTransmission = 0x03;                            // 'numberTransmission' recebe um valor inteiro 
    Wire.beginTransmission(SlaveAdress);                  // Inicia a transmissão de dados i2c para o endereço 0x08
    Wire.write(numberTransmission);                       // Envia o valor contido em 'numberTransmission'
    Wire.endTransmission();                               // Encerra a comunicação
    numberTransmission = 0x00;                            // Reseta a variável
  
  }                                                       // Fim do if
  
  else{                                                   // Se não
      numberTransmission = 0x04;                          // 'numberTransmission' recebe um valor inteiro
      Wire.beginTransmission(SlaveAdress);                // Inicia a transmissão de dados i2c para o endereço 0x08
      Wire.write(numberTransmission);                     // Envia o valor contido em 'numberTransmission'
      Wire.endTransmission();                             // Encerra a comunicação
      numberTransmission = 0x00;                          // Reseta a variável
      
      }                                                   // Fim do else
      
  }                                                       // Fim da rotina

//====================================================================================================================

// Rotina para seccionar um circuito da sala de estar
void seEntrada(uint8_t brightness){

  if(brightness){                                         // se pedir para alexa ligar
    numberTransmission = 0x05;                            // 'numberTransmission' recebe um valor inteiro 
    Wire.beginTransmission(SlaveAdress);                  // Inicia a transmissão de dados i2c para o endereço 0x08
    Wire.write(numberTransmission);                       // Envia o valor contido em 'numberTransmission'
    Wire.endTransmission();                               // Encerra a comunicação
    numberTransmission = 0x00;                            // Reseta a variável
  
  }                                                       // Fim do if
  
  else{                                                   // Se não
      numberTransmission = 0x06;                          // 'numberTransmission' recebe um valor inteiro
      Wire.beginTransmission(SlaveAdress);                // Inicia a transmissão de dados i2c para o endereço 0x08
      Wire.write(numberTransmission);                     // Envia o valor contido em 'numberTransmission'
      Wire.endTransmission();                             // Encerra a comunicação
      numberTransmission = 0x00;                          // Reseta a variável
      
      }                                                   // Fim do else
      
  }                                                       // Fim da rotina

//====================================================================================================================

// Rotina para seccionar um circuito da sala de estar
void seTv(uint8_t brightness){

  if(brightness){                                         // se pedir para alexa ligar
    numberTransmission = 0x07;                            // 'numberTransmission' recebe um valor inteiro 
    Wire.beginTransmission(SlaveAdress);                  // Inicia a transmissão de dados i2c para o endereço 0x08
    Wire.write(numberTransmission);                       // Envia o valor contido em 'numberTransmission'
    Wire.endTransmission();                               // Encerra a comunicação
    numberTransmission = 0x00;                            // Reseta a variável
  
  }                                                       // Fim do if
  
  else{                                                   // Se não
      numberTransmission = 0x08;                          // 'numberTransmission' recebe um valor inteiro
      Wire.beginTransmission(SlaveAdress);                // Inicia a transmissão de dados i2c para o endereço 0x08
      Wire.write(numberTransmission);                     // Envia o valor contido em 'numberTransmission'
      Wire.endTransmission();                             // Encerra a comunicação
      numberTransmission = 0x00;                          // Reseta a variável
      
      }                                                   // Fim do else
      
  }                                                       // Fim da rotina

//====================================================================================================================

// Rotina para seccionar um circuito do banheiro social
void banheiroSocial(uint8_t brightness){

  if(brightness){                                         // se pedir para alexa ligar
    numberTransmission = 0x09;                            // 'numberTransmission' recebe um valor inteiro 
    Wire.beginTransmission(SlaveAdress);                  // Inicia a transmissão de dados i2c para o endereço 0x08
    Wire.write(numberTransmission);                       // Envia o valor contido em 'numberTransmission'
    Wire.endTransmission();                               // Encerra a comunicação
    numberTransmission = 0x00;                            // Reseta a variável
  
  }                                                       // Fim do if
  
  else{                                                   // Se não
      numberTransmission = 0x0A;                          // 'numberTransmission' recebe um valor inteiro
      Wire.beginTransmission(SlaveAdress);                // Inicia a transmissão de dados i2c para o endereço 0x08
      Wire.write(numberTransmission);                     // Envia o valor contido em 'numberTransmission'
      Wire.endTransmission();                             // Encerra a comunicação
      numberTransmission = 0x00;                          // Reseta a variável
      
      }                                                   // Fim do else
      
  }                                                       // Fim da rotina

//====================================================================================================================

// Rotina para seccionar um circuito da sala de jantar
void sjPendente(uint8_t brightness){

  if(brightness){                                         // se pedir para alexa ligar
    numberTransmission = 0x0D;                            // 'numberTransmission' recebe um valor inteiro 
    Wire.beginTransmission(SlaveAdress);                  // Inicia a transmissão de dados i2c para o endereço 0x08
    Wire.write(numberTransmission);                       // Envia o valor contido em 'numberTransmission'
    Wire.endTransmission();                               // Encerra a comunicação
    numberTransmission = 0x00;                            // Reseta a variável
  
  }                                                       // Fim do if
  
  else{                                                   // Se não
      numberTransmission = 0x0E;                          // 'numberTransmission' recebe um valor inteiro
      Wire.beginTransmission(SlaveAdress);                // Inicia a transmissão de dados i2c para o endereço 0x08
      Wire.write(numberTransmission);                     // Envia o valor contido em 'numberTransmission'
      Wire.endTransmission();                             // Encerra a comunicação
      numberTransmission = 0x00;                          // Reseta a variável
      
      }                                                   // Fim do else
      
  }                                                       // Fim da rotina

//====================================================================================================================

// Rotina para seccionar um circuito da sala de jantar
void sjLustre(uint8_t brightness){

  if(brightness){                                         // se pedir para alexa ligar
    numberTransmission = 0x0B;                            // 'numberTransmission' recebe um valor inteiro 
    Wire.beginTransmission(SlaveAdress);                  // Inicia a transmissão de dados i2c para o endereço 0x08
    Wire.write(numberTransmission);                       // Envia o valor contido em 'numberTransmission'
    Wire.endTransmission();                               // Encerra a comunicação
    numberTransmission = 0x00;                            // Reseta a variável
  
  }                                                       // Fim do if
  
  else{                                                   // Se não
      numberTransmission = 0x0C;                          // 'numberTransmission' recebe um valor inteiro
      Wire.beginTransmission(SlaveAdress);                // Inicia a transmissão de dados i2c para o endereço 0x08
      Wire.write(numberTransmission);                     // Envia o valor contido em 'numberTransmission'
      Wire.endTransmission();                             // Encerra a comunicação
      numberTransmission = 0x00;                          // Reseta a variável
      
      }                                                   // Fim do else
      
  }                                                       // Fim da rotina

//====================================================================================================================

// Rotina para seccionar um circuito da cozinha
void cozinhaPia(uint8_t brightness){

  if(brightness){                                         // se pedir para alexa ligar
    numberTransmission = 0x0F;                            // 'numberTransmission' recebe um valor inteiro 
    Wire.beginTransmission(SlaveAdress);                  // Inicia a transmissão de dados i2c para o endereço 0x08
    Wire.write(numberTransmission);                       // Envia o valor contido em 'numberTransmission'
    Wire.endTransmission();                               // Encerra a comunicação
    numberTransmission = 0x00;                            // Reseta a variável
  
  }                                                       // Fim do if
  
  else{                                                   // Se não
      numberTransmission = 0x10;                          // 'numberTransmission' recebe um valor inteiro
      Wire.beginTransmission(SlaveAdress);                // Inicia a transmissão de dados i2c para o endereço 0x08
      Wire.write(numberTransmission);                     // Envia o valor contido em 'numberTransmission'
      Wire.endTransmission();                             // Encerra a comunicação
      numberTransmission = 0x00;                          // Reseta a variável
      
      }                                                   // Fim do else
      
  }                                                       // Fim da rotina

//====================================================================================================================

// Rotina para seccionar um circuito da cozinha
void cozinhaBalcao(uint8_t brightness){

  if(brightness){                                         // se pedir para alexa ligar
    numberTransmission = 0x11;                            // 'numberTransmission' recebe um valor inteiro 
    Wire.beginTransmission(SlaveAdress);                  // Inicia a transmissão de dados i2c para o endereço 0x08
    Wire.write(numberTransmission);                       // Envia o valor contido em 'numberTransmission'
    Wire.endTransmission();                               // Encerra a comunicação
    numberTransmission = 0x00;                            // Reseta a variável
  
  }                                                       // Fim do if
  
  else{                                                   // Se não
      numberTransmission = 0x12;                          // 'numberTransmission' recebe um valor inteiro
      Wire.beginTransmission(SlaveAdress);                // Inicia a transmissão de dados i2c para o endereço 0x08
      Wire.write(numberTransmission);                     // Envia o valor contido em 'numberTransmission'
      Wire.endTransmission();                             // Encerra a comunicação
      numberTransmission = 0x00;                          // Reseta a variável
      
      }                                                   // Fim do else
      
  }                                                       // Fim da rotina

//====================================================================================================================

// Rotina para seccionar um circuito do hall intimo
void hall(uint8_t brightness){

  if(brightness){                                         // se pedir para alexa ligar
    numberTransmission = 0x13;                            // 'numberTransmission' recebe um valor inteiro 
    Wire.beginTransmission(SlaveAdress);                  // Inicia a transmissão de dados i2c para o endereço 0x08
    Wire.write(numberTransmission);                       // Envia o valor contido em 'numberTransmission'
    Wire.endTransmission();                               // Encerra a comunicação
    numberTransmission = 0x00;                            // Reseta a variável
  
  }                                                       // Fim do if
  
  else{                                                   // Se não
      numberTransmission = 0x14;                          // 'numberTransmission' recebe um valor inteiro
      Wire.beginTransmission(SlaveAdress);                // Inicia a transmissão de dados i2c para o endereço 0x08
      Wire.write(numberTransmission);                     // Envia o valor contido em 'numberTransmission'
      Wire.endTransmission();                             // Encerra a comunicação
      numberTransmission = 0x00;                          // Reseta a variável
      
      }                                                   // Fim do else
      
  }                                                       // Fim da rotina

//====================================================================================================================

// Rotina para seccionar um circuito do jardim
void jardimLateral(uint8_t brightness){

  if(brightness){                                         // se pedir para alexa ligar
    numberTransmission = 0x15;                            // 'numberTransmission' recebe um valor inteiro 
    Wire.beginTransmission(SlaveAdress);                  // Inicia a transmissão de dados i2c para o endereço 0x08
    Wire.write(numberTransmission);                       // Envia o valor contido em 'numberTransmission'
    Wire.endTransmission();                               // Encerra a comunicação
    numberTransmission = 0x00;                            // Reseta a variável
  
  }                                                       // Fim do if
  
  else{                                                   // Se não
      numberTransmission = 0x16;                          // 'numberTransmission' recebe um valor inteiro
      Wire.beginTransmission(SlaveAdress);                // Inicia a transmissão de dados i2c para o endereço 0x08
      Wire.write(numberTransmission);                     // Envia o valor contido em 'numberTransmission'
      Wire.endTransmission();                             // Encerra a comunicação
      numberTransmission = 0x00;                          // Reseta a variável
      
      }                                                   // Fim do else
      
  }                                                       // Fim da rotina

//====================================================================================================================

// Rotina para seccionar um circuito da suite 1
void s1WcChuveiro(uint8_t brightness){

  if(brightness){                                         // se pedir para alexa ligar
    numberTransmission = 0x19;                            // 'numberTransmission' recebe um valor inteiro 
    Wire.beginTransmission(SlaveAdress);                  // Inicia a transmissão de dados i2c para o endereço 0x08
    Wire.write(numberTransmission);                       // Envia o valor contido em 'numberTransmission'
    Wire.endTransmission();                               // Encerra a comunicação
    numberTransmission = 0x00;                            // Reseta a variável
  
  }                                                       // Fim do if
  
  else{                                                   // Se não
      numberTransmission = 0x1A;                          // 'numberTransmission' recebe um valor inteiro
      Wire.beginTransmission(SlaveAdress);                // Inicia a transmissão de dados i2c para o endereço 0x08
      Wire.write(numberTransmission);                     // Envia o valor contido em 'numberTransmission'
      Wire.endTransmission();                             // Encerra a comunicação
      numberTransmission = 0x00;                          // Reseta a variável
      
      }                                                   // Fim do else
      
  }                                                       // Fim da rotina

//====================================================================================================================

// Rotina para seccionar um circuito da suite 1
void s1WcPia(uint8_t brightness){

  if(brightness){                                         // se pedir para alexa ligar
    numberTransmission = 0x1B;                            // 'numberTransmission' recebe um valor inteiro 
    Wire.beginTransmission(SlaveAdress);                  // Inicia a transmissão de dados i2c para o endereço 0x08
    Wire.write(numberTransmission);                       // Envia o valor contido em 'numberTransmission'
    Wire.endTransmission();                               // Encerra a comunicação
    numberTransmission = 0x00;                            // Reseta a variável
  
  }                                                       // Fim do if
  
  else{                                                   // Se não
      numberTransmission = 0x1C;                          // 'numberTransmission' recebe um valor inteiro
      Wire.beginTransmission(SlaveAdress);                // Inicia a transmissão de dados i2c para o endereço 0x08
      Wire.write(numberTransmission);                     // Envia o valor contido em 'numberTransmission'
      Wire.endTransmission();                             // Encerra a comunicação
      numberTransmission = 0x00;                          // Reseta a variável
      
      }                                                   // Fim do else
      
  }                                                       // Fim da rotina

//====================================================================================================================

// Rotina para seccionar um circuito da sala de estar
void s1Tv(uint8_t brightness){

  if(brightness){                                         // se pedir para alexa ligar
    numberTransmission = 0x1D;                            // 'numberTransmission' recebe um valor inteiro 
    Wire.beginTransmission(SlaveAdress);                  // Inicia a transmissão de dados i2c para o endereço 0x08
    Wire.write(numberTransmission);                       // Envia o valor contido em 'numberTransmission'
    Wire.endTransmission();                               // Encerra a comunicação
    numberTransmission = 0x00;                            // Reseta a variável
  
  }                                                       // Fim do if
  
  else{                                                   // Se não
      numberTransmission = 0x1E;                          // 'numberTransmission' recebe um valor inteiro
      Wire.beginTransmission(SlaveAdress);                // Inicia a transmissão de dados i2c para o endereço 0x08
      Wire.write(numberTransmission);                     // Envia o valor contido em 'numberTransmission'
      Wire.endTransmission();                             // Encerra a comunicação
      numberTransmission = 0x00;                          // Reseta a variável
      
      }                                                   // Fim do else
      
  }                                                       // Fim da rotina

//====================================================================================================================

// Rotina para seccionar um circuito da suite 1
void s1Cama(uint8_t brightness){

  if(brightness){                                         // se pedir para alexa ligar
    numberTransmission = 0x1F;                            // 'numberTransmission' recebe um valor inteiro 
    Wire.beginTransmission(SlaveAdress);                  // Inicia a transmissão de dados i2c para o endereço 0x08
    Wire.write(numberTransmission);                       // Envia o valor contido em 'numberTransmission'
    Wire.endTransmission();                               // Encerra a comunicação
    numberTransmission = 0x00;                            // Reseta a variável
  
  }                                                       // Fim do if
  
  else{                                                   // Se não
      numberTransmission = 0x20;                          // 'numberTransmission' recebe um valor inteiro
      Wire.beginTransmission(SlaveAdress);                // Inicia a transmissão de dados i2c para o endereço 0x08
      Wire.write(numberTransmission);                     // Envia o valor contido em 'numberTransmission'
      Wire.endTransmission();                             // Encerra a comunicação
      numberTransmission = 0x00;                          // Reseta a variável
      
      }                                                   // Fim do else
      
  }                                                       // Fim da rotina

//====================================================================================================================

// Rotina para seccionar um circuito da suite 1
void s1Armario(uint8_t brightness){

  if(brightness){                                         // se pedir para alexa ligar
    numberTransmission = 0x17;                            // 'numberTransmission' recebe um valor inteiro 
    Wire.beginTransmission(SlaveAdress);                  // Inicia a transmissão de dados i2c para o endereço 0x08
    Wire.write(numberTransmission);                       // Envia o valor contido em 'numberTransmission'
    Wire.endTransmission();                               // Encerra a comunicação
    numberTransmission = 0x00;                            // Reseta a variável
  
  }                                                       // Fim do if
  
  else{                                                   // Se não
      numberTransmission = 0x18;                          // 'numberTransmission' recebe um valor inteiro
      Wire.beginTransmission(SlaveAdress);                // Inicia a transmissão de dados i2c para o endereço 0x08
      Wire.write(numberTransmission);                     // Envia o valor contido em 'numberTransmission'
      Wire.endTransmission();                             // Encerra a comunicação
      numberTransmission = 0x00;                          // Reseta a variável
      
      }                                                   // Fim do else
      
  }                                                       // Fim da rotina

//====================================================================================================================

// Rotina para seccionar um circuito da suite master
void smWcPia(uint8_t brightness){

  if(brightness){                                         // se pedir para alexa ligar
    numberTransmission = 0x21;                            // 'numberTransmission' recebe um valor inteiro 
    Wire.beginTransmission(SlaveAdress);                  // Inicia a transmissão de dados i2c para o endereço 0x08
    Wire.write(numberTransmission);                       // Envia o valor contido em 'numberTransmission'
    Wire.endTransmission();                               // Encerra a comunicação
    numberTransmission = 0x00;                            // Reseta a variável
  
  }                                                       // Fim do if
  
  else{                                                   // Se não
      numberTransmission = 0x22;                          // 'numberTransmission' recebe um valor inteiro
      Wire.beginTransmission(SlaveAdress);                // Inicia a transmissão de dados i2c para o endereço 0x08
      Wire.write(numberTransmission);                     // Envia o valor contido em 'numberTransmission'
      Wire.endTransmission();                             // Encerra a comunicação
      numberTransmission = 0x00;                          // Reseta a variável
      
      }                                                   // Fim do else
      
  }                                                       // Fim da rotina

//====================================================================================================================

// Rotina para seccionar um circuito da suite master
void smWcChuveiro(uint8_t brightness){

  if(brightness){                                         // se pedir para alexa ligar
    numberTransmission = 0x23;                            // 'numberTransmission' recebe um valor inteiro 
    Wire.beginTransmission(SlaveAdress);                  // Inicia a transmissão de dados i2c para o endereço 0x08
    Wire.write(numberTransmission);                       // Envia o valor contido em 'numberTransmission'
    Wire.endTransmission();                               // Encerra a comunicação
    numberTransmission = 0x00;                            // Reseta a variável
  
  }                                                       // Fim do if
  
  else{                                                   // Se não
      numberTransmission = 0x24;                          // 'numberTransmission' recebe um valor inteiro
      Wire.beginTransmission(SlaveAdress);                // Inicia a transmissão de dados i2c para o endereço 0x08
      Wire.write(numberTransmission);                     // Envia o valor contido em 'numberTransmission'
      Wire.endTransmission();                             // Encerra a comunicação
      numberTransmission = 0x00;                          // Reseta a variável
      
      }                                                   // Fim do else
      
  }                                                       // Fim da rotina

//====================================================================================================================

// Rotina para seccionar um circuito da suite master
void smCloset(uint8_t brightness){

  if(brightness){                                         // se pedir para alexa ligar
    numberTransmission = 0x25;                            // 'numberTransmission' recebe um valor inteiro 
    Wire.beginTransmission(SlaveAdress);                  // Inicia a transmissão de dados i2c para o endereço 0x08
    Wire.write(numberTransmission);                       // Envia o valor contido em 'numberTransmission'
    Wire.endTransmission();                               // Encerra a comunicação
    numberTransmission = 0x00;                            // Reseta a variável
  
  }                                                       // Fim do if
  
  else{                                                   // Se não
      numberTransmission = 0x26;                          // 'numberTransmission' recebe um valor inteiro
      Wire.beginTransmission(SlaveAdress);                // Inicia a transmissão de dados i2c para o endereço 0x08
      Wire.write(numberTransmission);                     // Envia o valor contido em 'numberTransmission'
      Wire.endTransmission();                             // Encerra a comunicação
      numberTransmission = 0x00;                          // Reseta a variável
      
      }                                                   // Fim do else
      
  }                                                       // Fim da rotina

//====================================================================================================================

// Rotina para seccionar um circuito da suite master
void smCama(uint8_t brightness){

  if(brightness){                                         // se pedir para alexa ligar
    numberTransmission = 0x27;                            // 'numberTransmission' recebe um valor inteiro 
    Wire.beginTransmission(SlaveAdress);                  // Inicia a transmissão de dados i2c para o endereço 0x08
    Wire.write(numberTransmission);                       // Envia o valor contido em 'numberTransmission'
    Wire.endTransmission();                               // Encerra a comunicação
    numberTransmission = 0x00;                            // Reseta a variável
  
  }                                                       // Fim do if
  
  else{                                                   // Se não
      numberTransmission = 0x28;                          // 'numberTransmission' recebe um valor inteiro
      Wire.beginTransmission(SlaveAdress);                // Inicia a transmissão de dados i2c para o endereço 0x08
      Wire.write(numberTransmission);                     // Envia o valor contido em 'numberTransmission'
      Wire.endTransmission();                             // Encerra a comunicação
      numberTransmission = 0x00;                          // Reseta a variável
      
      }                                                   // Fim do else
      
  }                                                       // Fim da rotina

//====================================================================================================================

// Rotina para seccionar um circuito da suite master
void smEntrada(uint8_t brightness){

  if(brightness){                                         // se pedir para alexa ligar
    numberTransmission = 0x29;                            // 'numberTransmission' recebe um valor inteiro 
    Wire.beginTransmission(SlaveAdress);                  // Inicia a transmissão de dados i2c para o endereço 0x08
    Wire.write(numberTransmission);                       // Envia o valor contido em 'numberTransmission'
    Wire.endTransmission();                               // Encerra a comunicação
    numberTransmission = 0x00;                            // Reseta a variável
  
  }                                                       // Fim do if
  
  else{                                                   // Se não
      numberTransmission = 0x2A;                          // 'numberTransmission' recebe um valor inteiro
      Wire.beginTransmission(SlaveAdress);                // Inicia a transmissão de dados i2c para o endereço 0x08
      Wire.write(numberTransmission);                     // Envia o valor contido em 'numberTransmission'
      Wire.endTransmission();                             // Encerra a comunicação
      numberTransmission = 0x00;                          // Reseta a variável
      
      }                                                   // Fim do else
      
  }                                                       // Fim da rotina

//====================================================================================================================

// Rotina para seccionar um circuito da suite master
void smTv(uint8_t brightness){

  if(brightness){                                         // se pedir para alexa ligar
    numberTransmission = 0x2B;                            // 'numberTransmission' recebe um valor inteiro 
    Wire.beginTransmission(SlaveAdress);                  // Inicia a transmissão de dados i2c para o endereço 0x08
    Wire.write(numberTransmission);                       // Envia o valor contido em 'numberTransmission'
    Wire.endTransmission();                               // Encerra a comunicação
    numberTransmission = 0x00;                            // Reseta a variável
  
  }                                                       // Fim do if
  
  else{                                                   // Se não
      numberTransmission = 0x2C;                          // 'numberTransmission' recebe um valor inteiro
      Wire.beginTransmission(SlaveAdress);                // Inicia a transmissão de dados i2c para o endereço 0x08
      Wire.write(numberTransmission);                     // Envia o valor contido em 'numberTransmission'
      Wire.endTransmission();                             // Encerra a comunicação
      numberTransmission = 0x00;                          // Reseta a variável
      
      }                                                   // Fim do else
      
  }                                                       // Fim da rotina

//====================================================================================================================

// Rotina para seccionar um circuito do jardim
void jardimEntrada(uint8_t brightness){

  if(brightness){                                         // se pedir para alexa ligar
    numberTransmission = 0x2D;                            // 'numberTransmission' recebe um valor inteiro 
    Wire.beginTransmission(SlaveAdress);                  // Inicia a transmissão de dados i2c para o endereço 0x08
    Wire.write(numberTransmission);                       // Envia o valor contido em 'numberTransmission'
    Wire.endTransmission();                               // Encerra a comunicação
    numberTransmission = 0x00;                            // Reseta a variável
  
  }                                                       // Fim do if
  
  else{                                                   // Se não
      numberTransmission = 0x2E;                          // 'numberTransmission' recebe um valor inteiro
      Wire.beginTransmission(SlaveAdress);                // Inicia a transmissão de dados i2c para o endereço 0x08
      Wire.write(numberTransmission);                     // Envia o valor contido em 'numberTransmission'
      Wire.endTransmission();                             // Encerra a comunicação
      numberTransmission = 0x00;                          // Reseta a variável
      
      }                                                   // Fim do else
      
  }                                                       // Fim da rotina

//====================================================================================================================

// connect to wifi – returns true if successful or false if not
boolean connectWifi(){
  boolean state = true;
  int i = 0;
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.println("Connecting to WiFi");

  // Wait for connection
  Serial.print("Connecting...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (i > 20){
      state = false; break;
    }
    i++;
  }
  Serial.println("");
  if (state){
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
  else {
    Serial.println("Connection failed.");
  }
  return state;
}
