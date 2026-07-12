#ifndef PATRO_MENU_H
#define PATRO_MENU_H

#include <TFT_eSPI.h>

// Estrutura que representa um botão na tela
struct TouchButton {
  int x;
  int y;
  int width;
  int height;
  const char *label;
  uint32_t color;

  // Verifica se o toque (touchX, touchY) está dentro dos limites do botão
  bool IsPressed(int touchX, int touchY) const {
    return (touchX >= x && touchX <= (x + width) && touchY >= y &&
            touchY <= (y + height));
  }
};

// Gerenciador do Menu Touch
class PatroMenu {
public:
  // Passamos a referência da instância global do TFT
  PatroMenu(TFT_eSPI &tftRef);

  void Init();
  void DrawMainMenu();
  void DrawConfigMenu();
  void CheckTouch();
  void UpdateAnimation(); // Nova função para animar

private:
  TFT_eSPI &tft;
  int currentScreen; // 0 = Principal, 1 = Configurações

  // Definição dos botões usando nossa estrutura
  TouchButton btnConfig;
  TouchButton btnStatus;
  TouchButton btnBack;

  // Variáveis da animação
  int animX;
  int animDirection;
  unsigned long lastAnimTime;

  void DrawButton(const TouchButton &button);
  bool GetTouchCoordinates(uint16_t &x, uint16_t &y);
};

#endif // PATRO_MENU_H