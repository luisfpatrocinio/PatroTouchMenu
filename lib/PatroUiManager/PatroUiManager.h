#ifndef PATRO_UI_MANAGER_H
#define PATRO_UI_MANAGER_H

#include <lvgl.h>

class PatroUiManager {
public:
    void Init();
    void BuildMasterLayout();
    void BuildMainMenu();

private:
    // Nossos containers principais
    lv_obj_t* statusBar;
    lv_obj_t* contentArea;
    
    // Elementos dinâmicos da Barra de Status
    lv_obj_t* labelWifiIcon;
    lv_obj_t* labelBatteryIcon;

    // Elementos da Área Principal
    lv_obj_t* mainMenu;

    // Eventos
    static void onMainMenuBtnClicked(lv_event_t* event);
};

#endif