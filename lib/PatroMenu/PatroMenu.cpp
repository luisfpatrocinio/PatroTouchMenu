#include "PatroMenu.h"

PatroMenu::PatroMenu(TFT_eSPI &tftRef)
    : tft(tftRef), currentScreen(0), animX(0), animDirection(1),
      lastAnimTime(0) {

  // Inicializa as posições dos botões (Tela em modo paisagem: 320x240)
  btnConfig = {20, 100, 120, 50, "CONFIG", TFT_BLUE};
  btnStatus = {180, 100, 120, 50, "STATUS", TFT_DARKGREEN};
  btnBack = {10, 190, 80, 40, "VOLTAR", TFT_RED};
}

void PatroMenu::Init() {
  tft.init();
  tft.setRotation(1); // 1 = Modo Paisagem (320x240)

  // Importante: Na primeira execução, o touch precisa ser calibrado.
  // Para simplificar o aprendizado, a TFT_eSPI aceita dados de calibração
  // fixos.
  uint16_t calibrationData[5] = {158, 3579, 292, 3598, 1};
  tft.setTouch(calibrationData);

  DrawMainMenu();
}

void PatroMenu::DrawButton(const TouchButton &button) {
  tft.fillRect(button.x, button.y, button.width, button.height, button.color);
  tft.drawRect(button.x, button.y, button.width, button.height, TFT_WHITE);

  tft.setTextColor(TFT_WHITE);
  tft.setTextDatum(MC_DATUM); // Alinha o texto estritamente no centro do botão
  tft.drawString(button.label, button.x + (button.width / 2),
                 button.y + (button.height / 2), 2);
}

void PatroMenu::DrawMainMenu() {
  currentScreen = 0;
  tft.fillScreen(TFT_BLACK);

  tft.setTextColor(TFT_WHITE);
  tft.setTextDatum(TC_DATUM);
  tft.drawString("MENU PRINCIPAL", 160, 30, 4);

  // Desenha os botões da tela inicial
  DrawButton(btnConfig);
  DrawButton(btnStatus);
}

void PatroMenu::DrawConfigMenu() {
  currentScreen = 1;
  tft.fillScreen(TFT_BLACK);

  tft.setTextColor(TFT_WHITE);
  tft.setTextDatum(TC_DATUM);
  tft.drawString("CONFIGURACOES", 160, 30, 4);

  // Tela de config mostra apenas o botão de voltar
  DrawButton(btnBack);
}

void PatroMenu::UpdateAnimation() {
  if (millis() - lastAnimTime > 20) {
    tft.fillRect(animX, 5, 10, 10, TFT_BLACK);
    animX += animDirection * 4;

    if (animX >= 310 || animX <= 0) {
      animDirection *= -1;
    }

    tft.fillRect(animX, 5, 10, 10, TFT_YELLOW);
    lastAnimTime = millis();
  }
}

bool PatroMenu::GetTouchCoordinates(uint16_t &x, uint16_t &y) {
  bool isTouched = tft.getTouch(&x, &y);

  // Retorna true se a tela estiver de fato sendo pressionada
  return isTouched;
}

void PatroMenu::CheckTouch() {
  uint16_t touchX = 0;
  uint16_t touchY = 0;

  if (GetTouchCoordinates(touchX, touchY)) {
    // Pequeno delay para evitar múltiplos registros por clique (Debounce)
    delay(150);

    if (currentScreen == 0) {
      if (btnConfig.IsPressed(touchX, touchY)) {
        DrawConfigMenu();
      } else if (btnStatus.IsPressed(touchX, touchY)) {
        // Apenas pisca a tela com outra cor para dar feedback do touch
        tft.fillScreen(TFT_DARKGREEN);
        delay(300);
        DrawMainMenu();
      }
    } else if (currentScreen == 1) {
      if (btnBack.IsPressed(touchX, touchY)) {
        DrawMainMenu();
      }
    }
  }
}