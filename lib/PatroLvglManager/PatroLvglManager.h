#ifndef PATRO_LVGL_MANAGER_H
#define PATRO_LVGL_MANAGER_H

#include <TFT_eSPI.h>
#include <lvgl.h>

class PatroLvglManager {
public:
    PatroLvglManager(TFT_eSPI& tftRef);
    void Init();
    void Update(); 
    void CreateTestUi(); 

private:
    TFT_eSPI& tft;
    
    static const uint16_t screenWidth = 320;
    static const uint16_t screenHeight = 240;
    
    // Na v9 o buffer usa um array bruto de uint8_t
    // Multiplicamos por 2 porque cores de 16-bits ocupam 2 bytes
    uint8_t drawBuffer[screenWidth * 20 * 2]; 

    // Assinaturas exigidas pelo LVGL 9
    static void FlushDisplay(lv_display_t* display, const lv_area_t* area, uint8_t* pxMap);
    static void ReadTouch(lv_indev_t* indev, lv_indev_data_t* data);

    void BuildBasicTab(lv_obj_t* parent);
    void BuildVisualsTab(lv_obj_t* parent);

    static void OnButtonPress(lv_event_t* event);
    static void OnSliderChange(lv_event_t* event);
};

#endif