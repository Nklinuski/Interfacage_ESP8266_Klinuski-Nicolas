#include "mbed.h"
#include "button.h"
#include "view.h"
#include "stm32746g_discovery_lcd.h"
#include "stm32746g_discovery_ts.h"
#include <list>
#define SCREENWIDTH 480
#define SCREENHEIGHT 272

uint8_t text[30];

int main()
{
    TS_StateTypeDef TS_State;
    uint16_t x, y;
    uint8_t status;
    uint8_t idx;
    uint8_t cleared = 0;
    uint8_t prev_nb_touches = 0;

    BSP_LCD_Init();                                                                             //Initialise le LCD
    BSP_LCD_LayerDefaultInit(LTDC_ACTIVE_LAYER, LCD_FB_START_ADDRESS);
    BSP_LCD_SelectLayer(LTDC_ACTIVE_LAYER);

    status = BSP_TS_Init(BSP_LCD_GetXSize(), BSP_LCD_GetYSize());
    HAL_Delay(100);
    BSP_LCD_SetFont(&Font12);
    View v;
    v.draw();


    while(1) {
        
        v.updateLoop();
        v.drawImage(320, 0);                                                                    //Affichage de l'image
        BSP_TS_GetState(&TS_State);
        if (TS_State.touchDetected) {                                                           //Detecte si l'écran tactile est touché
            cleared = 0;
            sprintf((char*)text, "Touches: %d", TS_State.touchDetected);
            BSP_LCD_DisplayStringAt(0, LINE(0), (uint8_t *)&text, LEFT_MODE);
            for (idx = 0; idx < TS_State.touchDetected; idx++) {
                x = TS_State.touchX[idx];
                y = TS_State.touchY[idx];
                v.contain(x, y);

            }


        } else {
            if (!cleared) {
                sprintf((char*)text, "Touches: 0");
                BSP_LCD_DisplayStringAt(0, LINE(0), (uint8_t *)&text, LEFT_MODE);
                cleared = 1;

            }
        }
    }
}
