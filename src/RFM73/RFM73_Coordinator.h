#ifndef RFM73_COORDINATOR_H
#define RFM73_COORDINATOR_H

#include <stdint.h>
#include "RFM73_Driver.h"

enum RFM73_CoordWritePayloadStatus_t
{
    RFM73_COORD_NODE_NOT_FOUND_STATUS,
    RFM73_COORD_ACK_BUFFER_FULL_STATUS,
    RFM73_COORD_ACK_WRITE_SUCCESS_STATUS
};

enum RFM73_CoordinatorTxStatus_t
{
    RFM73_COORDINATOR_TX_SUCCESS,
};

struct RFM73_CoordinatorNodeRxInfo_t
{
    uint8_t      SourceAddress;
    RFM73_Pipe_t SourcePipe;
    uint8_t*     pData;
    uint8_t      DataLen;
};

struct RFM73_CoordinatorNodeTxInfo_t
{
    RFM73_CoordinatorTxStatus_t Status;
};

typedef void ( *RFM73_CoordinatorRxCallBack_t )( void* pContext, RFM73_CoordinatorNodeRxInfo_t* rxNodeInfo );
typedef void ( *RFM73_CoordinatorTxCallBack_t )( void* pContext, RFM73_CoordinatorNodeTxInfo_t* txNodeInfo );

class RFM73_Coordinator_t
{
public:
    void SetRxCallBack( void* pContext, RFM73_CoordinatorRxCallBack_t fp_CallBack );
    void SetTxCallBack( void* pContext, RFM73_CoordinatorTxCallBack_t fp_CallBack );
    void FlushPendingAckPayload();
    
    void Init( RFM73_ControlInterface_t* pControlInterface );
    RFM73_CoordWritePayloadStatus_t WriteAckPayload( uint8_t nodeAddr, uint8_t* pData, uint8_t len );
    void Run();
    
private:
    uint8_t GetRxSourceNodeAddr();
    RFM73_Pipe_t GetPipeFromNodeAddress( uint8_t nodeAddr ); 
    uint8_t *GetPipeAddrFromNodeAddr( uint8_t addr );
             
private:
    RFM73_CoordinatorRxCallBack_t fp_RxCallBack;
    RFM73_CoordinatorTxCallBack_t fp_TxCallBack;
    void*                         m_pRxContext;
    void*                         m_pTxContext;
    uint8_t                       m_RxData[32];
    uint8_t                       m_NetworkAddr[ 3 ];
private:
    RFM73_t                       m_RadioModule;
};



#endif