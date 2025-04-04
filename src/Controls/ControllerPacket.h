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
    bool edit = false;
    void navigationPlus();
    void navigationMinus();
    void navigationSet();
    void submenuPlus();
    void submenuMinus();
    void temperaturePlus();
    void temperatureMinus();
    void updateScreen();
    void adapterSerial(int incomingByte); // For collaborate witch Serial Packet
public:
    ControllerPacket(MenuLCD1602 *menu, ControllerEngine* controlEngine, ControllerIndoor* controlIndoor);
    void update();
    void update(int incomingByte);
};

#endif