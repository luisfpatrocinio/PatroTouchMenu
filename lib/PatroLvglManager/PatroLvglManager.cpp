#include "PatroLvglManager.h"

PatroLvglManager::PatroLvglManager(TFT_eSPI &tftRef) : tft(tftRef) {}

void PatroLvglManager::Init() {
  tft.begin();
  tft.setRotation(1);

  uint16_t calibrationData[5] = {158, 3579, 292, 3598, 1};
  tft.setTouch(calibrationData);

  lv_init();

  // ------------------------------------------
  // LVGL 9: Registro do Display
  // ------------------------------------------
  lv_display_t *display = lv_display_create(screenWidth, screenHeight);
  lv_display_set_buffers(display, drawBuffer, NULL, sizeof(drawBuffer),
                         LV_DISPLAY_RENDER_MODE_PARTIAL);
  lv_display_set_flush_cb(display, FlushDisplay);
  lv_display_set_user_data(display, this);

  // ------------------------------------------
  // LVGL 9: Registro do Input (Touch)
  // ------------------------------------------
  lv_indev_t *indev = lv_indev_create();
  lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
  lv_indev_set_read_cb(indev, ReadTouch);
  lv_indev_set_user_data(indev, this);
}

void PatroLvglManager::Update() {
  // LVGL 9 precisa que você controle o tempo explicitamente
  // Incrementamos o tempo interno (em milissegundos) antes de processar
  lv_tick_inc(5);
  lv_timer_handler();
}

void PatroLvglManager::FlushDisplay(lv_display_t *display,
                                    const lv_area_t *area, uint8_t *pxMap) {
  PatroLvglManager *manager =
      (PatroLvglManager *)lv_display_get_user_data(display);

  uint32_t width = (area->x2 - area->x1 + 1);
  uint32_t height = (area->y2 - area->y1 + 1);

  manager->tft.startWrite();
  manager->tft.setAddrWindow(area->x1, area->y1, width, height);
  manager->tft.pushColors((uint16_t *)pxMap, width * height, true);
  manager->tft.endWrite();

  lv_display_flush_ready(display);
}

void PatroLvglManager::ReadTouch(lv_indev_t *indev, lv_indev_data_t *data) {
  PatroLvglManager *manager = (PatroLvglManager *)lv_indev_get_user_data(indev);
  uint16_t touchX;
  uint16_t touchY;

  bool isTouched = manager->tft.getTouch(&touchX, &touchY);

  if (isTouched) {
    data->state = LV_INDEV_STATE_PRESSED;
    data->point.x = touchX;
    data->point.y = touchY;
  } else {
    data->state = LV_INDEV_STATE_RELEASED;
  }
}