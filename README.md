# PatroTouchMenu

Este projeto demonstra a implementação de um menu interativo touchscreen em um ESP32, substituindo a renderização manual de UI por uma interface gráfica avançada alimentada pela biblioteca **LVGL v9.5.0** em conjunto com a **TFT_eSPI** para o controle de hardware (display ILI9341 e touch XPT2046). 

## Arquitetura de Software: PatroLvglManager

Para manter o princípio **K.I.S.S. (Keep It Simple, Stupid)** e facilitar a manutenção, a complexidade do LVGL foi isolada e encapsulada na classe `PatroLvglManager`. 

Para desenvolvedores com experiência prévia em Game Engines (como a arquitetura baseada em nós da **Godot Engine**), a transição para este modelo será extremamente natural. Assim como em uma cena na Godot, a criação da UI segue uma hierarquia estruturada, onde cada widget/elemento visual (`lv_obj_t`) é instanciado atrelado a um nó "pai" (parent).

O `PatroLvglManager` abstrai toda a etapa de inicialização (buffers e callbacks) exigida pela nova versão 9 do LVGL, injetando nativamente o controle do display e a leitura do touch. Isso garante que o `main.cpp` permaneça enxuto e estritamente focado no ciclo de vida principal do firmware.

## Arquitetura de Hardware: Barramento SPI Compartilhado

O display ILI9341 e o controlador de toque XPT2046 comunicam-se com o ESP32 através do protocolo SPI (Serial Peripheral Interface). Para otimizar o uso de pinos do microcontrolador, ambos os periféricos compartilham o mesmo barramento SPI, o que significa que os pinos de **MOSI** (Master Out Slave In), **MISO** (Master In Slave Out) e **SCK** (Serial Clock) são conectados em paralelo para ambos os dispositivos.

### A Importância dos Pinos CS (Chip Select)

Quando múltiplos dispositivos compartilham um único barramento SPI, o ESP32 precisa de uma forma confiável para determinar com qual dispositivo ele está falando a cada instante. Isso é feito através dos pinos **CS (Chip Select)**, que atuam como habilitadores individuais.

- **TFT_CS (Display Chip Select):** Quando este pino está em nível lógico baixo (LOW), o display ILI9341 "escuta" e responde aos comandos de vídeo no barramento SPI.
- **TOUCH_CS (Touch Chip Select):** Quando este pino está em nível lógico baixo (LOW), o controlador XPT2046 é ativado para enviar as coordenadas físicas do toque ao ESP32.

**Regra de Ouro do Barramento Compartilhado:** Apenas um pino CS pode estar em nível lógico baixo por vez. Se ambos os pinos CS estiverem ativos simultaneamente, os sinais de ambos os dispositivos irão colidir (conflito de barramento), resultando em falha de comunicação, artefatos visuais na tela ou leituras erráticas do touch. 

A biblioteca `TFT_eSPI` gerencia a alternância desses sinais de forma eficiente e automática via software, desde que os pinos CS sejam definidos corretamente na configuração do arquivo `User_Setup.h`.

## Padrões de Código
Este projeto adere estritamente aos seguintes padrões de estilo de código:
- Variáveis e propriedades adotam obrigatoriamente a notação `camelCase`.
- Funções e métodos de classe utilizam `PascalCase`.
- A notação `snake_case` é desencorajada e refatorada de funções e propriedades próprias.
