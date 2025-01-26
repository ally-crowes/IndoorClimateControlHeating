#ifndef __CONTROLLERPACKET_H__
#define __CONTROLLERPACKET_H__
#include "Display\MenuLCD1602.h"

//#include "includes.h"

class ControllerPacket
{
private:
    
    enum MenuCondition
    {
        BuildMenu,
        TempMinEngine,
        TempMaxEngine,
    };

    MenuLCD1602 *menuLCD1602;
    ControllerEngine* controlEngine;
    ControllerIndoor* controlIndoor;

    int curentMenu = 0x30;
    int oldMenu = 0;
    int curentSubMenu = 0;
    void plusSubmenu();
    void minusSubmenu();
    void plusTemperature();
    void minusTemperature();
    void set();
    void updateScreen();
    void adapterSerial(); // For collaborate witch Serial Packet
public:
    ControllerPacket(MenuLCD1602 *menu, ControllerEngine* controlEngine, ControllerIndoor* controlIndoor);
    void update();
    void update(int menu);
};

#endif