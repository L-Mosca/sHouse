/* Software para arduino mega2560 rev3 para apresentação do trabalho de graduação

Autoria: Lucas Mosca
         Luigi Cavalieri

         Julho de 2021

===============================================================================================================================

Hardware:

Saídas:

 - Led branco de alto brilho - 3.1V ~ 3.4V, ligado em current sourcing:
    '1' - Led Acionado
    '0' - Led Desacionado
                                                   |
                  62R                              ||
    MCU ----------WWWW------------|>|--------------|||  -5VCC
                                                   ||
                                                   |

 - Resistores:
    - Resistores de 62R  -> resistores LEDs

===============================================================================================================================
 
Entradas:

 Botões pulsadores ligados na configuração pull up:
  '0' - Botão pressionado
  '1' - Botão liberado
 
 
                                MCU                                       
                                 0                                       
                    Sx           |          Rx 10K           |           
                   ____          |         _________         ||                        
+ 5VCC ___________0    0_________|________|         |________|||- 5VCC                                                            
                                          |_________|        ||                    
                                                             |           
 
 
=============================================================================================================================== 

 Outros componentes e ciruitos embarcados:
 
 - Arduino Mega SDK -> i2c slave
 - ESP 8266 NodeMCU 12E -> i2c master


===============================================================================================================================

Estrutura do Software:

 - Inclusão de bibliotecas
 - Definição de labels para elementos estáticos
 - Mapeamento de hardware -> Define labels para tratar dos terminais da placa
 - Declaração de variáveis globais
 - Declaração das sub-rotinas
 - Setup  -> Inicia o protocolo de comunicação i2c, define a execução em caso de recebimento de dados e configura os terminais da placa como entradas ou saídas
 - Loop   -> Delay para atualização do uC

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

Circuito 12: L22 -> Suíte 1 Cama
Circuito 13: L23 -> Suíte 1 WC Chuveiro
Circuito 14: L24 -> Suíte 1 WC Pia
Circuito 15: L25 -> Suíte 1 TV
Circuito 16: L26 -> Suíte 1 Armário

Circuito 17: L27 -> Suíte Master WC Pia
Circuito 18: L28 -> Suíte Master WC Chuveiro
Curcuito 19: L30, L31 -> Suíte Master Closet
Circuito 20: L29, L33 -> Suíte Master Cama
Circuito 21: L34 -> Suíte Master Entrada
Circuito 22: L32 -> Suíte Master TV

Circuito 23: L35, L36, L37, L38, L39 -> Jardim de Entrada 

===============================================================================================================================
*/

// --- Bibliotecas Inclusas no Projeto ---

#include <Wire.h>                                       // Biblioteca para utilização do protocolo de comunicação I2C


// --- Definição de Elementos Estáticos ---
#define MyAdress 0x08                                   // Endereço da placa no barramento i2c = 8

//====================================================================================================================================

// --- Mapeamento de Hardware ---

// Elementos da Maquete
// Terminais com led conectados
#define   L1     22                                      // Define 'L1'  conectado ao terminal 22 da placa
#define   L2     23                                      // Define 'L2'  conectado ao terminal 23 da placa
#define   L3     24                                      // Define 'L3'  conectado ao terminal 24 da placa
#define   L4     25                                      // Define 'L4'  conectado ao terminal 25 da placa
#define   L5     26                                      // Define 'L5'  conectado ao terminal 26 da placa
#define   L6     27                                      // Define 'L6'  conectado ao terminal 27 da placa
#define   L7     28                                      // Define 'L7'  conectado ao terminal 28 da placa
#define   L8     29                                      // Define 'L8'  conectado ao terminal 29 da placa
#define   L9     30                                      // Define 'L9'  conectado ao terminal 30 da placa
#define   L10    31                                      // Define 'L10' conectado ao terminal 31 da placa  
#define   L11    32                                      // Define 'L11' conectado ao terminal 32 da placa
#define   L12    33                                      // Define 'L12' conectado ao terminal 33 da placa
#define   L13    34                                      // Define 'L13' conectado ao terminal 34 da placa
#define   L14    35                                      // Define 'L14' conectado ao terminal 35 da placa
#define   L15    36                                      // Define 'L15' conectado ao terminal 36 da placa
#define   L16    37                                      // Define 'L16' conectado ao terminal 37 da placa
#define   L17    38                                      // Define 'L17' conectado ao terminal 38 da placa
#define   L18    39                                      // Define 'L18' conectado ao terminal 39 da placa
#define   L19    40                                      // Define 'L19' conectado ao terminal 40 da placa
#define   L20    41                                      // Define 'L20' conectado ao terminal 41 da placa
#define   L21    42                                      // Define 'L21' conectado ao terminal 42 da placa
#define   L22    43                                      // Define 'L22' conectado ao terminal 43 da placa
#define   L23    44                                      // Define 'L23' conectado ao terminal 44 da placa
#define   L24    45                                      // Define 'L24' conectado ao terminal 45 da placa
#define   L25    46                                      // Define 'L25' conectado ao terminal 46 da placa
#define   L26    47                                      // Define 'L26' conectado ao terminal 47 da placa
#define   L27    48                                      // Define 'L27' conectado ao terminal 48 da placa
#define   L28    49                                      // Define 'L28' conectado ao terminal 49 da placa
#define   L29    50                                      // Define 'L29' conectado ao terminal 50 da placa
#define   L30    51                                      // Define 'L30' conectado ao terminal 51 da placa
#define   L31    52                                      // Define 'L31' conectado ao terminal 52 da placa
#define   L32    53                                      // Define 'L32' conectado ao terminal 53 da placa
#define   L33     2                                      // Define 'L33' conectado ao terminal  2 da placa
#define   L34     3                                      // Define 'L34' conectado ao terminal  3 da placa
#define   L35     4                                      // Define 'L35' conectado ao terminal  4 da placa
#define   L36     5                                      // Define 'L36' conectado ao terminal  5 da placa
#define   L37     6                                      // Define 'L37' conectado ao terminal  6 da placa
#define   L38     7                                      // Define 'L38' conectado ao terminal  7 da placa
#define   L39     8                                      // Define 'L39' conectado ao terminal  8 da placa

// Elementos da Maleta
#define   S1     A0                                     // Define 'S1'  conectado ao terminal  A0 da placa
#define   S2     A1                                     // Define 'S2'  conectado ao terminal  A1 da placa
#define   S3     A2                                     // Define 'S3'  conectado ao terminal  A2 da placa
#define   S4     A3                                     // Define 'S4'  conectado ao terminal  A3 da placa
#define   R1     A4                                     // Define 'R1'  conectado ao terminal  A4 da placa
    
//====================================================================================================================================

// --- Variáveis Globais ---
unsigned int homeControl = 0x00;                         // Variável responsável por armazenar os dados recebidos pelo protocolo i2c

//====================================================================================================================================

// --- Declaração das sub-rotinas ---

// Cenas
void receiveEvent(int howMany);

void masterOn();                                          // Rotina de execução da cena Master ON
void masterOff();                                         // Rotina de execução da cena Master OFF

void ferias();                                            // Rotina de execução da cena Férias

void dormir();                                            // Rotina para desligar todos os circuitos da casa exceto os quartos e hall

// Teste de entradas
void inputTest();                                         // Rotina que executa os testes das entradas
void s1Test();                                            // Rotina que teste a entrada 'S1'  
void s2Test();                                            // Rotina que testa a entrada 'S2'
void s3Test();                                            // Rotina que testa a entrada 'S3'
void s4Test();                                            // Rotina que testa a entrada 'S4'

//====================================================================================================================================

// Seccionamento dos circuitos de iluminação
void circuit1On  ();                                      // Rotina para acionar    o circuito 1
void circuit1Off ();                                      // Rotina para desacionar o circuito 1

void circuit2On  ();                                      // Rotina para acionar    o circuito 2
void circuit2Off ();                                      // Rotina para desacionar o circuito 2

void circuit3On  ();                                      // Rotina para acionar    o circuito 3
void circuit3Off ();                                      // Rotina para desacionar o circuito 3

void circuit4On  ();                                      // Rotina para acionar    o circuito 4
void circuit4Off ();                                      // Rotina para desacionar o circuito 4

void circuit5On  ();                                      // Rotina para acionar    o circuito 5
void circuit5Off ();                                      // Rotina para desacionar o circuito 5

void circuit6On  ();                                      // Rotina para acionar    o circuito 6
void circuit6Off ();                                      // Rotina para desacionar o circuito 6

void circuit7On  ();                                      // Rotina para acionar    o circuito 7
void circuit7Off ();                                      // Rotina para desacionar o circuito 7

void circuit8On  ();                                      // Rotina para acionar    o circuito 8
void circuit8Off ();                                      // Rotina para desacionar o circuito 8

void circuit9On  ();                                      // Rotina para acionar    o circuito 9
void circuit9Off ();                                      // Rotina para desacionar o circuito 9

void circuit10On ();                                      // Rotina para acionar    o circuito 10
void circuit10Off();                                      // Rotina para desacionar o circuito 10

void circuit11On ();                                      // Rotina para acionar    o circuito 11
void circuit11Off();                                      // Rotina para desacionar o circuito 11

void circuit12On ();                                      // Rotina para acionar    o circuito 12
void circuit12Off();                                      // Rotina para desacionar o circuito 12

void circuit13On ();                                      // Rotina para acionar    o circuito 13
void circuit13Off();                                      // Rotina para desacionar o circuito 13

void circuit14On ();                                      // Rotina para acionar    o circuito 14
void circuit14Off();                                      // Rotina para desacionar o circuito 14

void circuit15On ();                                      // Rotina para acionar    o circuito 15
void circuit15Off();                                      // Rotina para desacionar o circuito 15

void circuit16On ();                                      // Rotina para acionar    o circuito 16
void circuit16Off();                                      // Rotina para desacionar o circuito 16

void circuit17On ();                                      // Rotina para acionar    o circuito 17
void circuit17Off();                                      // Rotina para desacionar o circuito 17

void circuit18On ();                                      // Rotina para acionar    o circuito 18
void circuit18Off();                                      // Rotina para desacionar o circuito 18

void circuit19On ();                                      // Rotina para acionar    o circuito 19
void circuit19Off();                                      // Rotina para desacionar o circuito 19

void circuit20On ();                                      // Rotina para acionar    o circuito 20
void circuit20Off();                                      // Rotina para desacionar o circuito 20

void circuit21On ();                                      // Rotina para acionar    o circuito 21
void circuit21Off();                                      // Rotina para desacionar o circuito 21

void circuit22On ();                                      // Rotina para acionar    o circuito 22
void circuit22Off();                                      // Rotina para desacionar o circuito 22

void circuit23On ();                                      // Rotina para acionar    o circuito 23
void circuit23Off();                                      // Rotina para desacionar o circuito 23

//====================================================================================================================================

void setup() {
  
  // Protocolo de comunicação
  Wire.begin(MyAdress);                                   // Inicia o protocolo de comunicação com o endereço 0x08
  Wire.onReceive (receiveEvent);                          // Define o que será executado ao receber um dado
  Serial.begin (9600);                                    // Baud rate

//====================================================================================================================================  
  
  // --- Configuração de Hardware ---

  // Saídas:
  pinMode (L1,  OUTPUT);                                   // Configura o terminal 'L1'  como saída
  pinMode (L2,  OUTPUT);                                   // Configura o terminal 'L2'  como saída
  pinMode (L3,  OUTPUT);                                   // Configura o terminal 'L3'  como saída
  pinMode (L4,  OUTPUT);                                   // Configura o terminal 'L4'  como saída
  pinMode (L5,  OUTPUT);                                   // Configura o terminal 'L5'  como saída
  pinMode (L6,  OUTPUT);                                   // Configura o terminal 'L6'  como saída
  pinMode (L7,  OUTPUT);                                   // Configura o terminal 'L7'  como saída
  pinMode (L8,  OUTPUT);                                   // Configura o terminal 'L8'  como saída
  pinMode (L9,  OUTPUT);                                   // Configura o terminal 'L9'  como saída
  pinMode (L10, OUTPUT);                                   // Configura o terminal 'L10' como saída
  pinMode (L11, OUTPUT);                                   // Configura o terminal 'L11' como saída
  pinMode (L12, OUTPUT);                                   // Configura o terminal 'L12' como saída
  pinMode (L13, OUTPUT);                                   // Configura o terminal 'L13' como saída
  pinMode (L14, OUTPUT);                                   // Configura o terminal 'L14' como saída
  pinMode (L15, OUTPUT);                                   // Configura o terminal 'L15' como saída
  pinMode (L16, OUTPUT);                                   // Configura o terminal 'L16' como saída
  pinMode (L17, OUTPUT);                                   // Configura o terminal 'L17' como saída
  pinMode (L18, OUTPUT);                                   // Configura o terminal 'L18' como saída
  pinMode (L19, OUTPUT);                                   // Configura o terminal 'L19' como saída
  pinMode (L20, OUTPUT);                                   // Configura o terminal 'L20' como saída
  pinMode (L21, OUTPUT);                                   // Configura o terminal 'L21' como saída
  pinMode (L22, OUTPUT);                                   // Configura o terminal 'L22' como saída
  pinMode (L23, OUTPUT);                                   // Configura o terminal 'L23' como saída
  pinMode (L24, OUTPUT);                                   // Configura o terminal 'L24' como saída
  pinMode (L25, OUTPUT);                                   // Configura o terminal 'L25' como saída
  pinMode (L26, OUTPUT);                                   // Configura o terminal 'L26' como saída
  pinMode (L27, OUTPUT);                                   // Configura o terminal 'L27' como saída
  pinMode (L28, OUTPUT);                                   // Configura o terminal 'L28' como saída
  pinMode (L29, OUTPUT);                                   // Configura o terminal 'L29' como saída
  pinMode (L30, OUTPUT);                                   // Configura o terminal 'L30' como saída
  pinMode (L31, OUTPUT);                                   // Configura o terminal 'L31' como saída
  pinMode (L32, OUTPUT);                                   // Configura o terminal 'L32' como saída
  pinMode (L33, OUTPUT);                                   // Configura o terminal 'L33' como saída
  pinMode (L34, OUTPUT);                                   // Configura o terminal 'L34' como saída
  pinMode (L35, OUTPUT);                                   // Configura o terminal 'L35' como saída
  pinMode (L36, OUTPUT);                                   // Configura o terminal 'L36' como saída
  pinMode (L37, OUTPUT);                                   // Configura o terminal 'L37' como saída
  pinMode (L38, OUTPUT);                                   // Configura o terminal 'L38' como saída
  pinMode (L39, OUTPUT);                                   // Configura o terminal 'L39' como saída

  // Entradas e Saídas da Maleta de Apresentação
  pinMode(R1, OUTPUT);                                    // Define o terminal 'R1' como saída
  pinMode(S1, INPUT);                                     // Define o terminal 'S1' como entrada
  pinMode(S2, INPUT);                                     // Define o terminal 'S2' como entrada
  pinMode(S3, INPUT);                                     // Define o terminal 'S3' como entrada
  pinMode(S4, INPUT);                                     // Define o terminal 'S4' como entrada


  digitalWrite(R1, HIGH);
  
}

//====================================================================================================================================

void loop() {  
  inputTest();                                             // Testa as Entradas
}

//====================================================================================================================================

void inputTest(){
    s1Test();                                             // Testa a entrada 's1'
    s2Test();                                             // Testa a entrada 's2'
    s3Test();                                             // Testa a entrada 's3'
    s4Test();                                             // Testa a entrada 's4'
  
}

//====================================================================================================================================

void s1Test(){

  if(!digitalRead(S1)){
    delay(10);
    if(digitalRead(S1))
      masterOn();
  }
}

//====================================================================================================================================

void s2Test(){

  if(!digitalRead(S2)){
    delay(10);
    if(digitalRead(S2))
      masterOff();
  }
}

//====================================================================================================================================

void s3Test(){

  if(!digitalRead(S3)){
    delay(10);
    if(digitalRead(S3))
      dormir();
  }
}

//====================================================================================================================================

void s4Test(){

  if(!digitalRead(S4)){
    delay(10);
    if(digitalRead(S4))
      digitalWrite(R1, !digitalRead(R1));
  }
}

//====================================================================================================================================

void relay1On(){
  digitalWrite(R1, LOW);
}

//====================================================================================================================================

void relay1Off(){
  digitalWrite(R1, HIGH);
}

//====================================================================================================================================


// --- Cenas para Automação ---

// Rotina para acionar todos os circuitos de iluminação
 void masterOn(){

  circuit1On();                                           // Aciona circuito 1
  circuit2On();                                           // Aciona circuito 2
  circuit3On();                                           // Aciona circuito 3
  circuit4On();                                           // Aciona circuito 4
  circuit5On();                                           // Aciona circuito 5
  circuit6On();                                           // Aciona circuito 6
  circuit7On();                                           // Aciona circuito 7
  circuit8On();                                           // Aciona circuito 8
  circuit9On();                                           // Aciona circuito 9
  circuit10On();                                          // Aciona circuito 10
  circuit11On();                                          // Aciona circuito 11
  circuit12On();                                          // Aciona circuito 12
  circuit13On();                                          // Aciona circuito 13
  circuit14On();                                          // Aciona circuito 14
  circuit15On();                                          // Aciona circuito 15
  circuit16On();                                          // Aciona circuito 16
  circuit17On();                                          // Aciona circuito 17
  circuit18On();                                          // Aciona circuito 18
  circuit19On();                                          // Aciona circuito 19
  circuit20On();                                          // Aciona circuito 20
  circuit21On();                                          // Aciona circuito 21
  circuit22On();                                          // Aciona circuito 22
  circuit23On();                                          // Aciona circuito 23
 
}                                                         // Master ON End

//====================================================================================================================================

// Rotina para desacionar todos os circuitos de iluminação
void masterOff(){
    
  circuit1Off ();                                         // Desaciona circuito 1
  circuit2Off ();                                         // Desaciona circuito 2
  circuit3Off ();                                         // Desaciona circuito 3
  circuit4Off ();                                         // Desaciona circuito 4
  circuit5Off ();                                         // Desaciona circuito 5
  circuit6Off ();                                         // Desaciona circuito 6
  circuit7Off ();                                         // Desaciona circuito 7
  circuit8Off ();                                         // Desaciona circuito 8
  circuit9Off ();                                         // Desaciona circuito 9
  circuit10Off();                                         // Desaciona circuito 10
  circuit11Off();                                         // Desaciona circuito 11
  circuit12Off();                                         // Desaciona circuito 12
  circuit13Off();                                         // Desaciona circuito 13
  circuit14Off();                                         // Desaciona circuito 14
  circuit15Off();                                         // Desaciona circuito 15
  circuit16Off();                                         // Desaciona circuito 16
  circuit17Off();                                         // Desaciona circuito 17
  circuit18Off();                                         // Desaciona circuito 18
  circuit19Off();                                         // Desaciona circuito 19
  circuit20Off();                                         // Desaciona circuito 20
  circuit21Off();                                         // Desaciona circuito 21
  circuit22Off();                                         // Desaciona circuito 22
  circuit23Off();                                         // Desaciona circuito 23

}                                                         // Master OFF End

//====================================================================================================================================

// Rorina de manipulação de saídas a partir dos dados recebidos através do protocolo i2c
void receiveEvent(int howMany){

  if(Wire.available())
  {
    
    homeControl = Wire.read();
  }
    switch(homeControl){

      case 0x01:  circuit1On  (); break;                  // Aciona circuito 1
      case 0x02:  circuit1Off (); break;                  // Desaciona circuito 1
      
      case 0x03:  circuit2On  (); break;                  // Aciona circuito 2
      case 0x04:  circuit2Off (); break;                  // Desaciona circuito 2
      
      case 0x05:  circuit3On  (); break;                  // Aciona circuito 3
      case 0x06:  circuit3Off (); break;                  // Desaciona circuito 3
      
      case 0x07:  circuit4On  (); break;                  // Aciona circuito 4
      case 0x08:  circuit4Off (); break;                  // Desaciona circuito 4
      
      case 0x09:  circuit5On  (); break;                  // Aciona circuito 5
      case 0x0A: circuit5Off  (); break;                  // Desaciona circuito 5
      
      case 0x0B: circuit6On   (); break;                  // Aciona circuito 6
      case 0x0C: circuit6Off  (); break;                  // Desaciona circuito 6
      
      case 0x0D: circuit7On   (); break;                  // Aciona circuito 7
      case 0x0E: circuit7Off  (); break;                  // Desaciona circuito 7
      
      case 0x0F: circuit8On   (); break;                  // Aciona circuito 8
      case 0x10: circuit8Off  (); break;                  // Desaciona circuito 8
      
      case 0x11: circuit9On   (); break;                  // Aciona circuito 9
      case 0x12: circuit9Off  (); break;                  // Desaciona circuito 9
      
      case 0x13: circuit10On  (); break;                  // Aciona circuito 10
      case 0x14: circuit10Off (); break;                  // Desaciona circuito 10
      
      case 0x15: circuit11On  (); break;                  // Aciona circuito 11
      case 0x16: circuit11Off (); break;                  // Desaciona circuito 11
      
      case 0x17: circuit12On  (); break;                  // Aciona circuito 12
      case 0x18: circuit12Off (); break;                  // Desaciona circuito 12
      
      case 0x19: circuit13On  (); break;                  // Aciona circuito 13
      case 0x1A: circuit13Off (); break;                  // Desaciona circuito 13
      
      case 0x1B: circuit14On  (); break;                  // Aciona circuito 14
      case 0x1C: circuit14Off (); break;                  // Desaciona circuito 14
      
      case 0x1D: circuit15On  (); break;                  // Aciona circuito 15
      case 0x1E: circuit15Off (); break;                  // Desaciona circuito 15
      
      case 0x1F: circuit16On  (); break;                  // Aciona circuito 16
      case 0x20: circuit16Off (); break;                  // Desaciona circuito 16
      
      case 0x21: circuit17On  (); break;                  // Aciona circuito 17
      case 0x22: circuit17Off (); break;                  // Desaciona circuito 17
      
      case 0x23: circuit18On  (); break;                  // Aciona circuito 18
      case 0x24: circuit18Off (); break;                  // Desaciona circuito 18
      
      case 0x25: circuit19On  (); break;                  // Aciona circuito 19
      case 0x26: circuit19Off (); break;                  // Desaciona circuito 19
      
      case 0x27: circuit20On  (); break;                  // Aciona circuito 20
      case 0x28: circuit20Off (); break;                  // Desaciona circuito 20
      
      case 0x29: circuit21On  (); break;                  // Aciona circuito 21
      case 0x2A: circuit21Off (); break;                  // Desaciona circuito 21
      
      case 0x2B: circuit22On  (); break;                  // Aciona circuito 22
      case 0x2C: circuit22Off (); break;                  // Desaciona circuito 22
      
      case 0x2D: circuit23On  (); break;                  // Aciona circuito 23
      case 0x2E: circuit23Off (); break;                  // Desaciona circuito 23

      case 0x2F: masterOn();   break;                     // Aciona cena masterOn      
      case 0x30: masterOff();  break;                     // Aciona cena masterOff
      case 0x31: dormir();     break;                     // Aciona Cena Dormir

      case 0x32: relay1On(); break;                       // Aciona o Relay 1
      case 0x33: relay1Off(); break;                       // Desaciona o Relay 1

                                                          // Caso receba um dado incompatível
      default: homeControl = 0x00;                        // Reseta a variável
      return;                                             // Sai da função
      
      }                                                   // Fim - Switch case
  
}                                                         // Fim receiveEvent

//====================================================================================================================================

// Rotina para acionar o circuito 1
void circuit1On(){

    digitalWrite(L1, HIGH);                               // Aciona saída 'L1'
    digitalWrite(L2, HIGH);                               // Aciona saída 'L2'
    digitalWrite(L3, HIGH);                               // Aciona saída 'L3'

}                                                         // fim - circuit1On

//====================================================================================================================================

// Rotina para desacionar o circuito 1
void circuit1Off(){

    digitalWrite(L1, LOW);                               // Desaciona saída 'L1'
    digitalWrite(L2, LOW);                               // Desaciona saída 'L2'
    digitalWrite(L3, LOW);                               // Desaciona saída 'L3'

}                                                        // fim - circuit1Off

//====================================================================================================================================

// Rotina para acionar o circuito 2
void circuit2On(){

    digitalWrite(L4, HIGH);                               // Aciona saída 'L4'
    
}                                                         // fim - circuit2On

//====================================================================================================================================

// Rotina para desacionar o circuito 2
void circuit2Off(){

    digitalWrite(L4, LOW);                               // Desaciona saída 'L4'
    
}                                                        // fim - circuit2Off

//====================================================================================================================================

// Rotina para acionar o circuito 3
void circuit3On(){

    digitalWrite(L5, HIGH);                               // Aciona saída 'L5'
    
}                                                         // fim - circuit3On

//====================================================================================================================================

// Rotina para desacionar o circuito 3
void circuit3Off(){

    digitalWrite(L5, LOW);                               // Desaciona saída 'L5'
    
}                                                        // fim - circuit3Off

//====================================================================================================================================

// Rotina para acionar o circuito 4
void circuit4On(){

    digitalWrite(L6, HIGH);                               // Aciona saída 'L6'
    
}                                                         // fim - circuit4On

//====================================================================================================================================

// Rotina para desacionar o circuito 4
void circuit4Off(){

    digitalWrite(L6, LOW);                               // Desaciona saída 'L6'
    
}                                                        // fim - circuit4Off

//====================================================================================================================================

// Rotina para acionar o circuito 5
void circuit5On(){

    digitalWrite(L7, HIGH);                               // Aciona saída 'L7'

}                                                         // fim - circuit5On

//====================================================================================================================================

// Rotina para desacionar o circuito 5
void circuit5Off(){

    digitalWrite(L7, LOW);                               // Desaciona saída 'L7'

}                                                        // fim - circuit5Off

//====================================================================================================================================

// Rotina para acionar o circuito 6
void circuit6On(){

    digitalWrite(L8, HIGH);                               // Aciona saída 'L8'

}                                                         // fim - circuit6On

//====================================================================================================================================

// Rotina para desacionar o circuito 6
void circuit6Off(){

    digitalWrite(L8, LOW);                               // Desaciona saída 'L8'

}                                                        // fim - circuit6Off

//====================================================================================================================================

// Rotina para acionar o circuito 7
void circuit7On(){

    digitalWrite(L9, HIGH);                               // Aciona saída 'L9'

}                                                         // fim - circuit7On

//====================================================================================================================================

// Rotina para desacionar o circuito 7
void circuit7Off(){

    digitalWrite(L9, LOW);                               // Desaciona saída 'L9'

}                                                        // fim - circuit7Off

//====================================================================================================================================

// Rotina para acionar o circuito 8
void circuit8On(){

    digitalWrite(L10, HIGH);                              // Aciona saída 'L10'

}                                                         // fim - circuit8On

//====================================================================================================================================

// Rotina para desacionar o circuito 8
void circuit8Off(){

    digitalWrite(L10, LOW);                              // Desaciona saída 'L10'

}                                                        // fim - circuit8Off

//====================================================================================================================================

// Rotina para acionar o circuito 9
void circuit9On(){

    digitalWrite(L11, HIGH);                              // Aciona saída 'L11'

}                                                         // fim - circuit9On

//====================================================================================================================================

// Rotina para desacionar o circuito 9
void circuit9Off(){

    digitalWrite(L11, LOW);                              // Desaciona saída 'L11

}                                                        // fim - circuit9Off

//====================================================================================================================================

// Rotina para acionar o circuito 10
void circuit10On(){

    digitalWrite(L12, HIGH);                              // Aciona saída 'L12'
    digitalWrite(L13, HIGH);                              // Aciona saída 'L13'
    digitalWrite(L14, HIGH);                              // Aciona saída 'L14'

}                                                         // fim - circuit10On

//====================================================================================================================================

// Rotina para desacionar o circuito 10
void circuit10Off(){

    digitalWrite(L12, LOW);                              // Desaciona saída 'L12'
    digitalWrite(L13, LOW);                              // Desaciona saída 'L13'
    digitalWrite(L14, LOW);                              // Desaciona saída 'L14'

}                                                        // fim - circuit10Off

//====================================================================================================================================

// Rotina para acionar o circuito 11
void circuit11On(){

    digitalWrite(L15, HIGH);                              // Aciona saída 'L15'
    digitalWrite(L16, HIGH);                              // Aciona saída 'L16'
    digitalWrite(L17, HIGH);                              // Aciona saída 'L17'
    digitalWrite(L18, HIGH);                              // Aciona saída 'L18'
    digitalWrite(L19, HIGH);                              // Aciona saída 'L19'
    digitalWrite(L20, HIGH);                              // Aciona saída 'L20'
    digitalWrite(L21, HIGH);                              // Aciona saída 'L21'

}                                                         // fim - circuit11On

//====================================================================================================================================

// Rotina para desacionar o circuito 11
void circuit11Off(){

    digitalWrite(L15, LOW);                              // Desaciona saída 'L15'
    digitalWrite(L16, LOW);                              // Desaciona saída 'L16'
    digitalWrite(L17, LOW);                              // Desaciona saída 'L17'
    digitalWrite(L18, LOW);                              // Desaciona saída 'L18'
    digitalWrite(L19, LOW);                              // Desaciona saída 'L19'
    digitalWrite(L20, LOW);                              // Desaciona saída 'L20'
    digitalWrite(L21, LOW);                              // Desaciona saída 'L21'
    
}                                                        // fim - circuit11Off

//====================================================================================================================================

// Rotina para acionar o circuito 12
void circuit12On(){

    digitalWrite(L22, HIGH);                              // Aciona saída 'L22'

}                                                         // fim - circuit12On

//====================================================================================================================================

// Rotina para desacionar o circuito 12
void circuit12Off(){

    digitalWrite(L22, LOW);                              // Desaciona saída 'L22'

}                                                        // fim - circuit12Off

//====================================================================================================================================

// Rotina para acionar o circuito 13
void circuit13On(){

    digitalWrite(L23, HIGH);                              // Aciona saída 'L23

}                                                         // fim - circuit13On

//====================================================================================================================================

// Rotina para desacionar o circuito 13
void circuit13Off(){

    digitalWrite(L23, LOW);                              // Desaciona saída 'L23'

}                                                        // fim - circuit13Off

//====================================================================================================================================

// Rotina para acionar o circuito 14
void circuit14On(){

    digitalWrite(L24, HIGH);                              // Aciona saída 'L24'

}                                                         // fim - circuit14On

//====================================================================================================================================

// Rotina para desacionar o circuito 14
void circuit14Off(){

    digitalWrite(L24, LOW);                              // Desaciona saída 'L24'

}                                                        // fim - circuit14Off

//====================================================================================================================================

// Rotina para acionar o circuito 15
void circuit15On(){

    digitalWrite(L25, HIGH);                              // Aciona saída 'L25'

}                                                         // fim - circuit15On

//====================================================================================================================================

// Rotina para desacionar o circuito 15
void circuit15Off(){

    digitalWrite(L25, LOW);                              // Desaciona saída 'L25'

}                                                        // fim - circuit15Off

//====================================================================================================================================

// Rotina para acionar o circuito 16
void circuit16On(){

    digitalWrite(L26, HIGH);                              // Aciona saída 'L26'

}                                                         // fim - circuit16On

//====================================================================================================================================

// Rotina para desacionar o circuito 16
void circuit16Off(){

    digitalWrite(L26, LOW);                              // Desaciona saída 'L26'

}                                                        // fim - circuit26Off

//====================================================================================================================================

// Rotina para acionar o circuito 17
void circuit17On(){

    digitalWrite(L27, HIGH);                              // Aciona saída 'L27'

}                                                         // fim - circuit17On

//====================================================================================================================================

// Rotina para desacionar o circuito 17
void circuit17Off(){

    digitalWrite(L27, LOW);                              // Desaciona saída 'L27'

}                                                        // fim - circuit17Off

//====================================================================================================================================

// Rotina para acionar o circuito 18
void circuit18On(){

    digitalWrite(L28, HIGH);                              // Aciona saída 'L28'

}                                                         // fim - circuit18On

//====================================================================================================================================

// Rotina para desacionar o circuito 18
void circuit18Off(){

    digitalWrite(L28, LOW);                              // Desaciona saída 'L28'

}                                                        // fim - circuit18Off

//====================================================================================================================================

// Rotina para acionar o circuito 19
void circuit19On(){

    digitalWrite(L30, HIGH);                              // Aciona saída 'L30'
    digitalWrite(L31, HIGH);                              // Aciona saída 'L31

}                                                         // fim - circuit19On

//====================================================================================================================================

// Rotina para desacionar o circuito 19
void circuit19Off(){

    digitalWrite(L30, LOW);                              // Desaciona saída 'L30'
    digitalWrite(L31, LOW);                              // Desaciona saída 'L31'

}                                                        // fim - circuit19Off

//====================================================================================================================================

// Rotina para acionar o circuito 20
void circuit20On(){

    digitalWrite(L29, HIGH);                              // Aciona saída 'L29'
    digitalWrite(L33, HIGH);                              // Aciona saída 'L33'

}                                                         // fim - circuit20On

//====================================================================================================================================

// Rotina para desacionar o circuito 20
void circuit20Off(){

    digitalWrite(L29, LOW);                              // Desaciona saída 'L29'
    digitalWrite(L33, LOW);                              // Desaciona saída 'L33'

}                                                        // fim - circuit20Off

//====================================================================================================================================

// Rotina para acionar o circuito 21
void circuit21On(){

    digitalWrite(L34, HIGH);                              // Aciona saída 'L34'

}                                                         // fim - circuit21On

//====================================================================================================================================

// Rotina para desacionar o circuito 21
void circuit21Off(){

    digitalWrite(L34, LOW);                              // Desaciona saída 'L34'

}                                                        // fim - circuit21Off

//====================================================================================================================================

// Rotina para acionar o circuito 22
void circuit22On(){

    digitalWrite(L32, HIGH);                              // Aciona saída 'L32'

}                                                         // fim - circuit22On

//====================================================================================================================================

// Rotina para desacionar o circuito 22
void circuit22Off(){

    digitalWrite(L32, LOW);                              // Desaciona saída 'L32'

}                                                        // fim - circuit22Off

//====================================================================================================================================

// Rotina para acionar o circuito 23
void circuit23On(){

    digitalWrite(L35, HIGH);                              // Aciona saída 'L35'
    digitalWrite(L36, HIGH);                              // Aciona saída 'L36'
    digitalWrite(L37, HIGH);                              // Aciona saída 'L37'
    digitalWrite(L38, HIGH);                              // Aciona saída 'L38'
    digitalWrite(L39, HIGH);                              // Aciona saída 'L39'
    
}                                                         // fim - circuit23On

//====================================================================================================================================

// Rotina para desacionar o circuito 23
void circuit23Off(){

    digitalWrite(L35, LOW);                              // Desaciona saída 'L35'
    digitalWrite(L36, LOW);                              // Desaciona saída 'L36'
    digitalWrite(L37, LOW);                              // Desaciona saída 'L37'
    digitalWrite(L38, LOW);                              // Desaciona saída 'L38'
    digitalWrite(L39, LOW);                              // Desaciona saída 'L39'
    
}                                                        // fim - circuit23Off

//====================================================================================================================================

// Rotina que desaciona todos os circuitos da casa, exceto os quartos e o hall íntimo
void dormir(){

  circuit1Off ();                                       // Desaciona circuito 1
  circuit2Off ();                                       // Desaciona circuito 2
  circuit3Off ();                                       // Desaciona circuito 3
  circuit4Off ();                                       // Desaciona circuito 4
  circuit5Off ();                                       // Desaciona circuito 5
  circuit6Off ();                                       // Desaciona circuito 6
  circuit7Off ();                                       // Desaciona circuito 7
  circuit8Off ();                                       // Desaciona circuito 8
  circuit9Off ();                                       // Desaciona circuito 9
  circuit11Off();                                       // Desaciona circuito 11
  
}

//====================================================================================================================================

// Rotina para simular presença na casa
void ferias(){
  
  // simula presença na casa  

}

//====================================================================================================================================

// Rotina para desacionar todos os circuítos da suíte 1
void s1Off(){

  circuit12Off();                                       // Desaciona circuito 12
  circuit13Off();                                       // Desaciona circuito 13
  circuit14Off();                                       // Desaciona circuito 14
  circuit15Off();                                       // Desaciona circuito 15
  circuit16Off();                                       // Desaciona circuito 16

}

//====================================================================================================================================

// Rotina para desacionar todos os circuítos da suíte master
void sMasterOff(){

  circuit17Off();                                       // Desaciona circuito 17
  circuit18Off();                                       // Desaciona circuito 18
  circuit19Off();                                       // Desaciona circuito 19
  circuit20Off();                                       // Desaciona circuito 20
  circuit21Off();                                       // Desaciona circuito 21
  circuit22Off();                                       // Desaciona circuito 22

}

//====================================================================================================================================
