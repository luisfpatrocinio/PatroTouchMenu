# PatroTouchMenu

Este projeto demonstra a implementação de um menu interativo touchscreen utilizando um display TFT (ILI9341) e um controlador de toque (XPT2046) integrados ao ESP32. A interface de usuário (UI) é mantida de forma modular e separada da lógica principal, seguindo o princípio K.I.S.S. (Keep It Simple, Stupid). O gerenciamento gráfico e de toque é feito utilizando a biblioteca `TFT_eSPI`.

## Arquitetura de Hardware: Barramento SPI Compartilhado

O display ILI9341 e o controlador de toque XPT2046 comunicam-se com o ESP32 através do protocolo SPI (Serial Peripheral Interface). Para otimizar o uso de pinos do microcontrolador, ambos os periféricos compartilham o mesmo barramento SPI, o que significa que os pinos de **MOSI** (Master Out Slave In), **MISO** (Master In Slave Out) e **SCK** (Serial Clock) são conectados em paralelo para ambos os dispositivos.

### A Importância dos Pinos CS (Chip Select)

Quando múltiplos dispositivos compartilham um único barramento SPI, o ESP32 precisa de uma forma confiável para determinar com qual dispositivo ele está falando a cada instante. Isso é feito através dos pinos **CS (Chip Select)**, que atuam como habilitadores individuais.

- **TFT_CS (Display Chip Select):** Quando este pino está em nível lógico baixo (LOW), o display ILI9341 "escuta" e responde aos comandos de vídeo no barramento SPI.
- **TOUCH_CS (Touch Chip Select):** Quando este pino está em nível lógico baixo (LOW), o controlador XPT2046 é ativado para enviar as coordenadas físicas do toque ao ESP32.

**Regra de Ouro do Barramento Compartilhado:** Apenas um pino CS pode estar em nível lógico baixo por vez. Se ambos os pinos CS estiverem ativos simultaneamente, os sinais de ambos os dispositivos irão colidir (conflito de barramento), resultando em falha de comunicação, artefatos visuais na tela ou leituras erráticas do touch.

A biblioteca `TFT_eSPI` gerencia a alternância desses sinais de forma eficiente e automática via software, desde que os pinos CS sejam definidos corretamente na configuração do arquivo `User_Setup.h`.

## Estrutura do Código

A arquitetura do software foca em alta modularidade e manutenibilidade:
- `main.cpp`: Ponto de entrada do firmware. Mantém o loop principal limpo e focado no fluxo operacional global, delegando as complexidades de UI para a classe gerenciadora.
- `PatroMenu.h` / `PatroMenu.cpp`: Encapsulam toda a lógica de desenho da interface (botões, menus e layouts) e o tratamento das interações físicas de toque (debounce e checagem de limites). 

## Padrões de Código
Este projeto adere estritamente aos seguintes padrões de estilo de código:
- Variáveis e propriedades adotam obrigatoriamente a notação `camelCase`.
- Funções e métodos de classe utilizam `PascalCase`.
- A notação `snake_case` não é utilizada na base de código.
