#ifndef PATRO_UI_MANAGER_H
#define PATRO_UI_MANAGER_H

#include <lvgl.h>

class PatroUiManager {
public:
  void Init();
  void Update();

private:
  lv_obj_t *splashScreen;
  unsigned long splashStartTime;
  bool isSplashActive;

  // Flag para saber se estamos aguardando a lista de Wi-Fi
  bool isWaitingForScan;

  lv_obj_t *statusBar;
  lv_obj_t *contentArea;
  lv_obj_t *mainMenu;

  lv_obj_t *labelWifiIcon;
  lv_obj_t *labelBatteryIcon;

  void BuildSplashScreen();
  void BuildMasterLayout();
  void BuildMainMenu();

  // Novas telas do fluxo de Rede
  void ShowWifiLoading();
  void BuildWifiList();

  // Callbacks em PascalCase
  static void OnMainMenuBtnClicked(lv_event_t *event);
  static void OnNetworkBtnClicked(lv_event_t *event);
  static void OnBackBtnClicked(lv_event_t *event);
};

#endif