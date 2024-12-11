# Projeto DISPLAY

Este projeto é uma implementação de controle para o display ILI9341 utilizando um microcontrolador STM32F407VETx. O código inclui funções para inicialização do display, envio de comandos e dados via SPI, e funções gráficas para desenhar formas e texto na tela.

## Estrutura do Projeto
. ├── Core/ │ ├── Inc/ │ │ ├── main.h │ │ ├── stm32f4xx_hal_conf.h │ │ ├── stm32f4xx_it.h │ │ ├── ILI9341.h │ │ ├── ILI9341_GFX.h │ │ └── 5x5_font.h │ ├── Src/ │ │ ├── main.c │ │ ├── ILI9341.c │ │ ├── ILI9341_GFX.c │ │ └── ... ├── Drivers/ │ ├── CMSIS/ │ └── STM32F4xx_HAL_Driver/ ├── Debug/ │ ├── Core/ │ └── ... ├── DISPLAY.ioc ├── README.md └── ...

## Arquivos Principais

### [`Core/Src/ILI9341.h`](Core/Src/ILI9341.h )

Este arquivo contém as definições de funções e macros para a comunicação com o display ILI9341 via SPI.

### [`Core/Src/ILI9341.c`](Core/Src/ILI9341.c )

Este arquivo contém a implementação das funções definidas em [`Core/Src/ILI9341.h`](Core/Src/ILI9341.h ), incluindo inicialização do display, envio de comandos e dados.

### [`Core/Src/ILI9341_GFX.h`](Core/Src/ILI9341_GFX.h )

Este arquivo contém as definições de funções gráficas para desenhar formas e texto no display.

### [`Core/Src/ILI9341_GFX.c`](Core/Src/ILI9341_GFX.c )

Este arquivo contém a implementação das funções gráficas definidas em [`Core/Src/ILI9341_GFX.h`](Core/Src/ILI9341_GFX.h ).

### [`Core/Src/5x5_font.h`](Core/Src/5x5_font.h )

Este arquivo contém a definição de uma fonte de 5x5 pixels usada para desenhar texto no display.

## Exemplo de Uso

O exemplo de uso do display está no arquivo [`Core/Src/main.c`](Core/Src/main.c ). Aqui está um trecho de exemplo de como inicializar o display e desenhar um círculo:

```c
#include "ILI9341.h"
#include "ILI9341_GFX.h"

int main(void) {
    // Inicialização do hardware
    HAL_Init();
    SystemClock_Config();
    
    // Inicialização do display
    ILI9341_Init();
    
    // Limpar a tela
    ILI9341_Fill_Screen(WHITE);
    
    // Desenhar um círculo
    ILI9341_Draw_Hollow_Circle(100, 100, 50, MAGENTA);
    
    while (1) {
        // Loop principal
    }
}
```

## Compilação e Upload
Para compilar e fazer o upload do código para o microcontrolador, utilize o STM32CubeIDE. Abra o projeto DISPLAY.ioc no STM32CubeIDE e siga os passos para compilar e fazer o upload do firmware.
