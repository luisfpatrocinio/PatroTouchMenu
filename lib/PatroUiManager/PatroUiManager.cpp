#include "PatroUiManager.h"

#include "PatroWifiCore.h"
extern PatroWifiCore wifiCore;

#include <Arduino.h>

void PatroUiManager::Init() {
  isSplashActive = true;
  splashStartTime = millis();
  BuildSplashScreen();
}

void PatroUiManager::Update() {
  // Se a splash screen estiver ativa e já se passaram 2 segundos
  if (isSplashActive && (millis() - splashStartTime > 2000)) {
    isSplashActive = false;

    // Deleta a splash screen da memória
    lv_obj_delete(splashScreen);

    // Constrói a interface definitiva
    BuildMasterLayout();
    BuildMainMenu();
  }
}

void PatroUiManager::BuildSplashScreen() {
  // Cria um container na tela inteira
  splashScreen = lv_obj_create(lv_screen_active());
  lv_obj_set_size(splashScreen, 320, 240);
  lv_obj_center(splashScreen);
  lv_obj_set_style_bg_color(splashScreen, lv_color_hex(0x000000), 0);
  lv_obj_set_style_border_width(splashScreen, 0, 0);
  lv_obj_set_style_radius(splashScreen, 0, 0);

  // Título Principal
  lv_obj_t *titleLabel = lv_label_create(splashScreen);
  lv_label_set_text(titleLabel, "PatroTouchMenu");
  lv_obj_set_style_text_color(titleLabel, lv_color_hex(0xFFFFFF), 0);
  // Move o texto um pouco para cima do centro matemático
  lv_obj_align(titleLabel, LV_ALIGN_CENTER, 0, -10);

  // Subtítulo de Créditos
  lv_obj_t *creditsLabel = lv_label_create(splashScreen);
  lv_label_set_text(creditsLabel, "Made by Patro");
  lv_obj_set_style_text_color(creditsLabel, lv_color_hex(0x888888), 0);
  lv_obj_align(creditsLabel, LV_ALIGN_CENTER, 0, 20);
}

void PatroUiManager::BuildMasterLayout() {
  lv_obj_t *screen = lv_screen_active();
  lv_obj_set_style_bg_color(screen, lv_color_hex(0x1a1a1a), 0);

  // Barra de Status
  statusBar = lv_obj_create(screen);
  lv_obj_set_size(statusBar, 320, 30);
  lv_obj_align(statusBar, LV_ALIGN_TOP_MID, 0, 0);
  lv_obj_set_style_radius(statusBar, 0, 0);
  lv_obj_set_style_border_width(statusBar, 0, 0);
  lv_obj_set_style_bg_color(statusBar, lv_color_hex(0x2d2d2d), 0);
  lv_obj_remove_flag(statusBar, LV_OBJ_FLAG_SCROLLABLE);

  labelWifiIcon = lv_label_create(statusBar);
  lv_label_set_text(labelWifiIcon, LV_SYMBOL_WIFI " Desc.");
  lv_obj_align(labelWifiIcon, LV_ALIGN_LEFT_MID, 0, 0);
  lv_obj_set_style_text_color(labelWifiIcon, lv_color_hex(0x888888), 0);

  labelBatteryIcon = lv_label_create(statusBar);
  lv_label_set_text(labelBatteryIcon, "100% " LV_SYMBOL_BATTERY_FULL);
  lv_obj_align(labelBatteryIcon, LV_ALIGN_RIGHT_MID, 0, 0);
  lv_obj_set_style_text_color(labelBatteryIcon, lv_color_hex(0xFFFFFF), 0);

  // Área Principal
  contentArea = lv_obj_create(screen);
  lv_obj_set_size(contentArea, 320, 210);
  lv_obj_align(contentArea, LV_ALIGN_BOTTOM_MID, 0, 0);
  lv_obj_set_style_radius(contentArea, 0, 0);
  lv_obj_set_style_border_width(contentArea, 0, 0);
  lv_obj_set_style_bg_color(contentArea, lv_color_hex(0x1a1a1a), 0);
  lv_obj_set_style_pad_all(contentArea, 0, 0);
}

void PatroUiManager::BuildMainMenu() {
  mainMenu = lv_list_create(contentArea);
  lv_obj_set_size(mainMenu, 320, 210);
  lv_obj_center(mainMenu);
  lv_obj_set_style_radius(mainMenu, 0, 0);
  lv_obj_set_style_border_width(mainMenu, 0, 0);
  lv_obj_set_style_bg_color(mainMenu, lv_color_hex(0x1a1a1a), 0);

  lv_obj_t *btnWifi =
      lv_list_add_button(mainMenu, LV_SYMBOL_WIFI, "  Rede Wi-Fi");
  lv_obj_add_event_cb(btnWifi, onMainMenuBtnClicked, LV_EVENT_CLICKED,
                      (void *)"wifi");

  lv_obj_t *btnTheme =
      lv_list_add_button(mainMenu, LV_SYMBOL_TINT, "  Personalizar Tema");
  lv_obj_add_event_cb(btnTheme, onMainMenuBtnClicked, LV_EVENT_CLICKED,
                      (void *)"theme");

  lv_obj_t *btnSys = lv_list_add_button(mainMenu, LV_SYMBOL_SETTINGS,
                                        "  Configuracoes do Sistema");
  lv_obj_add_event_cb(btnSys, onMainMenuBtnClicked, LV_EVENT_CLICKED,
                      (void *)"system");
}

void PatroUiManager::onMainMenuBtnClicked(lv_event_t *event) {
  const char *btnId = (const char *)lv_event_get_user_data(event);

  // Se o botão de rede foi clicado, comanda o backend a agir!
  if (strcmp(btnId, "wifi") == 0) {
    Serial.println("Interface solicitou abertura do menu de rede...");
    wifiCore.StartScan();
  }
}