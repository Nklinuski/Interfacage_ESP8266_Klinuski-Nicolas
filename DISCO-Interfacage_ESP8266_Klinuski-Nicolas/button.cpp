#include "button.h"
#include "mbed.h"
#include "stm32746g_discovery_lcd.h"

int Button::strlen(uint8_t *str)
{
    int i = 0;
    while(str[i])
    {
        i++;
    }
    return i;
}


Button::Button(int x, int y, int width, int height, uint32_t bgColor, uint32_t borderWidth)                                                     //Constructeur de bouton
    : m_x(x), m_y(y), m_width(width), m_height(height), m_bgColor(bgColor), m_borderWidth(borderWidth)
{
    
}

bool Button::contain(int x, int y)                                                                                                              //detection du x et y, vérification si c'est dans le bouton                                           
{
    m_etat = (x > m_x && x < m_x + m_width && y > m_y && y < m_y + m_height);
    return m_etat;
}

void Button::draw()                                                                                                                             //Dessin du bouton
{
    int x = m_x;
    while (x < m_x + m_width) {
        int y = m_y;
        while (y < m_y + m_height) {
            if (y - m_y < m_borderWidth)
                BSP_LCD_DrawPixel(x, y, m_borderColor);
            else if ( (m_y + m_height) - y <= m_borderWidth)
                BSP_LCD_DrawPixel(x, y, m_borderColor);
            else if (x - m_x < m_borderWidth)
                BSP_LCD_DrawPixel(x, y, m_borderColor);
            else if ( (m_x + m_width) - x <= m_borderWidth)
                BSP_LCD_DrawPixel(x, y, m_borderColor);
            else
                BSP_LCD_DrawPixel(x, y, m_bgColor);
            
            y++;
        }
        x++;
    }
    BSP_LCD_SetTextColor(m_textColor);
    BSP_LCD_SetBackColor(m_bgColor);
    BSP_LCD_DisplayStringAt((m_x + m_width/2)- 8*(strlen(m_text)/2), (m_y + m_height/2) - 5, (uint8_t *)&m_text, LEFT_MODE);

}


void Button::setText(const char *str, uint32_t textColor){                                                                                   //Texte du bouton
    m_textColor = textColor;
    sprintf((char*)m_text, str);
     
}
