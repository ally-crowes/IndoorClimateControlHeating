#ifndef __CONTROLLERPACKET_H__
#define __CONTROLLERPACKET_H__
#include "Display\MenuLCD1602.h"

//#include "includes.h"

class ControllerPacket
{
    private:
        MenuLCD1602* controller;
        int curentMenu = 0x30;
        int curentSubMenu = 0;
        void plus();
        void minus();
        void set();
        void listMenu();
    public:
        ControllerPacket(MenuLCD1602 *controller);
        void update();
        void update(int menu);
};

#endif