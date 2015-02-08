#include "RFM73_SleepDevice.h"
#include "RFM73_Config.h"


void RFM73_SleepDevice_t::SetTxCallBack( void* pContext, RFM73_StackTxCallBack_t fp_CallBack )
{
    m_pTxContext  = pContext;
    fp_TxCallBack = fp_CallBack;
}

void RFM73_SleepDevice_t::SetNodeAddress( uint32_t nodeAddr )
{
    m_NodeAddress = nodeAddr;
}
  
void RFM73_SleepDevice_t::Init( RFM73_ControlInterface_t* pControlInterface )
{
    m_IsPowerDownMode = true;
  
    m_RadioModule.Init( pControlInterface );  
    
    m_RadioModule.ChipEnableClr();
    static uint32_t chipId = m_RadioModule.GetChipId();
    
    RFM73_StatusReg_t statusReg = m_RadioModule.Nop();
    
   // m_RadioModule.InitBank1();

    m_RadioModule.SetCrcMode( RFM73_CRC_16BIT );
    
    m_RadioModule.SetAddressWidth( RFM73_ADDRESS_WIDTH_3BYTES );
    m_RadioModule.SetRfChannel( RFM73_STACK_RF_CHANNEL );
    m_RadioModule.SetDatarate( RFM73_STACK_DATARATE );
    m_RadioModule.EnableLnaGain( RFM73_STACK_LDA_GAIN_ENABLE );
    m_RadioModule.SetOutputPower( RFM73_STACK_OUTPUT_POWER );
    m_RadioModule.SetAutoRetransmission( 15, 5 );
    
    m_RadioModule.FeaturesActivate();
 
    m_RadioModule.EnableAckPayload( true );
    m_RadioModule.EnableDynamicPayload( true );
    m_RadioModule.EnableNoAckTxPayload( true );

    m_RadioModule.OpenPipe( RFM73_PIPE0, GetPipeAddrFromNodeAddr( RFM73_STACK_RX_ADDR_PIPE0 ), true, true );
    
    m_RadioModule.FlushTxFifo();
    m_RadioModule.SetPrimaryTxMode();

    m_RadioModule.EnterToStandByModeI();
    
    m_RadioModule.ChipEnableSet();
    
}


uint8_t* RFM73_SleepDevice_t::GetPipeAddrFromNodeAddr( uint8_t addr )
{
    m_NetworkAddr[ 0 ] = addr;
    m_NetworkAddr[ 1 ] = ( uint8_t )RFM73_STACK_NETWORK_ADDRESS;
    m_NetworkAddr[ 2 ] = RFM73_STACK_NETWORK_ADDRESS >> 8;
 
    return m_NetworkAddr;
}

void RFM73_SleepDevice_t::Run()
{
    RFM73_SleepDeviceNodeTxInfo_t txInfo;
     
    txInfo.AckDataLen = 0;
    txInfo.pAckData   = 0;
    
    if( true == m_RadioModule.GetRxDataReadyIrqFlag() )
    {
        m_RadioModule.ClearIrqFlag( RFM73_RX_DR );
        uint8_t ackDatalen = m_RadioModule.GetRxPayloadWidth();
        m_RadioModule.ReadRxPayload( m_RxData, ackDatalen );
        txInfo.pAckData   = m_RxData;
        txInfo.AckDataLen = ackDatalen;
    }
    
    if( true == m_RadioModule.GetTxDataSentIrqFlag() )
    {
       m_RadioModule.ClearIrqFlag( RFM73_TX_DS );
       txInfo.Status     = RFM73_STACK_TX_SUCCESS;
       
       if( 0 != fp_TxCallBack )
       {
           fp_TxCallBack( m_pTxContext, &txInfo );
       }    
    }
    
    if( true == m_RadioModule.GetMaxNumberRetransmitionIrqFlag() )
    {
       m_RadioModule.ClearIrqFlag( RFM73_MAX_RT );
       txInfo.Status = RFM73_STACK_TX_TIMEOUT;
       if( 0 != fp_TxCallBack )
       {
           fp_TxCallBack( m_pTxContext, &txInfo );
       }
    }
}

void RFM73_SleepDevice_t::SendData( uint8_t nodeAddr, uint8_t* pData, uint8_t len, bool ackEnable )
{
    if( true == m_IsPowerDownMode )
    {
        m_RadioModule.EnterToStandByModeI();
        m_RadioModule.ChipEnableSet();
        m_IsPowerDownMode = false;
    }
      
    m_RadioModule.FlushTxFifo();
    m_RadioModule.FlushRxFifo();
    
    m_RadioModule.SetTxAddress( GetPipeAddrFromNodeAddr( nodeAddr ) );
    m_RadioModule.SetRxPipeAddress( RFM73_PIPE0, GetPipeAddrFromNodeAddr( nodeAddr ) );
    
    if( false == m_RadioModule.IsTxFifoFull() )
    {
        m_RadioModule.ClearIrqFlag( RFM73_TX_DS );
        m_RadioModule.ClearIrqFlag( RFM73_MAX_RT );
        
        if( true == ackEnable )
        {
            m_RadioModule.WriteTxPayload( pData, len );
        }
        else
        {
            m_RadioModule.WriteTxPayloadNoAck( pData, len );
        }
    }
}

void RFM73_SleepDevice_t::PowerDown()
{
    m_RadioModule.EnterToPowerDownMode();
    m_IsPowerDownMode = true;

}

void RFM73_SleepDevice_t::PowerUp()
{
    
}
