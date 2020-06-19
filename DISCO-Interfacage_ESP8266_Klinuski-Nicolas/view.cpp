#include "view.h"
#include "stm32746g_discovery_lcd.h"
#include "mbed.h"
#include "button.h"
#include <list>
#include <string.h>
#include <stdint.h>
#include "iut.h"



RawSerial pc(USBTX, USBRX, 115200);
RawSerial wifi(D1, D0, 115200);


uint8_t donnee[100];
uint8_t donneecpy[100];
uint8_t valReaded = 0;
uint8_t dataReaded = 0;
uint8_t isInit = 0;

void View::drawImage(int offsetX, int offsetY)                                                             //Affiche l'image du iut.h au coordonnées demandé
{
    int x = 0;
    int y = 0;
    uint32_t* dataPtr = (uint32_t*)iut.data;
    while(y < iut.height)
    {
        while(x <iut.width)
        {
            BSP_LCD_DrawPixel(x + offsetX, y + offsetY, *dataPtr);
            dataPtr++;
            x++;
        }
        x = 0;
        y++;
    }
}

void reception()                                                                                          //Recupère les données du serveur et les envoies sur l'écran
{
    
    donnee[valReaded] = wifi.getc();
    pc.putc(donnee[valReaded]);
    
    if (isInit == 1) {
        if (donnee[valReaded] == '\n' ||  donnee[valReaded] == '\0' ||  donnee[valReaded] == '\r')       //Détection de valeur \n, \0 ou \r dans la donnée reçu
        { 
            if (valReaded > 0)
            {
                    donnee[valReaded] = 0;
                    donnee[0];
                    memset(donneecpy,0,100);
                    if (valReaded <= 9 )                                                                 //Si la valeur est supérieur ou égal à 9 alors supprimer les 7 premiers caractères (caractère invisible de base)
                    {
                        memcpy(donneecpy, &(donnee[7]), (valReaded > 100) ? 100 : valReaded);            // Si la chaine de caractère est supérieur à 100, alors mettre seulement les 100 premiers
                    }
                    if (valReaded > 9 )                                                                  //Si la valeur est supérieur ou égal à 9 alors supprimer les 8 premiers caractères (caractère invisible de base)
                    {
                        memcpy(donneecpy, &(donnee[8]), (valReaded > 100) ? 100 : valReaded);
                    }
                    memset(donnee,0,100);
                    valReaded = 0;
            }
        } 
        else {
            valReaded++;
        }
        dataReaded = 1;
    }

}

View::View(int width, int height)
    : m_width(width), m_height(height)
{
    
    Button *butDefault = new Button(130, 200, 70);                                                      //Position et config des boutons
    Button *but = new Button(260,200, 90, 30, LCD_COLOR_YELLOW);
    butDefault->setText("Connexion");
    but->setText("Deconnexion" );
    m_buttonList.push_front(butDefault);
    m_buttonList.push_front(but);
    wifi.printf("AT+CWMODE_CUR=1\r\n");                                                                 //Configuration du mode Wi-Fi
    wifi.attach(&reception);
    
}

View::~View()                                                                                           //Destructeur de bouton
{
    for (Button * it : m_buttonList) {
        delete it;
    }
    m_buttonList.clear();
}

void View::updateLoop()                                                                                 //Affiche le message reçu
{
   
    if (dataReaded != 0) {
        BSP_LCD_SetBackColor(m_bgColor);
        BSP_LCD_ClearStringLine(5);
        BSP_LCD_DisplayStringAt(0, LINE(5), (uint8_t *)"Message Recu :", CENTER_MODE);
        BSP_LCD_ClearStringLine(6);
        BSP_LCD_DisplayStringAt(0, LINE(6), (uint8_t *)&donneecpy, CENTER_MODE);
        dataReaded = 0;

    }

}

void View::contain(int x, int y)
{

    int idBut = 0;

    for (Button * it : m_buttonList) {

        bool memoetat = it->etat();
        if (it->contain(x,y)) {
            if (idBut == 0) {                                                                           //Bouton déconnexion
                BSP_LCD_DisplayStringAt(0, LINE(4),(uint8_t *)"Deconnection en cours", CENTER_MODE);
                if (!memoetat) {
                    isInit = 0;
                    m_bgColor = LCD_COLOR_LIGHTRED;
                    BSP_LCD_ClearStringLine(5);
                    BSP_LCD_ClearStringLine(6);  
                    wifi.printf("AT+CWQAP\r\n");                                                        //Déconnexion de la wifi
                    wait(1);
                    BSP_LCD_DisplayStringAt(0, LINE(5),(uint8_t *)"Deconnection OK", CENTER_MODE);
                    wait(2);
                    View::draw();
                }
            } else {                                                                                    //Bouton connexion                                          
                BSP_LCD_DisplayStringAt(0, LINE(4),(uint8_t *)"Connection en cours", CENTER_MODE);
                if (!memoetat) {
                m_bgColor = LCD_COLOR_LIGHTGREEN; 
                    wifi.printf("AT+CWJAP_CUR=\"Nicobox 4.0\",\"skaynis00\"\r\n");                      //Connexion à la wifi
                    wait(5);
                    wifi.printf("AT+CIPSTART=\"TCP\",\"192.168.43.75\",4242\r\n");                      //Connection sur le serveur TCP (dans mon cas, 192.168.43.75 pour pc portable, 192.168.43.193 pour pc fixe)
                    wait(1);
                    BSP_LCD_DisplayStringAt(0, LINE(5),(uint8_t *)"Connection OK", CENTER_MODE);
                    wait(2);
                    View::draw();
                    isInit = 1;

                    
                }

            }
        }
        idBut++;
    }

}


void View::draw()
{
    BSP_LCD_Clear(m_bgColor);                                                                          //Enlève l'affichage
    for (Button * it : m_buttonList) {
        it->draw();
    }
}
