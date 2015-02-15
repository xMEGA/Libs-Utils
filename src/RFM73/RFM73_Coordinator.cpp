#include "RFM73_Coordinator.h"
#include "RFM73_Config.h"


void RFM73_Coordinator_t::SetRxCallBack( void* pContext, RFM73_CoordinatorRxCallBack_t fp_CallBack )
{
    m_pRxContext  = pContext;
    fp_RxCallBack = fp_CallBack;
}

void RFM73_Coordinator_t::SetTxCallBack( void* pContext, RFM73_CoordinatorTxCallBack_t fp_CallBack )
{
    m_pTxContext  = pContext;
    fp_TxCallBack = fp_CallBack;
}

void RFM73_Coordinator_t::Init( RFM73_ControlInterface_t* pControlInterface )
{
    m_RadioModule.Init( pControlInterface );  
    
    m_RadioModule.ChipEnableClr();
    uint32_t chipId = m_RadioModule.GetChipId();
    
  //  RFM73_StatusReg_t statusReg = m_RadioModule.Nop();
    
  // m_RadioModule.InitBank1();
   
    m_RadioModule.SetCrcMode( RFM73_CRC_16BIT );
    
    m_RadioModule.SetAddressWidth( RFM73_ADDRESS_WIDTH_3BYTES );
    m_RadioModule.SetRfChannel( RFM73_STACK_RF_CHANNEL );
    m_RadioModule.SetDatarate( RFM73_STACK_DATARATE );
    m_RadioModule.EnableLnaGain( RFM73_STACK_LDA_GAIN_ENABLE );
    m_RadioModule.SetOutputPower( RFM73_STACK_OUTPUT_POWER );

    m_RadioModule.FeaturesActivate();
    
    m_RadioModule.DelayUsec( 30000 );
 
    m_RadioModule.EnableAckPayload( true );
    m_RadioModule.EnableDynamicPayload( true );
    m_RadioModule.EnableNoAckTxPayload( true );
     
    m_RadioModule.DelayUsec( 30000 );
 
    
    m_RadioModule.OpenPipe( RFM73_PIPE0, GetPipeAddrFromNodeAddr( RFM73_STACK_RX_ADDR_PIPE0 ), true, true );
    m_RadioModule.OpenPipe( RFM73_PIPE1, GetPipeAddrFromNodeAddr( RFM73_STACK_RX_ADDR_PIPE1 ), true, true );
    m_RadioModule.OpenPipe( RFM73_PIPE2, GetPipeAddrFromNodeAddr( RFM73_STACK_RX_ADDR_PIPE2 ), true, true );
    m_RadioModule.OpenPipe( RFM73_PIPE3, GetPipeAddrFromNodeAddr( RFM73_STACK_RX_ADDR_PIPE3 ), true, true );
    m_RadioModule.OpenPipe( RFM73_PIPE4, GetPipeAddrFromNodeAddr( RFM73_STACK_RX_ADDR_PIPE4 ), true, true );
    m_RadioModule.OpenPipe( RFM73_PIPE5, GetPipeAddrFromNodeAddr( RFM73_STACK_RX_ADDR_PIPE5 ), true, true );

    m_RadioModule.FlushRxFifo();
    m_RadioModule.SetPrimaryRxMode();
    
    m_RadioModule.ChipEnableSet();
    
    
    m_RadioModule.EnterToStandByModeI();
}

uint8_t* RFM73_Coordinator_t::GetPipeAddrFromNodeAddr( uint8_t addr )
{
    m_NetworkAddr[ 0 ] = addr;
    m_NetworkAddr[ 1 ] = ( uint8_t )RFM73_STACK_NETWORK_ADDRESS;
    m_NetworkAddr[ 2 ] = RFM73_STACK_NETWORK_ADDRESS >> 8;
 
    return m_NetworkAddr;
}

uint8_t RFM73_Coordinator_t::GetRxSourceNodeAddr()
{
    uint8_t address[ 5 ];
    RFM73_Pipe_t dataSourcePipe = m_RadioModule.GetRxDataSource();
    m_RadioModule.GetRxPipeAddress( dataSourcePipe, address );
    uint8_t sourceNodeAddr = address[ 0 ];
    return sourceNodeAddr;
}

void RFM73_Coordinator_t::Run()
{
    if( true == m_RadioModule.GetRxDataReadyIrqFlag() )
    {
        uint8_t len = m_RadioModule.GetRxPayloadWidth();
      
        RFM73_Pipe_t sourcePipe = m_RadioModule.GetRxDataSource();
        uint8_t sourceNodeAddr  = GetRxSourceNodeAddr();

        m_RadioModule.ClearIrqFlag( RFM73_RX_DR );
        m_RadioModule.ReadRxPayload( m_RxData, len );
             
        RFM73_CoordinatorNodeRxInfo_t rxInfo;
        rxInfo.SourceAddress = sourceNodeAddr;
        rxInfo.SourcePipe    = sourcePipe;
        rxInfo.pData         = m_RxData;
        rxInfo.DataLen       = len;
        
        if( len > 0 )
        {
            fp_RxCallBack( m_pRxContext, &rxInfo );
        }
    }
    
    if( true == m_RadioModule.GetTxDataSentIrqFlag() )
    {
       m_RadioModule.ClearIrqFlag( RFM73_TX_DS );
       RFM73_CoordinatorNodeTxInfo_t txInfo;
       txInfo.Status = RFM73_COORDINATOR_TX_SUCCESS;
       if( 0 != fp_TxCallBack )
       {
           fp_TxCallBack( m_pTxContext, &txInfo );
       }    
    }
}

void RFM73_Coordinator_t::FlushPendingAckPayload()
{
    m_RadioModule.FlushTxFifo();  
}
    
RFM73_Pipe_t RFM73_Coordinator_t::GetPipeFromNodeAddress( uint8_t nodeAddr )
{
    RFM73_Pipe_t pipeCollection[] = { RFM73_PIPE0, RFM73_PIPE1, RFM73_PIPE2, 
                                      RFM73_PIPE3, RFM73_PIPE4, RFM73_PIPE5 };
    
    RFM73_Pipe_t foundPipe = RFM73_PIPE_UNKNOWN;
    uint8_t pipesCnt = sizeof( pipeCollection ) / sizeof( RFM73_Pipe_t );
    
    for( uint8_t idx = 0; idx != pipesCnt ; idx ++ )
    {
        uint8_t pipeAddr[ 5 ] = { 0 };
        RFM73_Pipe_t pipe = pipeCollection[ idx ];    
        
        m_RadioModule.GetRxPipeAddress( pipe, pipeAddr );
     
        if( pipeAddr[ 0 ] == nodeAddr )
        {
            foundPipe = pipe; 
            break;
        }
    }
    
    return foundPipe;
}

RFM73_CoordWritePayloadStatus_t RFM73_Coordinator_t::WriteAckPayload( uint8_t nodeAddr, uint8_t* pData, uint8_t len )
{
    RFM73_CoordWritePayloadStatus_t status = RFM73_COORD_ACK_BUFFER_FULL_STATUS;

    if( false == m_RadioModule.IsTxFifoFull() )
    {
        status = RFM73_COORD_NODE_NOT_FOUND_STATUS;
        
        RFM73_Pipe_t pipe = GetPipeFromNodeAddress( nodeAddr );
        
        if( RFM73_PIPE_UNKNOWN != pipe )
        {
            m_RadioModule.WriteAckPayload( pipe , pData, len );
            status = RFM73_COORD_ACK_WRITE_SUCCESS_STATUS;
        }
    }

    return status;
}