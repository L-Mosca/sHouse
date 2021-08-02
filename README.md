# sHouse
Sistema de automação residencial integrado com aplicação mobile e inteligência artificial

Esse projeto se trata da implementação de um sistema de automação residencial comtemplando apenas circuitos de iluminação. Estes, por sua vez, serão comandados por: teclas pulsadoras, aplicativo mobile e inteligência artificial.

As teclas pulsadoras são importantes, porque elas substituiram os interruptores convencionas que normalmente temos instalados em nossas casas.

Para o aplicativo mobile e a inteligência artificial, será usada a aplicação: "Amazon Alexa". Esta é gratuita e pode ser instaladas em dispositivos android e ios.



Alguns esclarecimentos:

Por que realizar a automação apenas de lâmpadas?
  - Este projeto contempla apenas circuitos de iluminação, porque esta é a base da automação residencial, assim, a partir disso, este trabalho pode se expandir para qualquer rumo, desde cortinas e persianas a até TVs e ar-condicionados.

Por que utilizar teclas pulsadoras?
  - A idéia de substituir todos os interruptores convencionais por teclas pulsadoras, vem do ideal de realizar um sistema de automação completo, assim os pulsadores comtemplam a parte física da casa. Outro fator importante de se citar é que dessa forma, a automação seja simples e intuitiva para dismistificar o pré-conceito que a maioria das pessoas possuem de encarar a técnologia como algo complexo e difícil de se lidar.


Por que utilizar o aplicativo "Amazon Alexa"?
 - A utilização desse aplicativo se dá pelo fato de eu possuir uma echo dot, então a escolha pelo uso dessa técnologia foi mais pela praticidade e economia de minha parte.



Instruções:

Este documento tem o objetivo de explanar todos os componenetes utilizados e a arquitetura da rede utilizada para comunicação.

Primeiramente é importante salientar que esse projeto foi prototipado em uma maquete, a qual comtempla 39 LEDs que por sua vez constituem 27 circuitos de iluminação distribuídos
entre os ambientes. Vale destacar que como isto é um protótipo todos os 39 LEDs foram conectados em saídas independentes do microcontrolador, isso foi feito para que varios LEDs sejam conectadas a uma única saída do MCU que por sinal possui uma grande limitação quantitativa a respeito da corrente elétrica provida pela mesma.

Juntamente a maquete, tenho uma maleta que usei para conectar as teclas pulsadoras (entradas) e uma lâmpada bivolt interfaceada com um módulo relé 5V. Tudo isso com o intuito de mostrar a eficiencia prática do projeto, que não se limita apenas a seccionar LEDs em uma maquete.



No projeto são utilizadas duas placas embarcadas. A primeira é um arduino mega ADK e a segunda é um ESP8266.


Arduino Mega ADK:
No arduino mega foram conectadas todas as entradas (teclas pulsadoras) e saídas do sistema de automação (circuitos de iluminação). Ele é o responsável por seccionar todos os circuitos de iluminação e executar as rotinas responsáveis por testar as entradas e realizar a lógica por trás das "cenas" criadas para o protótipo.



Esp8266:
O Esp8266 é responsável por realizar a integração do sistema de automação com a assistente virtual "Alexa" e adicionar todos os dispositivos da casa (lâmpadas).
Ele tambem está conectado ao arduino mega pelo protocólo de comunicação i2c. Isso é muito importante, porque é através dessa comunicação que serão executados os comandos recebidos pela inteligência artificial, seja ele por voz ou manual (por meio do aplicativo amazon alexa).



**CENAS -> Uma cena é um conceito na automação que define uma sequência de comandos executados simultaneamente com o objetivo de tornar um ou mais ambientes da casa propícios a realização de alguma atividade específica. Exemplo: Eu elaborei uma cena chamada "masterOff" que desliga toda a iluminação da casa. Essa é uma cena idealizada, por exemplo,
para uma situação em que todos os moradores da casa estão saindo e querem desligar todos os comodos da casa.

