#ifndef RFM73_SLEEP_DEVICE_H
#define RFM73_SLEEP_DEVICE_H

#include <stdint.h>
#include "RFM73_Driver.h"



enum RFM73_SleepDeviceTxStatus_t
{
    RFM73_STACK_TX_SUCCESS,
    RFM73_STACK_TX_TIMEOUT,
};

struct RFM73_SleepDeviceNodeTxInfo_t
{
    RFM73_SleepDeviceTxStatus_t Status;
    uint8_t*                    pAckData;
    uint8_t                     AckDataLen;
};

typedef void ( *RFM73_StackTxCallBack_t )( void* pContext, RFM73_SleepDeviceNodeTxInfo_t* txNodeInfo );


class RFM73_SleepDevice_t
{
public:
    void SetTxCallBack( void* pContext, RFM73_StackTxCallBack_t fp_CallBack );
    void SetNodeAddress( uint32_t nodeAddr );
    
    

    void Init( RFM73_ControlInterface_t* pControlInterface );
    void SendData( uint8_t nodeAddr, uint8_t* pData, uint8_t len, bool ackEnable );
    void Run();
    void PowerDown();
    void PowerUp();
    
    
private:
  
    uint8_t *GetPipeAddrFromNodeAddr( uint8_t addr );
             
private:
    RFM73_StackTxCallBack_t fp_TxCallBack;
    void*                   m_pRxContext;
    void*                   m_pTxContext;
    uint32_t                m_NodeAddress;    
    uint8_t                 m_RxData[100];
    uint8_t                 m_NetworkAddr[ 3 ];
    bool                    m_IsPowerDownMode;
private:
    RFM73_t                 m_RadioModule;
   
};

#endif