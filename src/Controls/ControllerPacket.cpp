#include "ControllerPacket.h"



ControllerPacket::ControllerPacket(MenuLCD1602 *controller)
{
    this->controller = controller;
}

void ControllerPacket::update()
{
  this->update(curentMenu);
}

void ControllerPacket::update(int incomingByte)
{
  switch (incomingByte)
  {
  case 0x2B:
    plus();
    break;
  case 0x2D:
    minus();
    break;
  case 0x53:
    set();
    break;

  default:
    curentMenu = incomingByte;
    listMenu();
    //controller->printException(3, curentMenu);
    break;
  }
}

void ControllerPacket::listMenu()
{
  switch (curentMenu)
  {
  case 0x30:
    controller->printMainMenu(curentSubMenu);
    break;
  case 0x33:
    controller->printMainMenu(1);
    break;
  case 0x34:
    controller->printMainMenu(2);
    break;
  case 0x31:
    controller->printIndoorSubmenu(curentSubMenu);
    break;
  case 0x35:
    controller->printIndoorSubmenu(1);
    break;
  case 0x36:
    controller->printIndoorSubmenu(2);
    break;
  case 0x32:
    controller->printEngineSubmenu(curentSubMenu);
    break;
  case 0x37:
    controller->printEngineSubmenu(1);
    break;
  case 0x38:
    controller->printEngineSubmenu(2);
    break;

  default:
    controller->printException(1, curentMenu);
    break;
  }
}

void ControllerPacket::plus()
{
  //if ()
  switch (curentSubMenu)
  {
  case 0:
    curentSubMenu++;
    listMenu();
    break;
  case 1:
    curentSubMenu++;
    listMenu();
    break;
  case 2:
    curentSubMenu = 0;
    listMenu();
    break;
  
  default:
    break;
  }
}

void ControllerPacket::minus()
{
  switch (curentSubMenu)
  {
  case 0:
    curentSubMenu = 2;
    listMenu();
    break;
  case 1:
    curentSubMenu--;
    listMenu();
    break;
  case 2:
    curentSubMenu--;
    listMenu();
    break;
  
  default:
    break;
  }
}

void ControllerPacket::set()
{
  if (curentMenu == 0x30)
  {
    if (curentSubMenu == 1)
    {
      curentMenu = 0x31;
      curentSubMenu = 0;
      listMenu();
    }
    else if (curentSubMenu == 2)
    {
      curentMenu = 0x32;
      curentSubMenu = 0;
      listMenu();
    }
    //controller->printException(4, curentMenu);
    listMenu();
  }
  else
  {
    controller->printException(2, curentMenu);
  }
}