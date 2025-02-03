# Projeto: Controle de Matriz de LEDs com Interrupções no RP2040

## Descrição
Este projeto utiliza a placa BitDogLab (RP2040) para controlar uma matriz de LEDs WS2812 de 5x5, exibindo números de 0 a 9. O número exibido pode ser alterado através de botões físicos, utilizando interrupções para incrementar ou decrementar o valor. Além disso, um LED vermelho pisca continuamente a uma frequência de 5Hz.

## Funcionalidades
- **Piscar LED RGB vermelho**: O LED vermelho pisca 5 vezes por segundo.
- **Controle via botões**:
  - O botão A (GPIO5) incrementa o número exibido.
  - O botão B (GPIO6) decrementa o número exibido.
- **Exibição de números na matriz de LEDs**: 
  - A matriz de LEDs WS2812 exibe os números de 0 a 9 com um layout fixo.
- **Tratamento de bouncing**: O código implementa um mecanismo de debouncing via software para evitar leituras falsas dos botões.

## Componentes Utilizados
- Placa BitDogLab (RP2040)
- Matriz de LEDs WS2812 (5x5) conectada à GPIO 7
- LED RGB (GPIOs 11, 12 e 13)
- Botão A (GPIO5)
- Botão B (GPIO6)

## Estrutura do Código
- **Interrupções (IRQ)**: São utilizadas para capturar eventos de pressionamento dos botões.
- **Debouncing**: Implementado via software para garantir leituras confiáveis dos botões.
- **PIO (Programmable I/O)**: Utilizado para controlar a matriz WS2812 de forma eficiente.
- **Loop principal**: Alterna o estado do LED RGB e atualiza a matriz de LEDs com base no valor atual.

## Como Executar
1. Compile e carregue o código na placa RP2040.
2. Pressione os botões para alterar o número exibido na matriz de LEDs.
3. Observe o LED vermelho piscando continuamente a 5Hz.

## Demonstração
Confira o vídeo explicativo neste link: [Vídeo no Google Drive](https://drive.google.com/file/d/1w5WSy9XkPhuAusy14a8Ck-q5Mhl9h2uJ/view?usp=sharing)

---
Projeto desenvolvido para consolidar conceitos de interrupções, debouncing e controle de LEDs no RP2040.
