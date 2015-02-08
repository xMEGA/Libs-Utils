#ifndef RFM73_CONTROL_INTERFACE_H
#define RFM73_CONTROL_INTERFACE_H

#include <stdint.h>


class RFM73_ControlInterface_t
{
public:  
    virtual void ConnectionInit() = 0; 
    virtual void ChipEnableClr() = 0;
    virtual void ChipEnableSet() = 0;  
    virtual void SpiChipSelectClr() = 0;
    virtual void SpiChipSelectSet() = 0;
    virtual uint8_t SpiRead() = 0;
    virtual uint8_t SpiWrite( uint8_t byte ) = 0;
    virtual void DelayMsec( uint32_t msec ) = 0;
    virtual void EnterPinsToLowPower() = 0;
    virtual void ExitPinsFromLowPower() = 0;
};



#endif