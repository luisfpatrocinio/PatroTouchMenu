#include "PatroUiManager.h"
#include "PatroStorageCore.h"
#include "PatroTimeCore.h"
#include "PatroWifiCore.h"
#include "PatroRetroCore.h"
#include <Arduino.h>

// Instâncias globais necessárias para os callbacks se comunicarem
extern PatroWifiCore wifiCore;
extern PatroUiManager interfaceApp;
extern PatroStorageCore storageCore;
extern PatroTimeCore timeCore;
extern PatroRetroCore retroCore;

void PatroUiManager::Init() {
  isSplashActive = true;
  isWaitingForScan = false;
  isPendingScan = false;
  lastWifiState = WifiState::Disconnected;
  splashStartTime = millis();
  lastTimeUpdate = 0;

  // Recupera a cor salva e já aplica no LVGL!
  currentThemeColor = storageCore.GetThemeColor();
  ApplyThemeColor(currentThemeColor);

  BuildSplashScreen();
}

void PatroUiManager::Update() {
  if (isSplashActive && (millis() - splashStartTime > 2000)) {
    isSplashActive = false;
    lv_obj_delete(splashScreen);
    BuildMasterLayout();
    BuildMainMenu();
  }

  // ==========================================
  // GATILHO DE SCAN COM ATRASO (Para não congelar a UI)
  // ==========================================
  if (isPendingScan && (millis() - scanTriggerTime > 50)) {
    isPendingScan = false;
    wifiCore
        .StartScan(); // Agora sim o ESP32 congela, mas a tela já está certa!
  }

  // ==========================================
  // LÓGICA DE ESPERA DA LISTA
  // ==========================================
  if (isWaitingForScan) {
    if (wifiCore.GetState() == WifiState::ScanComplete) {
      isWaitingForScan = false;
      BuildWifiList();
    } else if (wifiCore.GetState() == WifiState::Disconnected) {
      isWaitingForScan = false;
      HandleScanFailure();
    }
  }

  // ==========================================
  // ATUALIZAÇÃO DINÂMICA DA BARRA DE STATUS
  // ==========================================
  WifiState currentState = wifiCore.GetState();

  // Só atualiza a tela se o estado mudou (evita piscar a tela à toa) e a barra
  // de status já existir
  if (!isSplashActive && currentState != lastWifiState) {
    lastWifiState = currentState;

    if (currentState == WifiState::Connected) {
      lv_label_set_text(labelWifiIcon, LV_SYMBOL_WIFI " Conectado");
      lv_obj_set_style_text_color(labelWifiIcon, lv_color_hex(0x00FF00),
                                  0); // Verde brilhante!
    } else if (currentState == WifiState::Connecting) {
      lv_label_set_text(labelWifiIcon, LV_SYMBOL_WIFI " Conectando...");
      lv_obj_set_style_text_color(labelWifiIcon, lv_color_hex(0xFFFF00),
                                  0); // Amarelo
    } else {
      lv_label_set_text(labelWifiIcon, LV_SYMBOL_WIFI " Desc.");
      lv_obj_set_style_text_color(labelWifiIcon, lv_color_hex(0x888888),
                                  0); // Cinza
    }
  }

  // ==========================================
  // ATUALIZAÇÃO DO RELÓGIO
  // ==========================================
  if (!isSplashActive && (millis() - lastTimeUpdate >= 1000)) {
    lastTimeUpdate = millis();
    lv_label_set_text(labelTime, timeCore.GetFormattedTime().c_str());
  }
}

void PatroUiManager::HandleScanFailure() {
  lv_obj_clean(contentArea);
  lv_obj_t *errorLabel = lv_label_create(contentArea);
  lv_label_set_text(errorLabel, LV_SYMBOL_WARNING
                    " Falha ao buscar redes.\nTente novamente.");
  lv_obj_set_style_text_color(errorLabel, lv_color_hex(0xFF0000), 0);
  lv_obj_center(errorLabel);

  // Botão para voltar
  lv_obj_t *btnBack = lv_btn_create(contentArea);
  lv_obj_align(btnBack, LV_ALIGN_BOTTOM_MID, 0, -20);
  lv_obj_t *btnLabel = lv_label_create(btnBack);
  lv_label_set_text(btnLabel, "Voltar");
  lv_obj_add_event_cb(btnBack, OnBackBtnClicked, LV_EVENT_CLICKED, NULL);
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

  labelTime = lv_label_create(statusBar);
  lv_label_set_text(labelTime, "--:--");
  lv_obj_align(labelTime, LV_ALIGN_CENTER, 0, 0);
  lv_obj_set_style_text_color(labelTime, lv_color_hex(0xFFFFFF), 0);

  // Área Principal
  contentArea = lv_obj_create(screen);
  lv_obj_set_size(contentArea, 320, 210);
  lv_obj_align(contentArea, LV_ALIGN_BOTTOM_MID, 0, 0);
  lv_obj_set_style_radius(contentArea, 0, 0);
  lv_obj_set_style_border_width(contentArea, 0, 0);
  lv_obj_set_style_pad_all(contentArea, 0, 0);
}

void PatroUiManager::BuildMainMenu() {
  mainMenu = lv_list_create(contentArea);
  lv_obj_set_size(mainMenu, 320, 210);
  lv_obj_center(mainMenu);
  lv_obj_set_style_radius(mainMenu, 0, 0);
  lv_obj_set_style_border_width(mainMenu, 0, 0);

  // 1. Rede Wi-Fi
  lv_obj_t *btnWifi =
      lv_list_add_button(mainMenu, LV_SYMBOL_WIFI, "  Rede Wi-Fi");
  lv_obj_add_event_cb(btnWifi, OnMainMenuBtnClicked, LV_EVENT_CLICKED,
                      (void *)"wifi");

  // 2. RetroAchievements
  lv_obj_t *btnRetro =
      lv_list_add_button(mainMenu, LV_SYMBOL_PLAY, "  RetroAchievements");
  lv_obj_add_event_cb(btnRetro, OnMainMenuBtnClicked, LV_EVENT_CLICKED,
                      (void *)"retro");

  // 3. Personalizar Tema
  lv_obj_t *btnTheme =
      lv_list_add_button(mainMenu, LV_SYMBOL_TINT, "  Personalizar Tema");
  lv_obj_add_event_cb(btnTheme, OnMainMenuBtnClicked, LV_EVENT_CLICKED,
                      (void *)"theme");

  // 4. Configurações do Sistema
  lv_obj_t *btnSys = lv_list_add_button(mainMenu, LV_SYMBOL_SETTINGS,
                                        "  Configuracoes do Sistema");
  lv_obj_add_event_cb(btnSys, OnMainMenuBtnClicked, LV_EVENT_CLICKED,
                      (void *)"system");
}

void PatroUiManager::ShowWifiLoading() {
  // Apaga o menu principal de forma limpa
  lv_obj_clean(contentArea);

  lv_obj_t *loadingLabel = lv_label_create(contentArea);
  lv_label_set_text(loadingLabel, LV_SYMBOL_REFRESH " Procurando redes...");
  lv_obj_set_style_text_color(loadingLabel, lv_color_hex(0xFFFFFF), 0);
  lv_obj_center(loadingLabel);

  isWaitingForScan = true; // Avisa o Update() para ficar alerta
}

void PatroUiManager::BuildWifiList() {
  lv_obj_clean(contentArea);

  lv_obj_t *wifiList = lv_list_create(contentArea);
  lv_obj_set_size(wifiList, 320, 210);
  lv_obj_center(wifiList);
  lv_obj_set_style_radius(wifiList, 0, 0);
  lv_obj_set_style_border_width(wifiList, 0, 0);

  lv_obj_t *btnBack = lv_list_add_button(wifiList, LV_SYMBOL_LEFT, "  Voltar");
  lv_obj_add_event_cb(btnBack, OnBackBtnClicked, LV_EVENT_CLICKED, NULL);

  int totalNetworks = wifiCore.GetFoundNetworksCount();

  if (totalNetworks == 0) {
    lv_list_add_text(wifiList, "Nenhuma rede encontrada.");
  } else {
    for (int i = 0; i < totalNetworks; i++) {
      String ssid = wifiCore.GetNetworkName(i);
      lv_obj_t *btnNet =
          lv_list_add_button(wifiList, LV_SYMBOL_WIFI, ssid.c_str());

      // O truque: passamos o índice 'i' diretamente no user_data!
      lv_obj_add_event_cb(btnNet, OnNetworkBtnClicked, LV_EVENT_CLICKED,
                          (void *)(uintptr_t)i);
    }
  }
}

void PatroUiManager::OnMainMenuBtnClicked(lv_event_t *event) {
  const char *btnId = (const char *)lv_event_get_user_data(event);

  if (strcmp(btnId, "wifi") == 0) {
    interfaceApp.ShowWifiLoading();
    interfaceApp.isPendingScan = true;
    interfaceApp.scanTriggerTime = millis();
  } else if (strcmp(btnId, "theme") == 0) {
    interfaceApp.BuildThemeScreen();
  } else if (strcmp(btnId, "retro") == 0) {
    if (storageCore.HasRetroUsername()) {
      String savedUser = storageCore.GetRetroUsername();

      // Dispara a requisição HTTP para a API
      Serial.print("Buscando dados de: ");
      Serial.println(savedUser);
      retroCore.FetchPlayerSummary(savedUser);

      interfaceApp.BuildRetroDashboard();
    } else {
      interfaceApp.BuildRetroLoginScreen();
    }
  }
}

void PatroUiManager::OnBackBtnClicked(lv_event_t *event) {
  lv_obj_clean(interfaceApp.contentArea);
  interfaceApp.BuildMainMenu(); // Reconstrói o menu principal
}

void PatroUiManager::OnNetworkBtnClicked(lv_event_t *event) {
  // Recupera o índice da rede que salvamos no user_data
  int index = (int)(uintptr_t)lv_event_get_user_data(event);

  // Salva o SSID na interface global
  interfaceApp.selectedSsid = wifiCore.GetNetworkName(index);

  // Abre a tela de senha
  interfaceApp.ShowPasswordInput();
}

void PatroUiManager::ShowPasswordInput() {
  lv_obj_clean(contentArea);

  lv_obj_t *titleLabel = lv_label_create(contentArea);
  lv_label_set_text_fmt(titleLabel, "Senha para: %s", selectedSsid.c_str());
  lv_obj_set_style_text_color(titleLabel, lv_color_hex(0xFFFFFF), 0);
  lv_obj_align(titleLabel, LV_ALIGN_TOP_MID, 0, 5);

  // Cria a caixa de texto
  passwordTextArea = lv_textarea_create(contentArea);
  lv_obj_set_width(passwordTextArea, 280);
  lv_obj_align(passwordTextArea, LV_ALIGN_TOP_MID, 0, 30);
  lv_textarea_set_password_mode(passwordTextArea,
                                true); // Esconde a senha com asteriscos
  lv_textarea_set_one_line(passwordTextArea, true);
  lv_textarea_set_placeholder_text(passwordTextArea, "Digite a senha...");

  // Cria o teclado virtual ocupando a base da tela
  keyboard = lv_keyboard_create(contentArea);
  lv_keyboard_set_textarea(keyboard,
                           passwordTextArea); // Linka o teclado na caixa
  lv_obj_set_size(keyboard, 320, 130);
  lv_obj_align(keyboard, LV_ALIGN_BOTTOM_MID, 0, 0);

  // Eventos do teclado ("V" para confirmar, "X" ou teclado pra baixo para
  // cancelar)
  lv_obj_add_event_cb(keyboard, OnKeyboardReadyBtn, LV_EVENT_READY, NULL);
  lv_obj_add_event_cb(keyboard, OnKeyboardCancelBtn, LV_EVENT_CANCEL, NULL);
}

void PatroUiManager::OnKeyboardReadyBtn(lv_event_t *event) {
  const char *password = lv_textarea_get_text(interfaceApp.passwordTextArea);

  Serial.print("Conectando em: ");
  Serial.println(interfaceApp.selectedSsid);
  Serial.print("Senha digitada: ");
  Serial.println(password);

  // Manda o backend conectar!
  wifiCore.ConnectToNetwork(interfaceApp.selectedSsid.c_str(), password);

  // Limpa a tela e volta pro menu principal enquanto conecta em background
  lv_obj_clean(interfaceApp.contentArea);
  interfaceApp.BuildMainMenu();
}

void PatroUiManager::OnKeyboardCancelBtn(lv_event_t *event) {
  // Se o usuário desistir, só voltamos para a lista de redes
  interfaceApp.BuildWifiList();
}

void PatroUiManager::ApplyThemeColor(uint32_t hexColor) {
  currentThemeColor = hexColor;

  // Configura o tema padrão do LVGL como Modo Escuro (true) e passa a nova cor
  // principal
  lv_theme_t *theme =
      lv_theme_default_init(NULL, // Usa o display padrão
                            lv_color_hex(hexColor),
                            lv_color_hex(0x444444), // Cor secundária (cinza)
                            true,                   // Modo Escuro ativado
                            LV_FONT_DEFAULT);

  lv_display_set_theme(NULL, theme);

  // Avisa todos os widgets da tela que o tema mudou!
  lv_obj_report_style_change(NULL);
}

void PatroUiManager::BuildThemeScreen() {
  lv_obj_clean(contentArea);

  lv_obj_t *titleLabel = lv_label_create(contentArea);
  lv_label_set_text(titleLabel, LV_SYMBOL_TINT " Cor do Sistema");
  lv_obj_set_style_text_color(titleLabel, lv_color_hex(0xFFFFFF), 0);
  lv_obj_align(titleLabel, LV_ALIGN_TOP_MID, 0, 5);

  // Cria o Arco Interativo (Substituindo a antiga roda de cores)
  themeArc = lv_arc_create(contentArea);
  lv_obj_set_size(themeArc, 130, 130);
  lv_obj_align(themeArc, LV_ALIGN_CENTER, 0, 0);

  // Configura o arco para dar uma volta completa (0 a 359 graus do sistema HSV)
  lv_arc_set_range(themeArc, 0, 359);
  lv_arc_set_value(themeArc, 200); // Valor inicial (um tom de azul)

  // Dispara o evento toda vez que você arrastar o arco
  lv_obj_add_event_cb(themeArc, OnThemeColorChanged, LV_EVENT_VALUE_CHANGED,
                      NULL);

  // Botões Inferiores
  lv_obj_t *btnBack = lv_btn_create(contentArea);
  lv_obj_align(btnBack, LV_ALIGN_BOTTOM_LEFT, 10, -10);
  lv_obj_t *labelBack = lv_label_create(btnBack);
  lv_label_set_text(labelBack, LV_SYMBOL_LEFT " Voltar");
  lv_obj_add_event_cb(btnBack, OnBackBtnClicked, LV_EVENT_CLICKED, NULL);

  lv_obj_t *btnSave = lv_btn_create(contentArea);
  lv_obj_align(btnSave, LV_ALIGN_BOTTOM_RIGHT, -10, -10);
  lv_obj_t *labelSave = lv_label_create(btnSave);
  lv_label_set_text(labelSave, LV_SYMBOL_SAVE " Salvar");
  lv_obj_add_event_cb(btnSave, OnSaveThemeBtnClicked, LV_EVENT_CLICKED, NULL);
}

void PatroUiManager::OnThemeColorChanged(lv_event_t *event) {
  // Pega o valor do arco (Matiz / Hue)
  int hue = lv_arc_get_value(interfaceApp.themeArc);

  // Converte HSV para RGB (Matiz atual, 100% de Saturação, 100% de Brilho)
  lv_color_t color = lv_color_hsv_to_rgb(hue, 100, 100);

  // Converte a cor nativa para inteiro hexadecimal de 32 bits e aplica
  interfaceApp.ApplyThemeColor(lv_color_to_u32(color));
}

void PatroUiManager::OnSaveThemeBtnClicked(lv_event_t *event) {
  // Manda gravar na Flash
  storageCore.SaveThemeColor(interfaceApp.currentThemeColor);

  // Volta pro menu
  lv_obj_clean(interfaceApp.contentArea);
  interfaceApp.BuildMainMenu();
}

// ==========================================
// TELAS DA RETROACHIEVEMENTS
// ==========================================

void PatroUiManager::BuildRetroLoginScreen() {
  lv_obj_clean(contentArea);

  // Título
  lv_obj_t *titleLabel = lv_label_create(contentArea);
  lv_label_set_text(titleLabel, LV_SYMBOL_EDIT " Perfil Retro");
  lv_obj_align(titleLabel, LV_ALIGN_TOP_MID, 0, 5);

  // Caixa de Texto (Text Area)
  retroUserTextArea = lv_textarea_create(contentArea);
  lv_textarea_set_one_line(retroUserTextArea, true);
  lv_textarea_set_placeholder_text(retroUserTextArea, "Digite seu usuario...");
  lv_obj_set_width(retroUserTextArea, lv_pct(90));
  lv_obj_align(retroUserTextArea, LV_ALIGN_TOP_MID, 0, 40);

  // Teclado Virtual
  lv_obj_t *retroKeyboard = lv_keyboard_create(contentArea);
  lv_keyboard_set_textarea(retroKeyboard,
                           retroUserTextArea); // Conecta o teclado na caixa

  // Escuta os eventos de Confirmar e Cancelar
  lv_obj_add_event_cb(retroKeyboard, OnRetroKeyboardEvent, LV_EVENT_READY,
                      NULL);
  lv_obj_add_event_cb(retroKeyboard, OnRetroKeyboardEvent, LV_EVENT_CANCEL,
                      NULL);
}

void PatroUiManager::BuildRetroDashboard() {
  lv_obj_clean(contentArea);

  // Por enquanto, apenas um texto de sucesso e um botão de voltar
  String welcomeText = "Jogador: " + storageCore.GetRetroUsername() +
                       "\n(Dashboard em construcao...)";

  lv_obj_t *labelInfo = lv_label_create(contentArea);
  lv_label_set_text(labelInfo, welcomeText.c_str());
  lv_obj_align(labelInfo, LV_ALIGN_CENTER, 0, -20);

  // Botão de Voltar
  lv_obj_t *btnBack = lv_btn_create(contentArea);
  lv_obj_align(btnBack, LV_ALIGN_BOTTOM_LEFT, 10, -10);
  lv_obj_t *labelBack = lv_label_create(btnBack);
  lv_label_set_text(labelBack, LV_SYMBOL_LEFT " Voltar");
  lv_obj_add_event_cb(btnBack, OnBackBtnClicked, LV_EVENT_CLICKED, NULL);
}

void PatroUiManager::OnRetroKeyboardEvent(lv_event_t *event) {
  lv_event_code_t code = lv_event_get_code(event);

  if (code == LV_EVENT_READY) {
    // Pega o texto digitado
    const char *typedUser =
        lv_textarea_get_text(interfaceApp.retroUserTextArea);

    if (strlen(typedUser) > 0) {
      // Salva na memória Flash e avança para o Dashboard
      storageCore.SaveRetroUsername(String(typedUser));
      interfaceApp.BuildRetroDashboard();
    }
  } else if (code == LV_EVENT_CANCEL) {
    // Desistiu de digitar, volta pro menu
    interfaceApp.BuildMainMenu();
  }
}