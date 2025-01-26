#include "ControllerPacket.h"

const char *errorNames[] = {
        "update",       // 0
        "updateScreen", // 1
        "setSwitch",    // 2
        "adapter",      // 3
        "set",          // 4
        "Breakpoint",   // 5
    };

ControllerPacket::ControllerPacket(MenuLCD1602 *menu, ControllerEngine* controlEngine, ControllerIndoor* controlIndoor)
{
    this->menuLCD1602 = menu;
    this->controlEngine = controlEngine;
    this->controlIndoor = controlIndoor;
}

void ControllerPacket::update()
{
  updateScreen();
}

void ControllerPacket::update(int incomingByte)
{
  switch (incomingByte)
  {
  case 0x2B: // plus
    if (curentMenu == 0x39 || curentMenu == 0x40 )
    {
      plusTemperature();
    }
    else
    {
      plusSubmenu();
    }
    break;
  case 0x2D: // minus
    if (curentMenu == 0x39 || curentMenu == 0x40)
    {
      minusTemperature();
    }
    else
    {
      minusSubmenu();
    }
    break;
  case 0x73: // set
    set();
    menuLCD1602->clear();
    break;

  default:
    //menuLCD1602->printException(errorNames[0], curentMenu);
        
    if (oldMenu != incomingByte)
    {
      curentMenu = incomingByte;
      menuLCD1602->clear();
      adapterSerial();
    }
    
    oldMenu = incomingByte;
    break;
  }

  Serial.print("incomingByte-");
  Serial.print(incomingByte);
  Serial.print(" oldMenu-");
  Serial.print(oldMenu);
  Serial.print(" curentMenu-");
  Serial.print(curentMenu);
  Serial.print(" curentSubMenu-");
  Serial.println(curentSubMenu);

  updateScreen();
}

void ControllerPacket::plusSubmenu()
{
  switch (curentSubMenu)
  {
  case 0:
    curentSubMenu++;
    break;
  case 1:
    curentSubMenu++;
    break;
  case 2:
    curentSubMenu = 0;
    break;
  
  default:
    break;
  }

  updateScreen();
}

void ControllerPacket::minusSubmenu()
{
  switch (curentSubMenu)
  {
  case 0:
    curentSubMenu = 2;
    break;
  case 1:
    curentSubMenu--;
    break;
  case 2:
    curentSubMenu--;
    break;
  
  default:
    break;
  }

  updateScreen();
}

void ControllerPacket::plusTemperature()
{
  float plus = 0;

  switch (curentMenu)
  {
  case 0x39:
    if (curentSubMenu == 1)
    {
      plus = controlIndoor->GetMinTemperature();
      controlIndoor->SetMinTemperature(plus + 0.1);
      Serial.print(" indoorMin+");
    }
    else if (curentSubMenu == 2)
    {
      plus = controlIndoor->GetMaxTemperature();
      controlIndoor->SetMaxTemperature(plus + 0.1);
      Serial.print(" indoorMax+");
    }
    break;
  case 0x40:
    if (curentSubMenu == 1)
    {
      plus = controlEngine->GetMinTemperature();
      controlEngine->SetMinTemperature(plus + 0.1);
      Serial.print(" engineMin-");
    }
    else if (curentSubMenu == 2)
    {
      plus = controlEngine->GetMaxTemperature();
      controlEngine->SetMaxTemperature(plus + 0.1);
      Serial.print(" engineMin-");
    }
    break;

  default:
    break;
  }
  
  Serial.println(plus);
}

void ControllerPacket::minusTemperature()
{
  float minus = 0;

  switch (curentMenu)
  {
  case 0x39:
    if (curentSubMenu == 1)
    {
      minus = controlIndoor->GetMinTemperature();
      controlIndoor->SetMinTemperature(minus - 0.1);
      Serial.print(" minus1-");
    }
    else if (curentSubMenu == 2)
    {
      minus = controlIndoor->GetMaxTemperature();
      controlIndoor->SetMaxTemperature(minus - 0.1);
      Serial.print(" minus2-");
    }
    break;
  case 0x40:
    if (curentSubMenu == 1)
    {
      minus = controlEngine->GetMinTemperature();
      controlEngine->SetMinTemperature(minus - 0.1);
      Serial.print(" minus1-");
    }
    else if (curentSubMenu == 2)
    {
      minus = controlEngine->GetMaxTemperature();
      controlEngine->SetMaxTemperature(minus - 0.1);
      Serial.print(" minus2-");
    }
    break;

  default:
    break;
  }
  
  Serial.println(minus);
}

void ControllerPacket::set()
{
  switch (curentMenu)
  {
  case 0x30:
    if (curentSubMenu == 1) { curentMenu = 0x31; curentSubMenu = 0; }
    if (curentSubMenu == 2) { curentMenu = 0x32; curentSubMenu = 0; }
    break;
  case 0x31: // Indoor
    if (curentSubMenu == 0) { curentMenu = 0x30; }
    if (curentSubMenu == 1) { curentMenu = 0x39; }
    if (curentSubMenu == 2) { curentMenu = 0x39; }
    break;
  case 0x32: // Engine
    if (curentSubMenu == 0) { curentMenu = 0x30; }
    if (curentSubMenu == 1) { curentMenu = 0x40; }
    if (curentSubMenu == 2) { curentMenu = 0x40; }
    break;
  case 0x39: // Indoor_Min+-Set
    if (curentSubMenu == 0) { curentMenu = 0x31; }
    if (curentSubMenu == 1) { curentMenu = 0x31; }
    if (curentSubMenu == 2) { curentMenu = 0x31; }
    break;
  case 0x40: // Engine_Min+-Set
    if (curentSubMenu == 0) { curentMenu = 0x32; }
    if (curentSubMenu == 1) { curentMenu = 0x32; }
    if (curentSubMenu == 2) { curentMenu = 0x32; }
    break;
  //case 0x71: // q
    //menuLCD1602->blink = true;
    //break;
  
  default:
    menuLCD1602->printException(errorNames[2], curentMenu);
    delay(2000);
    break;
  }

}

// For collaborate witch Serial Packet
void ControllerPacket::adapterSerial()
{
  switch (curentMenu)
  {
  case 0x30:
    curentMenu = 0x30; curentSubMenu = 0;
    break;
  case 0x33:
    curentMenu = 0x30; curentSubMenu = 1;
    break;
  case 0x34:
    curentMenu = 0x30; curentSubMenu = 2;
    break;
  case 0x31:
    curentMenu = 0x31; curentSubMenu = 0;
    break;
  case 0x35:
    curentMenu = 0x31; curentSubMenu = 1;
    break;
  case 0x36:
    curentMenu = 0x31; curentSubMenu = 2;
    break;
  case 0x32:
    curentMenu = 0x32; curentSubMenu = 0;
    break;
  case 0x37:
    curentMenu = 0x32; curentSubMenu = 1;
    break;
  case 0x38:
    curentMenu = 0x32; curentSubMenu = 2;
    break;
  case 0x71: // q
    curentMenu = 0x40; curentSubMenu = 0;
    break;

  default:
    menuLCD1602->printException(errorNames[3], curentMenu);
    delay(2000);
    break;
  }
}

void ControllerPacket::updateScreen()
{
  switch (curentMenu)
  {
  case 0x30:
    menuLCD1602->printMainmenu(curentSubMenu);
    break;
  case 0x31:
    menuLCD1602->printIndoorSubmenu(curentSubMenu);
    break;
  case 0x32:
    menuLCD1602->printEngineSubmenu(curentSubMenu);
    break;
  case 0x39:
    //menuLCD1602->printEngineSubmenu(curentSubMenu, MenuLCD1602::MenuCondition::TempMinEngine);
    menuLCD1602->printIndoorSubmenu(curentSubMenu);
    break;
  case 0x40:
    //menuLCD1602->printEngineSubmenu(curentSubMenu, MenuLCD1602::MenuCondition::TempMinEngine);
    menuLCD1602->printEngineSubmenu(curentSubMenu);
    break;
  case 0x71: // q
    menuLCD1602->printException(errorNames[5], curentMenu);// delay(2000); 
    break;

  default:
    menuLCD1602->printException(errorNames[1], curentMenu);
    delay(100);
    break;
  }
}
