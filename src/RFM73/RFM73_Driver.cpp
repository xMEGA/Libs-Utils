#include "RFM73_Driver.h"


// ------------------------ Connection MCU <-> RFM73 ------------------------ //
void RFM73_t::ConnectionInit()
{
    m_pControlInterface->ConnectionInit();
}
  
void RFM73_t::SpiChipSelectClr()
{
    m_pControlInterface->SpiChipSelectClr();

}

void RFM73_t::SpiChipSelectSet()
{
    m_pControlInterface->SpiChipSelectSet();
}

void RFM73_t::ChipEnableClr()
{
    m_pControlInterface->ChipEnableClr();
}

void RFM73_t::ChipEnableSet()
{
    m_pControlInterface->ChipEnableSet();
}

uint8_t RFM73_t::SpiWrite( uint8_t byte )
{
    return m_pControlInterface->SpiWrite( byte );
}

uint8_t RFM73_t::SpiRead( void )
{
    return m_pControlInterface->SpiWrite( 0xFF );
}

void RFM73_t::DelayUsec( uint32_t msec )
{
    m_pControlInterface->DelayMsec( msec );
}

void RFM73_t::WriteData( uint8_t* pData, uint8_t len )
{
    for( uint8_t idx = 0; idx != len ; idx++ )
    {
        SpiWrite( pData[idx] );
    }
}

void RFM73_t::ReadData( uint8_t* pData, uint8_t len )
{
    for( uint8_t idx = 0; idx != len ; idx++ )
    {
        pData[ idx ] = SpiRead();
    }
}
// -------------------------------------------------------------------------- //


// ----------------------------- RFM73 Commands------------------------------ //

uint8_t  RFM73_t::ReadReg( RFM73_RegisterBank0_t reg )
{
    uint8_t retValue = 0;
    
    uint8_t writeValue = RFM73_R_REGISTER_CMD;
    writeValue |= reg;
      
    SpiChipSelectClr();

    SpiWrite( writeValue ) ;
    retValue = SpiRead();

    SpiChipSelectSet();

    return retValue;

}

uint32_t RFM73_t::ReadReg( RFM73_RegisterBank1_t reg )
{
    uint32_t retValue = 0;
    uint8_t writeValue = RFM73_R_REGISTER_CMD;
    writeValue |= reg;
      
    SpiChipSelectClr();

    SpiWrite( writeValue ) ;
    ReadData( ( uint8_t * )&retValue, sizeof( uint32_t ) );

    SpiChipSelectSet();

    return retValue;
}

void RFM73_t::ReadReg( RFM73_RegisterBank0_t reg, uint8_t* pData, uint8_t len )
{
    uint8_t writeValue = RFM73_R_REGISTER_CMD;
    writeValue |= reg;
      
    SpiChipSelectClr();

    SpiWrite( writeValue ) ;

    ReadData( pData, len );
   
    SpiChipSelectSet();
}

void RFM73_t::WriteReg( RFM73_RegisterBank0_t reg, const uint8_t* pData, uint8_t len )
{
    uint8_t writeValue = RFM73_W_REGISTER_CMD;
    
    writeValue |= reg;
      
    SpiChipSelectClr();

    SpiWrite( writeValue ) ;
    
    WriteData( ( uint8_t* )pData, len );

    SpiChipSelectSet();
}
  
RFM73_StatusReg_t  RFM73_t::WriteReg( RFM73_RegisterBank0_t reg, uint8_t value )
{
    RFM73_StatusReg_t statusReg;
    
    uint8_t writeValue = RFM73_W_REGISTER_CMD;
    writeValue |= reg;
      
    SpiChipSelectClr();

    statusReg.Value = SpiWrite( writeValue ) ;
    SpiWrite( value );

    SpiChipSelectSet();

    return statusReg;
}

void RFM73_t::WriteReg( RFM73_RegisterBank1_t reg, uint32_t value )
{
    uint8_t writeValue = RFM73_W_REGISTER_CMD;
    writeValue |= reg;
    
    SpiChipSelectClr();
    
    SpiWrite( writeValue );
    WriteData( ( uint8_t* )&value, sizeof( uint32_t ) );
    
    SpiChipSelectSet();
}

uint8_t RFM73_t::ReadRxPayload( uint8_t* pPayload, uint8_t len )
{
    SpiChipSelectClr();
    
    SpiWrite( RFM73_R_RX_PAYLOAD_CMD );
    
    ReadData( pPayload, len );    
    
    SpiChipSelectSet();
    
    return len;
}

void RFM73_t::WriteTxPayload( const uint8_t* pPayload, uint8_t len )
{
    SpiChipSelectClr();
    
    SpiWrite( RFM73_W_TX_PAYLOAD_CMD );

    WriteData( ( uint8_t* )pPayload, len );
    
    SpiChipSelectSet();  
}

void RFM73_t::WriteTxPayloadNoAck( const uint8_t* pPayload, uint8_t len )
{  
    SpiChipSelectClr();
    
    SpiWrite( RFM73_W_TX_PAYLOAD_NOACK_CMD );

    WriteData( ( uint8_t* )pPayload, len );    
    
    SpiChipSelectSet();
}

void RFM73_t::WriteAckPayload( uint8_t pipe, const uint8_t* pPayload, uint8_t len )
{
    SpiChipSelectClr();
    
    uint8_t writeValue = RFM73_W_ACK_PAYLOAD_CMD | pipe;
    
    SpiWrite( writeValue );
     
    WriteData( ( uint8_t* )pPayload, len );  
    
    SpiChipSelectSet();
}

void RFM73_t::FlushRxFifo()
{
    SpiChipSelectClr();
    SpiWrite( RFM73_FLUSH_RX_CMD );
    SpiChipSelectSet();
}

void RFM73_t::FlushTxFifo()
{
    SpiChipSelectClr();
    SpiWrite( RFM73_FLUSH_TX_CMD );
    SpiChipSelectSet();
}

void RFM73_t::ReuseLastTxPayload()
{
    SpiChipSelectClr();
    SpiWrite( RFM73_REUSE_TX_PL_CMD );
    SpiChipSelectSet();
}

void RFM73_t::Activate( uint8_t value )
{
    SpiChipSelectClr();
    SpiWrite( RFM73_ACTIVATE_CMD );
    SpiWrite( value );
    SpiChipSelectSet();
}

uint8_t RFM73_t::GetRxPayloadWidth()
{
    SpiChipSelectClr();
    SpiWrite( RFM73_R_RX_PL_WID_CMD );
    uint8_t width = SpiRead();
    SpiChipSelectSet();
    
    return width;
}

RFM73_StatusReg_t RFM73_t::Nop()
{
    RFM73_StatusReg_t statusReg;
    
    SpiChipSelectClr();
    statusReg.Value = SpiWrite( RFM73_NOP_CMD );
    SpiChipSelectSet();    
    
    return statusReg;
}
// -----------------------------------------------------------------------------



// ------------------------------ RFM73 Utils ----------------------------------

void RFM73_t::SetAddressWidth( RFM73_AddressWidth_t addressWidth )
{
    RFM73_SetupAddressWithReg_t setupAddressWith;
    setupAddressWith.Value = 0;
    setupAddressWith.AddressWidth = addressWidth;
    WriteReg( RFM73_SETUP_ADDR_WIDTH_REG, setupAddressWith.Value );
}

void RFM73_t::EnableAckPayload( bool enable )
{
    RFM73_FeatureReg_t featureReg;
    featureReg.Value = ReadReg( RFM73_FEATURE_REG );

    featureReg.EnableAckPayload = ( enable ) ? 1U : 0U;
    
    WriteReg( RFM73_FEATURE_REG, featureReg.Value );
}

void RFM73_t::EnableNoAckTxPayload( bool enable )
{
    RFM73_FeatureReg_t featureReg;
    featureReg.Value = ReadReg( RFM73_FEATURE_REG );
    featureReg.EnableNoAckTxPayload = ( enable ) ? 1U : 0U;

    WriteReg( RFM73_FEATURE_REG, featureReg.Value );
}

void RFM73_t::EnableDynamicPayload( bool enable )
{
    RFM73_FeatureReg_t featureReg;
    featureReg.Value = ReadReg( RFM73_FEATURE_REG );
    featureReg.EnableDynPayloadLen = ( enable ) ? 1U : 0U;
    
    WriteReg( RFM73_FEATURE_REG, featureReg.Value );
}

void RFM73_t::SetDatarate( RFM73_Datarate_t datarate )
{
    RFM73_RfSetupReg_t rfSetup;
    rfSetup.Value = ReadReg ( RFM73_RF_SETUP_REG );

    switch( datarate )
    {
        case RFM73_DATARATE_250KBPS:
            rfSetup.AirDataRateLow   = 1U;
            rfSetup.AirDataRateHight = 0U;
        break;
       
        case RFM73_DATARATE_1MBPS:
            rfSetup.AirDataRateLow   = 0U;
            rfSetup.AirDataRateHight = 0U;
        break;
        
        case RFM73_DATARATE_2MBPS:
        default:
            rfSetup.AirDataRateLow   = 0U;
            rfSetup.AirDataRateHight = 1U;
        break;
    }

    WriteReg( RFM73_RF_SETUP_REG, rfSetup.Value );
}

void RFM73_t::SetRfChannel( uint8_t channel )
{
    RFM73_RfChannel_t rfChannel;
    rfChannel.Value = 0;
    rfChannel.Channel = channel;
    WriteReg ( RFM73_RF_CHANNEL_REG, rfChannel.Value );
}

void RFM73_t::EnableLnaGain( bool enable )
{
    //Todo
}

RFM73_AddressWidth_t RFM73_t::GetAddressWidth()
{
    RFM73_SetupAddressWithReg_t setupAddressWith;
    setupAddressWith.Value = ReadReg ( RFM73_SETUP_ADDR_WIDTH_REG );
    return setupAddressWith.AddressWidth;
}

void RFM73_t::SetCrcMode( RFM73_CrcMode_t crcMode )
{
    RFM73_ConfigReg_t configReg;

    configReg.Value = ReadReg( RFM73_CONFIG_REG );

    switch( crcMode )
    {
        case RFM73_CRC_OFF:
            configReg.EnableCrc = 0U;
        break;
       
        case RFM73_CRC_8BIT:
            configReg.EnableCrc = 1U;
            configReg.Crc0      = 0U;
        break;

        case RFM73_CRC_16BIT:
            configReg.EnableCrc = 1U;
            configReg.Crc0      = 1U;
        break;
        
        default:
        break;
    }

    WriteReg( RFM73_CONFIG_REG, configReg.Value );
}

bool RFM73_t::GetMaxNumberRetransmitionIrqFlag()
{
    RFM73_StatusReg_t statusReg = Nop();
    return statusReg.MaxNumberRetransmitionIrq;
}

bool RFM73_t::GetTxDataSentIrqFlag()
{
    RFM73_StatusReg_t statusReg = Nop();
    return statusReg.DataSentTxFifoIrq;
}

bool RFM73_t::GetRxDataReadyIrqFlag()
{
    RFM73_StatusReg_t statusReg = Nop();
    return statusReg.DataReadyRxFifoIrq;
}

void RFM73_t::SetPowerMode( RFM73_PowerMode_t pwrMode )
{
    RFM73_ConfigReg_t configReg;

    configReg.Value = ReadReg( RFM73_CONFIG_REG );

    if( RFM73_PWR_UP == pwrMode )
    {
        configReg.PowerUp = 1U;
    }
    else
    {
        configReg.PowerUp = 0U;
    }

    WriteReg( RFM73_CONFIG_REG, configReg.Value );
}

void RFM73_t::SelectRegisterBank( RFM73_RegBank_t regBank )
{
    RFM73_StatusReg_t statusReg;

    statusReg = Nop();
          
    if( regBank != statusReg.RegBankSelStates )  
    {
        Activate( 0x53 );
    }
}

void RFM73_t::SetOutputPower( RFM73_OutputPower_t power )
{
    RFM73_RfSetupReg_t rfSetupReg;

    rfSetupReg.Value = ReadReg( RFM73_RF_SETUP_REG );

    rfSetupReg.RfOutputPower = power;

    WriteReg( RFM73_RF_SETUP_REG, rfSetupReg.Value );
}
 
bool RFM73_t::IsCarrierDetect()
{
    RFM73_CarrierDetectReg_t carrierDetectReg;
    
    carrierDetectReg.Value = ReadReg( RFM73_CARRIER_DETECT_REG );

    bool isCarrierDetectSet = ( carrierDetectReg.CarrierDetect ) ? 1U : 0U;
    
    return isCarrierDetectSet;
}

bool RFM73_t::IsRxFifoEmpty()
{
    RFM73_FifoStatusReg_t fifoStatus;
    
    fifoStatus.Value = ReadReg( RFM73_FIFO_STATUS_REG );

    bool isRxFifoEmpty = ( fifoStatus.RxFifoEmpty ) ? 1U : 0U;
    
    return isRxFifoEmpty;
}

bool RFM73_t::IsRxFifoFull()
{
    RFM73_FifoStatusReg_t fifoStatus;
    
    fifoStatus.Value = ReadReg( RFM73_FIFO_STATUS_REG );

    bool isRxFifoFull = ( fifoStatus.RxFifoFull ) ? 1U : 0U;
    
    return isRxFifoFull;
}

bool RFM73_t::IsTxFifoEmpty()
{
    RFM73_FifoStatusReg_t fifoStatus;
    
    fifoStatus.Value = ReadReg( RFM73_FIFO_STATUS_REG );

    bool isRxFifoEmpty = ( fifoStatus.TxFifoEmpty ) ? 1U : 0U;
    
    return isRxFifoEmpty;
}

bool RFM73_t::IsTxFifoFull()
{
    RFM73_FifoStatusReg_t fifoStatus;
    
    fifoStatus.Value = ReadReg( RFM73_FIFO_STATUS_REG );

    bool isRxFifoFull = ( fifoStatus.TxFifoFull ) ? 1U : 0U;
    
    return isRxFifoFull;
}

void RFM73_t::SetRxPayloadWidth( RFM73_Pipe_t pipe, uint8_t ploadWidth )
{
    RFM73_RegisterBank0_t reg[] = { 
                              RFM73_RX_DATA_WIDTH_P0_REG,
                              RFM73_RX_DATA_WIDTH_P1_REG,
                              RFM73_RX_DATA_WIDTH_P2_REG,
                              RFM73_RX_DATA_WIDTH_P3_REG,
                              RFM73_RX_DATA_WIDTH_P4_REG,
                              RFM73_RX_DATA_WIDTH_P5_REG
                            };  
    
    WriteReg( reg[ pipe ], ploadWidth );
}

RFM73_Pipe_t RFM73_t::GetRxDataSource()
{
    RFM73_StatusReg_t status = Nop();
    
    RFM73_Pipe_t pipe = ( RFM73_Pipe_t )status.RxPayloadDataPipeNumber;
    
    return pipe;
}


void RFM73_t::SeIrqMode( RFM73_IrqSource_t intSource, bool irqState )  
{
    RFM73_ConfigReg_t configReg;
    configReg.Value = ReadReg( RFM73_CONFIG_REG );

    switch ( intSource )
    {
        case RFM73_MAX_RT:
            configReg.MaskMaxRetrans = irqState ? 0U : 1U;
        break;
          
        case RFM73_TX_DS:
            configReg.MaskTxDataSent  = irqState ? 0U : 1U;
        break;
          
        case RFM73_RX_DR:
            configReg.MaskRxDataReady = irqState ? 0U : 1U;
        break;
    }

    WriteReg( RFM73_CONFIG_REG, configReg.Value );
}

void RFM73_t::SetPllMode( bool isPllLock )
{
    RFM73_RfSetupReg_t rfSetupReg;

    rfSetupReg.Value = ReadReg( RFM73_RF_SETUP_REG );
    rfSetupReg.PllLock = ( isPllLock ) ? 1U : 0U;

    WriteReg( RFM73_RF_SETUP_REG, rfSetupReg.Value );
}

RFM73_RxFifoStatus_t RFM73_t::GetRxFifoStatus()
{
    RFM73_RxFifoStatus_t rxFifoStatus = RFM73_RX_FIFO_NOT_EMPTY_NOT_FULL;
    RFM73_FifoStatusReg_t fifoStatus;
    
    fifoStatus.Value = ReadReg( RFM73_FIFO_STATUS_REG );

    if( fifoStatus.RxFifoEmpty )
    {
        rxFifoStatus = RFM73_RX_FIFO_EMPTY;
    }
    
    if( fifoStatus.RxFifoFull )  
    {
        rxFifoStatus = RFM73_RX_FIFO_FULL;
    }
    
    return rxFifoStatus;
}

RFM73_TxFifoStatus_t RFM73_t::GetTxFifoStatus()
{
    RFM73_TxFifoStatus_t txFifoStatus = RFM73_TX_FIFO_NOT_EMPTY_NOT_FULL;
    RFM73_FifoStatusReg_t fifoStatus;
    
    fifoStatus.Value = ReadReg( RFM73_FIFO_STATUS_REG );

    if( fifoStatus.TxFifoEmpty )
    {
        txFifoStatus = RFM73_TX_FIFO_EMPTY;
    }
    
    if( fifoStatus.TxFifoFull )  
    {
        txFifoStatus = RFM73_TX_FIFO_FULL;
    }
    
    return txFifoStatus;
}

void RFM73_t::EnableContiniousWave( bool enable )
{
    RFM73_RfSetupReg_t rfSetupReg;
    
    rfSetupReg.Value = ReadReg ( RFM73_RF_SETUP_REG );
    rfSetupReg.ContiniousWave = enable ? 1U : 0U;

    WriteReg( RFM73_RF_SETUP_REG, rfSetupReg.Value );
}
    
uint8_t RFM73_t::GetRxPayloadWidth( RFM73_Pipe_t pipe )
{
    RFM73_RegisterBank0_t reg[] = { 
                              RFM73_RX_DATA_WIDTH_P0_REG,
                              RFM73_RX_DATA_WIDTH_P1_REG,
                              RFM73_RX_DATA_WIDTH_P2_REG,
                              RFM73_RX_DATA_WIDTH_P3_REG,
                              RFM73_RX_DATA_WIDTH_P4_REG,
                              RFM73_RX_DATA_WIDTH_P5_REG
                            };  
    
    uint8_t payloadWidth = ReadReg( reg[ pipe ] );
  
    return payloadWidth;  
}

void RFM73_t::SetAutoRetransmission( uint8_t retrCnt, uint16_t delay )
{
    RFM73_SetupAutoRetransmissionReg_t setupRetrReg;
    setupRetrReg.AutoRetransmissionDelay = (uint8_t)( delay >> 8 );
    setupRetrReg.AutoRetransmissionCount = retrCnt;
    WriteReg( RFM73_SETUP_RETRY_REG, setupRetrReg.Value );
}

uint8_t RFM73_t::GetAutoRetransmittedCounter()
{
    RFM73_ObserveTxReg_t observeTxReg;
    
    observeTxReg.Value = ReadReg( RFM73_OBSERVE_TX_REG );
    
    return observeTxReg.RetransmittedPacketCount;
}

uint8_t RFM73_t::GetLostPacketCounter()
{
    RFM73_ObserveTxReg_t observeTxReg;
    
    observeTxReg.Value = ReadReg( RFM73_OBSERVE_TX_REG );
    
    return observeTxReg.LostPacketsCount;
}
    
void RFM73_t::ClearIrqFlag( RFM73_IrqSource_t intSource )
{
    RFM73_StatusReg_t statusReg = Nop();
  
    statusReg.Value = 0;
    
    switch ( intSource )
    {
        case RFM73_MAX_RT:
            statusReg.MaxNumberRetransmitionIrq = 1U;
        break;
          
        case RFM73_TX_DS:
            statusReg.DataSentTxFifoIrq = 1U;
        break;
          
        case RFM73_RX_DR:
            statusReg.DataReadyRxFifoIrq = 1U;
        break;
    }  
      
    WriteReg( RFM73_STATUS_REG, statusReg.Value );
    statusReg = Nop();
}

bool RFM73_t::GetReuseTxStatus()
{
    RFM73_FifoStatusReg_t fifoStatusReg;
    
    fifoStatusReg.Value = ReadReg( RFM73_FIFO_STATUS_REG );
    
    return fifoStatusReg.TxReuse ? true : false;
}
    
void RFM73_t::SetPrimaryTxMode()
{ 
    RFM73_ConfigReg_t configReg;
    configReg.Value = ReadReg( RFM73_CONFIG_REG );

    configReg.PrimRxMode = 0U;

    WriteReg( RFM73_CONFIG_REG, configReg.Value );
}

void RFM73_t::SetPrimaryRxMode()
{
    RFM73_ConfigReg_t configReg;
    configReg.Value = ReadReg( RFM73_CONFIG_REG );

    configReg.PrimRxMode = 1U;

    WriteReg( RFM73_CONFIG_REG, configReg.Value );
}

void RFM73_t::OpenPipe( RFM73_Pipe_t pipe, const uint8_t *addr, bool autoAckEnable, bool dynPayloadEnable )
{
    RFM73_EnableRxAddrReg_t  enableRxAddr;
    RFM73_EnableAutoAckReg_t enableAutoAckReg;
    
    enableRxAddr.Value     = ReadReg( RFM73_ENABLE_RX_ADDR_REG );
    enableAutoAckReg.Value = ReadReg( RFM73_ENABLE_AUTO_ACK_REG );

    uint8_t pipeNum = 1U << pipe;
    
    enableRxAddr.Value |= pipeNum;
    
    if( true == autoAckEnable )
    {
        enableAutoAckReg.Value |= pipeNum;
    }
    else
    {
        enableAutoAckReg.Value &= ~pipeNum;
    }

    enableRxAddr.Reserved     = 0;
    enableAutoAckReg.Reserved = 0;
    
    WriteReg( RFM73_ENABLE_RX_ADDR_REG, enableRxAddr.Value );
    WriteReg( RFM73_ENABLE_AUTO_ACK_REG, enableAutoAckReg.Value );
    
    if( true == dynPayloadEnable )
    {
        EnabplePipeDynamicPayload( pipe );  
    }
    else
    {
        DisablePipeDynamicPayload( pipe );
    }
    
    SetRxPipeAddress( pipe, addr );
}

void RFM73_t::ClosePipe( RFM73_Pipe_t pipe )
{
    RFM73_EnableRxAddrReg_t  enableRxAddr;
    RFM73_EnableAutoAckReg_t enableAutoAckReg;
    
    enableRxAddr.Value     = ReadReg( RFM73_ENABLE_RX_ADDR_REG );
    enableAutoAckReg.Value = ReadReg( RFM73_ENABLE_AUTO_ACK_REG );

    uint8_t pipeNum = 1U << pipe;
    
    enableRxAddr.Value     &= ~pipeNum;
    enableAutoAckReg.Value &= ~pipeNum;
   
    enableRxAddr.Reserved     = 0;
    enableAutoAckReg.Reserved = 0;
    
    WriteReg( RFM73_ENABLE_RX_ADDR_REG, enableRxAddr.Value );
    WriteReg( RFM73_ENABLE_AUTO_ACK_REG, enableAutoAckReg.Value );
}

void RFM73_t::SetRxPipeAddress( RFM73_Pipe_t pipe, const uint8_t *addr )
{ 
    RFM73_RegisterBank0_t regCollection[] = { 
                              RFM73_RX_ADDR_P0_REG,
                              RFM73_RX_ADDR_P1_REG,
                              RFM73_RX_ADDR_P2_REG,
                              RFM73_RX_ADDR_P3_REG,
                              RFM73_RX_ADDR_P4_REG,
                              RFM73_RX_ADDR_P5_REG
                            };  

    RFM73_RegisterBank0_t reg = regCollection[ pipe ];

    if( ( RFM73_RX_ADDR_P0_REG == reg ) || ( RFM73_RX_ADDR_P1_REG == reg ) )
    {
        uint8_t addrDataWithTable[] = { 0, 3, 4, 5 };
        uint8_t addrDataWith = addrDataWithTable[ GetAddressWidth() ];
        WriteReg( reg, addr, addrDataWith );
    }
    else
    {
        WriteReg( reg, *addr );
    }
}  

void RFM73_t::GetRxPipeAddress( RFM73_Pipe_t pipe, uint8_t *addr )
{
    RFM73_RegisterBank0_t regCollection[] = { 
                              RFM73_RX_ADDR_P0_REG,
                              RFM73_RX_ADDR_P1_REG,
                              RFM73_RX_ADDR_P2_REG,
                              RFM73_RX_ADDR_P3_REG,
                              RFM73_RX_ADDR_P4_REG,
                              RFM73_RX_ADDR_P5_REG
                            };  

    RFM73_RegisterBank0_t reg = regCollection[ pipe ];

    if( ( RFM73_RX_ADDR_P0_REG == reg ) || ( RFM73_RX_ADDR_P1_REG == reg ) )
    {
        uint8_t addrDataWithTable[] = { 0, 3, 4, 5 };
        uint8_t addrDataWith = addrDataWithTable[ GetAddressWidth() ];
        ReadReg( reg, addr, addrDataWith );
    }
    else
    {
        *addr = ReadReg( reg );
    }
}


void RFM73_t::SetTxAddress( const uint8_t *addr )
{
    uint8_t addrDataWithTable[] = { 0, 3, 4, 5 };
    uint8_t addrDataWith = addrDataWithTable[ GetAddressWidth() ];
    WriteReg( RFM73_TX_ADDR_REG, addr, addrDataWith );
}
   
void RFM73_t::EnabplePipeDynamicPayload( RFM73_Pipe_t pipe )
{
    RFM73_EnableDynamicPayloadReg_t enaDynPayload;
    enaDynPayload.Value = ReadReg( RFM73_EN_DYNAMIC_PAYLOAD_REG ); 

    enaDynPayload.Value |= 1U << pipe;
  
    WriteReg ( RFM73_EN_DYNAMIC_PAYLOAD_REG, enaDynPayload.Value);
}

void RFM73_t::DisablePipeDynamicPayload( RFM73_Pipe_t pipe )
{
    RFM73_EnableDynamicPayloadReg_t enaDynPayload;
    enaDynPayload.Value = ReadReg( RFM73_EN_DYNAMIC_PAYLOAD_REG ); 

    enaDynPayload.Value &= ~( 1U << pipe );
  
    WriteReg ( RFM73_EN_DYNAMIC_PAYLOAD_REG, enaDynPayload.Value);
}

void RFM73_t::Init( RFM73_ControlInterface_t* pControlInterface )
{
    m_pControlInterface = pControlInterface;
    DelayUsec( RFM73_POWER_ON_RESET_TIME_US );
  
    ConnectionInit();
    ClearIrqFlag( RFM73_RX_DR );
    ClearIrqFlag( RFM73_TX_DS );
    ClearIrqFlag( RFM73_MAX_RT );
    //DelayUsec( RFM73_POWER_ON_RESET_TIME_US );
  
    InitBank1();   
}

uint32_t RFM73_t::GetChipId()
{
    uint32_t chipId = 0;
    if( RFM73_REG_BANK_0 == GetActiveRegBank() )
    {
        SelectRegisterBank( RFM73_REG_BANK_1 );
        chipId = ReadReg( RFM73_CHIP_ID_REG );
        SelectRegisterBank( RFM73_REG_BANK_0 );
    }
    else
    {
        chipId = ReadReg( RFM73_CHIP_ID_REG );
    }
  
    return chipId;
}

RFM73_RegBank_t RFM73_t::GetActiveRegBank()
{
    RFM73_StatusReg_t statusReg = Nop();        

    return statusReg.RegBankSelStates;    
}
 
bool RFM73_t::IsFeaturesActivate()
{
    bool isActivate = false;
    
    RFM73_FeatureReg_t featureReg;

    featureReg.Value = ReadReg( RFM73_FEATURE_REG );
  
    RFM73_FeatureReg_t restoreFeatureReg = featureReg;
    
    featureReg.EnableAckPayload     ^= 1U;
    featureReg.EnableNoAckTxPayload ^= 1U;
    featureReg.EnableDynPayloadLen  ^= 1U;
        
    WriteReg( RFM73_FEATURE_REG, featureReg.Value );
    
    featureReg.Value = ReadReg( RFM73_FEATURE_REG );
    
    if( featureReg.Value != restoreFeatureReg.Value )
    {
        isActivate = true;
        WriteReg( RFM73_FEATURE_REG, restoreFeatureReg.Value );
    }
    
    return isActivate;
}

void RFM73_t::FeaturesActivate()
{
    if( false == IsFeaturesActivate() )
    {
        Activate( 0x73 );
    }
}

void RFM73_t::FeaturesDeActivate()
{
    if( true == IsFeaturesActivate() )
    {
        Activate( 0x73 );
    }
}
    
void RFM73_t::EnterToPowerDownMode()
{
    SetPowerMode( RFM73_PWR_DOWN );
    ChipEnableClr();
}

void RFM73_t::EnterToStandByModeI()
{
    SetPowerMode( RFM73_PWR_UP ); 
    DelayUsec( RFM73_START_UP_FROM_POWER_DOWN_MODE_TIME_US ); 
}

void RFM73_t::InitBank1()
{
    //Todo
}
