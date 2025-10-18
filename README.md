# Sistema de resfriamento de estufa de hortaliças com Arduino.

Este projeto foi desenvolvido como parte da formação IoT Specialist da DIO.me.
O objetivo é criar um circuito eletrônico que simule o controle de temperatura de uma estufa de hortaliças, utilizando um Arduino UNO e componentes conectados a uma protoboard.
O sistema é composto por um sensor de temperatura (TMP36), uma buzina (buzzer), um LED vermelho e um motor DC que representa um exaustor.
Quando a temperatura medida atinge 50 °C ou mais, o LED e a buzina são acionados para indicar uma situação de alerta, enquanto o motor é ligado automaticamente para reduzir a temperatura da estufa.
Todo o funcionamento é controlado por código em linguagem C/C++ executado na placa Arduino.
O sistema faz leituras contínuas do sensor de temperatura. Caso a temperatura seja inferior a 30 °C, todos os dispositivos permanecem desligados. A partir de 30 °C, o motor é acionado para ventilação, e ao atingir 50 °C, o LED e o buzzer entram em funcionamento, indicando uma situação de emergência.

## Componentes
Arduino UNO, TMP36, LED + resistor 220Ω, buzzer, motor DC + 2N2222 + diodo 1N4007 + resistor 1kΩ, protoboard, jumpers.

## Código
`src/estufa.ino`

## Circuito
`docs/Projeto de Circuitos Eletrônicos - IoT`

## Esquema
`docs/vista_esquematica`
