#ifndef BUTTON_H
#define BUTTON_H
#include "stm32746g_discovery_lcd.h"
class Button
{

public:
    Button(int x = 50, int y = 50, int width = 50, int height = 30, uint32_t bgColor = LCD_COLOR_LIGHTGRAY, uint32_t borderWidth = 1);
    void setText(const char *str, uint32_t textColor = LCD_COLOR_BLACK);
    bool contain(int x, int y);
    void draw();
    int strlen(uint8_t *str);
    bool etat(){return m_etat;}
    
private :
    bool m_etat = false;
    int16_t m_x = 0;
    int16_t m_y = 0;
    int16_t m_width = 50;
    int16_t m_height = 30;
    uint32_t m_bgColor = LCD_COLOR_LIGHTGRAY;
    uint32_t m_borderColor = LCD_COLOR_GRAY;
    uint32_t m_textColor = LCD_COLOR_BLACK;
    uint32_t m_borderWidth = 1;
    uint8_t m_text[30];
};
#endif
