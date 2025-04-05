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
  case 0x2B: // '+' plus 43dec
    navigationPlus();
    break;
  case 0x2D: // '-' minus 45dec
    navigationMinus();
    break;
  case 0x73: // 's' set 115dec
    navigationSet();
    menuLCD1602->clear();
    oldMenu = incomingByte;
    break;

  default:  // other incoming byte
    //menuLCD1602->printException(errorNames[0], curentMenu);
        
    // if (oldMenu != incomingByte)
    // {
    //   adapterSerial();
    //   menuLCD1602->clear();
    // }
    
    adapterSerial(incomingByte);
    menuLCD1602->clear();
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
  Serial.print(curentSubMenu);
  Serial.print(" edit-");
  Serial.println(edit);

  updateScreen();
}

void ControllerPacket::navigationPlus()
{
  if ( !edit )
  {
    submenuPlus();
  }
  else if ( curentMenu == 0x31 || curentMenu == 0x32 )
  {
    temperaturePlus();
  }
}

void ControllerPacket::navigationMinus()
{
  if ( !edit )
  {
    submenuMinus();
  }
  else if (curentMenu == 0x31 || curentMenu == 0x32 )
  {
    temperatureMinus();
  }
}

void ControllerPacket::navigationSet()
{
  switch (curentMenu)
  {
  case 0x30:
    if (curentSubMenu == 0) { curentMenu = 0x72; }
    else if (curentSubMenu == 1) { curentMenu = 0x31; curentSubMenu = 0; }
    else if (curentSubMenu == 2) { curentMenu = 0x32; curentSubMenu = 0; }
    break;
  case 0x31: // Indoor
    if (curentSubMenu == 0) { curentMenu = 0x30; }
    else if (curentSubMenu == 1 && !edit) { edit=true; }
    else if (curentSubMenu == 1 &&  edit) { edit=false; curentSubMenu = 0; }
    else if (curentSubMenu == 2 && !edit) { edit=true; }
    else if (curentSubMenu == 2 &&  edit) { edit=false; curentSubMenu = 0; }
    break;
  case 0x32: // Engine
    if (curentSubMenu == 0) { curentMenu = 0x30; }
    else if (curentSubMenu == 1 && !edit) { edit=true; }
    else if (curentSubMenu == 1 &&  edit) { edit=false; curentSubMenu = 0; }
    else if (curentSubMenu == 2 && !edit) { edit=true; }
    else if (curentSubMenu == 2 &&  edit) { edit=false; curentSubMenu = 0; }
    break;
  case 0x71: // q
    menuLCD1602->blink = true;
    break;
  case 0x72: // RTC
    curentMenu = 0x30;
    break;
  
  
  default:
    menuLCD1602->printException(errorNames[2], curentMenu);
    delay(2000);
    break;
  }

}

void ControllerPacket::submenuMinus()
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

void ControllerPacket::submenuPlus()
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

void ControllerPacket::temperaturePlus()
{
  float plus = 0;

  switch (curentMenu)
  {
  case 0x31:
    if (curentSubMenu == 1)
    {
      controlIndoor->ChangeMinUpTemperature();
      //controlIndoorExt->temperatureChangeMinUp();
    }
    else if (curentSubMenu == 2)
    {
      controlIndoor->ChangeMaxUpTemperature();
      // controlIndoorExt->temperatureChangeMaxUp();
    }
    break;
  case 0x32:
    if (curentSubMenu == 1)
    {
      controlEngine->ChangeMinUpTemperature();
    }
    else if (curentSubMenu == 2)
    {
      controlEngine->ChangeMaxUpTemperature();
    }
    break;
    
  default:
    break;
  }
  
  Serial.println(plus);
}

void ControllerPacket::temperatureMinus()
{
  float minus = 0;
  
  switch (curentMenu)
  {
    case 0x31:
    if (curentSubMenu == 1)
    {
      controlIndoor->ChangeMinDownTemperature();
      // controlIndoorExt->temperatureChangeMinDown();
    }
    else if (curentSubMenu == 2)
    {
      controlIndoor->ChangeMaxDownTemperature();
      // controlIndoorExt->temperatureChangeMaxDown();
    }
    break;
  case 0x32:
    if (curentSubMenu == 1)
    {
      controlEngine->ChangeMinDownTemperature();
    }
    else if (curentSubMenu == 2)
    {
      controlEngine->ChangeMaxDownTemperature();
    }
    break;

  default:
    break;
  }
  
  Serial.println(minus);
}

// For collaborate witch Serial Packet
void ControllerPacket::adapterSerial(int incomingByte)
{
  switch (incomingByte)
  {
  case 0x30: // '0'
    curentMenu = 0x30; curentSubMenu = 0;
    break;
  case 0x33: // '3'
    curentMenu = 0x30; curentSubMenu = 1;
    break;
  case 0x34: // '4'
    curentMenu = 0x30; curentSubMenu = 2;
    break;
  case 0x31: // '1'
    curentMenu = 0x31; curentSubMenu = 0;
    break;
  case 0x35: // '5'
    curentMenu = 0x31; curentSubMenu = 1;
    break;
  case 0x36: // '6'
    curentMenu = 0x31; curentSubMenu = 2;
    break;
  case 0x32: // '2'
    curentMenu = 0x32; curentSubMenu = 0;
    break;
  case 0x37: // '7'
    curentMenu = 0x32; curentSubMenu = 1;
    break;
  case 0x38: // '8'
    curentMenu = 0x32; curentSubMenu = 2;
    break;
  case 0x71: // 'q'
    //curentMenu = 0x40; curentSubMenu = 0;
    break;
  case 0x72: // 'r'
    curentMenu = 0x72; curentSubMenu = 0;
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
    menuLCD1602->printMainMenu(curentSubMenu);
    break;
  case 0x31:
    menuLCD1602->printIndoorSubmenu(curentSubMenu);
    break;
  case 0x32:
    menuLCD1602->printEngineSubmenu(curentSubMenu);
    break;
  case 0x39:
    menuLCD1602->printIndoorSubmenu(curentSubMenu);
    break;
  case 0x40:
    menuLCD1602->printEngineSubmenu(curentSubMenu);
    break;
  case 0x71: // q
    menuLCD1602->printException(errorNames[5], curentMenu);// delay(2000); 
    break;
  case 0x72: // 'r'
    menuLCD1602->printRTC();
    break;
  

  default:
    menuLCD1602->printException(errorNames[1], curentMenu);
    delay(100);
    break;
  }
}
