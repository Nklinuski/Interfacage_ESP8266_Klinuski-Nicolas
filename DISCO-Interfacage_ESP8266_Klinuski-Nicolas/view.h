#ifndef VIEW_H
#define VIEW_H
#include "mbed.h"
#include "button.h"
#include "stm32746g_discovery_lcd.h"
#include <list>
#define SCREENWIDTH 480
#define SCREENHEIGHT 272
#include <ESP8266.h>
#include <stdint.h>

extern RawSerial pc;
extern RawSerial wifi;


class View
{
public:
    View(int width = SCREENWIDTH, int height = SCREENHEIGHT);
    void contain(int x, int y);
    void draw();
    ~View();
    void updateLoop();
    void drawImage(int offsetX, int offsetY);


private :
    int16_t m_width = SCREENWIDTH;
    int16_t m_height = SCREENHEIGHT;
    uint32_t m_bgColor = LCD_COLOR_WHITE;
    list<Button *> m_buttonList;

    ESP8266 * m_esp;

};

#endif
