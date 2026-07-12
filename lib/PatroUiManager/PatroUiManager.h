#ifndef PATRO_UI_MANAGER_H
#define PATRO_UI_MANAGER_H

#include <lvgl.h>

class PatroUiManager {
public:
  void Init();
  void Update(); // Nova função para gerenciar o tempo da UI

private:
  // Controle da Splash Screen
  lv_obj_t *splashScreen;
  unsigned long splashStartTime;
  bool isSplashActive;

  // Nossos containers principais
  lv_obj_t *statusBar;
  lv_obj_t *contentArea;
  lv_obj_t *mainMenu;

  // Elementos dinâmicos da Barra de Status
  lv_obj_t *labelWifiIcon;
  lv_obj_t *labelBatteryIcon;

  void BuildSplashScreen();
  void BuildMasterLayout();
  void BuildMainMenu();

  static void onMainMenuBtnClicked(lv_event_t *event);
};

#endif