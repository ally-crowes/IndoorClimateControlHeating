#ifndef __RELAY_DEVICE_H__
#define __RELAY_DEVICE_H__

class RelayDevice {
private:
    int pin;
    bool status = false;
    unsigned int count = 0;
    void ToggleCondition(bool status);

public:
    RelayDevice(int pin);
    bool getCondition();
    void setCondition(bool status);
    unsigned int getCount();
};

#endif
