#include "PatroUiManager.h"
#include <Arduino.h>

void PatroUiManager::Init() {
  BuildMasterLayout();
  BuildMainMenu();
}

void PatroUiManager::BuildMasterLayout() {
  // Pega a tela principal atual
  lv_obj_t *screen = lv_screen_active();
  lv_obj_set_style_bg_color(screen, lv_color_hex(0x1a1a1a),
                            0); // Fundo escuro padrão

  // ==========================================
  // 1. BARRA DE STATUS (Topo)
  // ==========================================
  statusBar = lv_obj_create(screen);
  lv_obj_set_size(statusBar, 320, 30);
  lv_obj_align(statusBar, LV_ALIGN_TOP_MID, 0, 0);
  lv_obj_set_style_radius(statusBar, 0, 0);       // Bordas quadradas
  lv_obj_set_style_border_width(statusBar, 0, 0); // Sem borda externa
  lv_obj_set_style_bg_color(statusBar, lv_color_hex(0x2d2d2d), 0);
  lv_obj_remove_flag(statusBar, LV_OBJ_FLAG_SCROLLABLE); // Trava o scroll

  // Ícone de Wi-Fi (Inicia cinza, pois está desconectado)
  labelWifiIcon = lv_label_create(statusBar);
  lv_label_set_text(labelWifiIcon, LV_SYMBOL_WIFI " Desc.");
  lv_obj_align(labelWifiIcon, LV_ALIGN_LEFT_MID, 0, 0);
  lv_obj_set_style_text_color(labelWifiIcon, lv_color_hex(0x888888), 0);

  // Ícone de Bateria (Apenas cosmético por enquanto)
  labelBatteryIcon = lv_label_create(statusBar);
  lv_label_set_text(labelBatteryIcon, "100% " LV_SYMBOL_BATTERY_FULL);
  lv_obj_align(labelBatteryIcon, LV_ALIGN_RIGHT_MID, 0, 0);
  lv_obj_set_style_text_color(labelBatteryIcon, lv_color_hex(0xFFFFFF), 0);

  // ==========================================
  // 2. ÁREA DE CONTEÚDO (Centro/Abaixo)
  // ==========================================
  contentArea = lv_obj_create(screen);
  lv_obj_set_size(contentArea, 320,
                  210); // Altura total (240) menos a barra (30)
  lv_obj_align(contentArea, LV_ALIGN_BOTTOM_MID, 0, 0);
  lv_obj_set_style_radius(contentArea, 0, 0);
  lv_obj_set_style_border_width(contentArea, 0, 0);
  lv_obj_set_style_bg_color(contentArea, lv_color_hex(0x1a1a1a), 0);
  lv_obj_set_style_pad_all(contentArea, 0, 0); // Remove margens internas
}

void PatroUiManager::BuildMainMenu() {
  // Cria um widget de Lista preenchendo toda a Área de Conteúdo
  mainMenu = lv_list_create(contentArea);
  lv_obj_set_size(mainMenu, 320, 210);
  lv_obj_center(mainMenu);
  lv_obj_set_style_radius(mainMenu, 0, 0);
  lv_obj_set_style_border_width(mainMenu, 0, 0);
  lv_obj_set_style_bg_color(mainMenu, lv_color_hex(0x1a1a1a), 0);

  // Adiciona os botões passando um ID secreto no "user_data"
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
  // Recupera qual botão foi clicado através do ID
  const char *btnId = (const char *)lv_event_get_user_data(event);

  Serial.print("Voce clicou no menu: ");
  Serial.println(btnId);

  // Nas próximas etapas, usaremos o btnId para limpar o mainMenu e desenhar a
  // tela correta!
}