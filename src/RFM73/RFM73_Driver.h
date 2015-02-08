#ifndef RFM73_DRIVER_H
#define RFM73_DRIVER_H

#include <stdint.h>
#include "RFM73_Control.h"

// ---------------------------- RFM73 COMMANDS ---------------------------------

enum RFM73_Cmd_t
{
    RFM73_R_REGISTER_CMD         = 0x00,
    RFM73_W_REGISTER_CMD         = 0x20,
    RFM73_R_RX_PAYLOAD_CMD       = 0x61,
    RFM73_W_TX_PAYLOAD_CMD       = 0xA0,
    RFM73_FLUSH_TX_CMD           = 0xE1,
    RFM73_FLUSH_RX_CMD           = 0xE2,
    RFM73_REUSE_TX_PL_CMD        = 0xE3,
    RFM73_ACTIVATE_CMD           = 0x50,
    RFM73_R_RX_PL_WID_CMD        = 0x60,
    RFM73_W_ACK_PAYLOAD_CMD      = 0xA8,
    RFM73_W_TX_PAYLOAD_NOACK_CMD = 0xB0,
    RFM73_NOP_CMD                = 0xFF  
};   

// -------------------------RFM73 Regiters (Bank 0)-----------------------------
enum RFM73_RegisterBank0_t
{
    RFM73_CONFIG_REG  	         = 0x00, //Size: 1 Bytes, default value 0x08
    RFM73_ENABLE_AUTO_ACK_REG	 = 0x01, //Size: 1 Bytes, default value 0x3F
    RFM73_ENABLE_RX_ADDR_REG	 = 0x02, //Size: 1 Bytes, default value 0x03
    RFM73_SETUP_ADDR_WIDTH_REG   = 0x03, //Size: 1 Bytes, default value 0x03
    RFM73_SETUP_RETRY_REG	 = 0x04, //Size: 1 Bytes, default value 0x03
    RFM73_RF_CHANNEL_REG	 = 0x05, //Size: 1 Bytes, default value 0x02
    RFM73_RF_SETUP_REG    	 = 0x06, //Size: 1 Bytes, default value 0x3F
    RFM73_STATUS_REG		 = 0x07, //Size: 1 Bytes, default value 0x0E
    RFM73_OBSERVE_TX_REG	 = 0x08, //Size: 1 Bytes, default value 0x00
    RFM73_CARRIER_DETECT_REG	 = 0x09, //Size: 1 Bytes, default value 0x00
    RFM73_RX_ADDR_P0_REG	 = 0x0A, //Size: 5 Bytes, default value 0xE7E7E7E7E7
    RFM73_RX_ADDR_P1_REG	 = 0x0B, //Size: 5 Bytes, default value 0xC2C2C2C2C2
    RFM73_RX_ADDR_P2_REG	 = 0x0C, //Size: 1 Bytes, default value 0xC3
    RFM73_RX_ADDR_P3_REG	 = 0x0D, //Size: 1 Bytes, default value 0xC4
    RFM73_RX_ADDR_P4_REG	 = 0x0E, //Size: 1 Bytes, default value 0xC5
    RFM73_RX_ADDR_P5_REG	 = 0x0F, //Size: 1 Bytes, default value 0xC6
    RFM73_TX_ADDR_REG    	 = 0x10, //Size: 5 Bytes, default value 0xE7E7E7E7E7
    RFM73_RX_DATA_WIDTH_P0_REG	 = 0x11, //Size: 1 Bytes, default value 0x00
    RFM73_RX_DATA_WIDTH_P1_REG	 = 0x12, //Size: 1 Bytes, default value 0x00
    RFM73_RX_DATA_WIDTH_P2_REG	 = 0x13, //Size: 1 Bytes, default value 0x00
    RFM73_RX_DATA_WIDTH_P3_REG	 = 0x14, //Size: 1 Bytes, default value 0x00
    RFM73_RX_DATA_WIDTH_P4_REG	 = 0x15, //Size: 1 Bytes, default value 0x00
    RFM73_RX_DATA_WIDTH_P5_REG	 = 0x16, //Size: 1 Bytes, default value 0x00
    RFM73_FIFO_STATUS_REG	 = 0x17, //Size: 1 Bytes, default value 0x11
    RFM73_EN_DYNAMIC_PAYLOAD_REG = 0x1C, //Size: 1 Bytes, default value 0x00
    RFM73_FEATURE_REG		 = 0x1D  //Size: 1 Bytes, default value 0x00
};


// -------------------------RFM73 Registers (Bank 1)-----------------------------
enum RFM73_RegisterBank1_t
{
    RFM73_REG0  	         = 0x00, //Size: 4 bytes, reset value: 0x00000000, must write with 0x404B01E2
    RFM73_REG1  	         = 0x01, //Size: 4 bytes, reset value: 0x00000000, must write with 0xC04B0000
    RFM73_REG2  	         = 0x02, //Size: 4 bytes, reset value: 0x00000000, must write with 0xD0FC8C02
    RFM73_REG3  	         = 0x03, //Size: 4 bytes, reset value: 0x03001200, must write with 0x99003941
    RFM73_RX_SEN_TX_PWR_REG      = 0x04, //Size: 4 bytes, reset value: 0x00100000, must write with 0xD99E860B, for single carrier mode:0xD99E8621
    RFM73_RSSI_TH_ENABLE_REG     = 0x05, //Size: 4 bytes, reset value: 0x24000000, must write with 0x24067FA6 (Disable RSSI)
    RFM73_REG6  	         = 0x06, //Size: 4 bytes, reset value: 0x00000000, reserved register
    RFM73_RBANK_REG  	         = 0x07, //Size: 4 bytes, reset value: 0x00000000
    RFM73_CHIP_ID_REG  	         = 0x08, //Size: 4 bytes, reset value: 0x00000063
    RFM73_REG9  	         = 0x09, //Size: 4 bytes, reset value: 0x00000000, reserved register
    RFM73_REGA  	         = 0x0A, //Size: 4 bytes, reset value: 0x00000000, reserved register
    RFM73_REGB  	         = 0x0B, //Size: 4 bytes, reset value: 0x00000000, reserved register
    RFM73_PLL_SET_COMP_MODE_REG  = 0x0C, //Size: 4 bytes, reset value: 0x05000200, must write with 0x05731200, for 120us mode:0x00731200
    RFM73_NEW_FEATURE_REG        = 0x0D, //Size: 4 bytes, reset value: 0x00000000, must write with 0x0080B436
    RFM73_RAMP_REG               = 0x0E, //Size: 4 bytes, reset value: ?         , must write with 0xFFEF7DF208082082041041
};

// -----------------------------------------------------------------------------

enum RFM73_RxFifoStatus_t
{
    RFM73_RX_FIFO_NOT_EMPTY_NOT_FULL = 0x00,
    RFM73_RX_FIFO_EMPTY              = 0x01,
    RFM73_RX_FIFO_FULL               = 0x02
};

enum RFM73_TxFifoStatus_t
{
    RFM73_TX_FIFO_NOT_EMPTY_NOT_FULL = 0x00,
    RFM73_TX_FIFO_EMPTY              = 0x01,
    RFM73_TX_FIFO_FULL               = 0x02
};

enum RFM73_IrqSource_t
{
    RFM73_MAX_RT = 4,     /**< Max retries interrupt */
    RFM73_TX_DS,          /**< TX data sent interrupt */
    RFM73_RX_DR           /**< RX data received interrupt */
};

enum RFM73_PowerMode_t
{
    RFM73_PWR_DOWN,
    RFM73_PWR_UP
};

enum RFM73_OutputPower_t
{
    RFM73_MINUS_10DBM = 0x00,
    RFM73_MINUS_5DBM  = 0x01,
    RFM73_0DBM        = 0x02,
    RFM73_PLUS_5DBM   = 0x03
};

enum RFM73_Datarate_t
{
    RFM73_DATARATE_1MBPS,
    RFM73_DATARATE_2MBPS,
    RFM73_DATARATE_250KBPS
};

enum RFM73_RegBank_t
{
    RFM73_REG_BANK_0 = ( uint8_t )0,
    RFM73_REG_BANK_1 = ( uint8_t )1
};

enum RFM73_CrcMode_t
{
    RFM73_CRC_OFF,
    RFM73_CRC_8BIT,
    RFM73_CRC_16BIT
};

enum RFM73_Pipe_t
{
    RFM73_PIPE0 = 0,
    RFM73_PIPE1 = 1,
    RFM73_PIPE2 = 2,
    RFM73_PIPE3 = 3,
    RFM73_PIPE4 = 4,
    RFM73_PIPE5 = 5,
    RFM73_PIPE_UNKNOWN = 0xFF   
};

enum RFM73_AddressWidth_t
{
    RFM73_ADDRESS_WIDTH_3BYTES = 0x01,   /**< Set address width to 3 bytes */
    RFM73_ADDRESS_WIDTH_4BYTES = 0x02,   /**< Set address width to 4 bytes */
    RFM73_ADDRESS_WIDTH_5BYTES = 0x03    /**< Set address width to 5 bytes */
} ;

//---------------------------- BANK0 REGISTERS ---------------------------------

union RFM73_ConfigReg_t
{
    uint8_t Value;
    
    struct 
    {
        uint8_t PrimRxMode      : 1;
                                 //'1' - PRX
                                 //'0' - PTX
        uint8_t PowerUp         : 1;
                                 //'1' - POWER UP
                                 //'0' - POWER DOWN
        uint8_t Crc0            : 1;
                                 //'0' - 1 byte 
                                 //'1' - 2 bytes
        uint8_t EnableCrc       : 1;
                                 //Enable CRC. Forced high if one of the bits in the EN_AA is high 
        uint8_t MaskMaxRetrans  : 1;
                                 //1: Interrupt not reflected on the IRQ pin 
                                 //0: Reflect MAX_RT as active low interrupt on the IRQ pin
        uint8_t MaskTxDataSent  : 1;
                                 //1: Interrupt not reflected on the IRQ pin 
                                 //0: Reflect TX_DS as active low interrupt on the IRQ pin 
        uint8_t MaskRxDataReady : 1;
                                 //1: Interrupt not reflected on the IRQ pin 
                                 //0: Reflect RX_DR as active low interrupt on the IRQ pin
        uint8_t                 : 1; 
                                 // Only '0' allowed 
    };
};

union RFM73_EnableAutoAckReg_t
{
    uint8_t Value;
    struct 
    {
        uint8_t EnableAutoAckDataPipe0 : 1;
        uint8_t EnableAutoAckDataPipe1 : 1;
        uint8_t EnableAutoAckDataPipe2 : 1;
        uint8_t EnableAutoAckDataPipe3 : 1;
        uint8_t EnableAutoAckDataPipe4 : 1;
        uint8_t EnableAutoAckDataPipe5 : 1;
        uint8_t Reserved               : 2; //Only '00' allowed
    };
};

union RFM73_EnableRxAddrReg_t
{
    uint8_t Value;
    struct 
    {
        uint8_t EnableDataPipe0 : 1;
        uint8_t EnableDataPipe1 : 1;
        uint8_t EnableDataPipe2 : 1;
        uint8_t EnableDataPipe3 : 1;
        uint8_t EnableDataPipe4 : 1;
        uint8_t EnableDataPipe5 : 1;
        uint8_t Reserved        : 2; //Only '00' allowed
    };
};

union RFM73_SetupAddressWithReg_t
{
    uint8_t Value;

    struct 
    {
        RFM73_AddressWidth_t AddressWidth : 2;
                                         //'00' - Illegal 
                                         //'01' - 3 bytes 
                                         //'10' - 4 bytes 
                                         //'11' - 5 bytes
        uint8_t               : 6; // Only '000000' allowed 
    };
};

union RFM73_SetupAutoRetransmissionReg_t
{
    uint8_t Value;
    struct 
    {
        uint8_t AutoRetransmissionCount : 4;      
                                            //"0000" –Re-Transmit disabled 
                                            //"0001" – Up to 1 Re-Transmission on fail of AA 
                                            //         ....
                                            //"1111" – Up to 15 Re-Transmission on fail of AA 

        uint8_t AutoRetransmissionDelay : 4;
                                            //"0000" – Wait 250 us 
                                            //"0001" – Wait 500 us 
                                            //"0010" – Wait 750 us 
                                            //       .... 
                                            //"1111" – Wait 4000 us
    };
};

union RFM73_RfChannel_t
{
    uint8_t Value;
    struct 
    {
        uint8_t Channel : 7; // Sets the frequency channel 
        uint8_t         : 1; // Only '000000' allowed 
    };
};

union RFM73_RfSetupReg_t
{
    uint8_t Value;
    struct 
    {
        uint8_t SetupLnaGain     : 1;
                                     //'0' - Low gain(20dB down) 
                                     //'1' - High gain
        uint8_t RfOutputPower    : 2;
                                     //'00' – -10 dBm 
                                     //'01' – -5 dBm 
                                     //'10' – 0 dBm 
                                     //'11' – 5 dBm 

        uint8_t AirDataRateHight : 1;  
                                     //Encoding: air_data_rate_low, air_data_rate_hight: 
                                     //'00' – 1Mbps 
                                     //'01' – 2Mbps (default) 
                                     //'10' – 250Kbps 
                                     //'11' – 2Mbps  
        
        uint8_t PllLock          : 1; //Force PLL lock signal. Only used in test
        uint8_t AirDataRateLow   : 1;
        uint8_t                  : 1; 
        uint8_t ContiniousWave   : 1; 
    };
};




union RFM73_StatusReg_t
{
    uint8_t Value;
    
    struct 
    {
        uint8_t TxFifiFull               : 1;
                                          //'1' - TX FIFO full 
                                          //'0' - Available locations in TX FIFO
        
        
        uint8_t RxPayloadDataPipeNumber  : 3;
                                          //'000-101': Data Pipe Number 
                                          //'110' Not used 
                                          //'111' RX FIFO Empty
        
        uint8_t MaxNumberRetransmitionIrq : 1;
                                         //Write 1 to clear bit. If MAX_RT is asserted 
                                         //it must be cleared to enable further communication. 

        
        
        uint8_t DataSentTxFifoIrq        : 1;
                                         //Asserted when packet transmitted on TX. If AUTO_ACK is activated, 
                                         //this bit is set high only when ACK is received. Write 1 to clear bit.
        
        uint8_t DataReadyRxFifoIrq      : 1;
                                         //Asserted when new data arrives RX FIFO. Write 1 to clear bit. 

        
        RFM73_RegBank_t RegBankSelStates   : 1;
        
                                        //register bank is done by SPI command “ACTIVATE” followed by 0x53 
                                        //'0' - Register bank 0 
                                        //'1' - Register bank 1 
    };
};

union RFM73_ObserveTxReg_t
{
    uint8_t Value;
    struct 
    {
        uint8_t RetransmittedPacketCount : 4;
                                          //The counter is reset when transmission of a new packet starts 
 
        
        uint8_t LostPacketsCount         : 4;
                                         //The counter is overflow protected to 15, and discontinues
                                         //at max until reset. The counter is reset by writing to RF_CH. 
    };
};

union RFM73_CarrierDetectReg_t
{
    uint8_t Value;
    struct 
    {
        uint8_t CarrierDetect : 1;
        uint8_t               : 7;                                 
    };
};
  
union RFM73_FifoStatusReg_t
{
    uint8_t Value;
    struct 
    {
        uint8_t RxFifoEmpty : 1;
                                //'1' - RX FIFO empty 
                                //'0' - Data in RX FIFO

        uint8_t RxFifoFull  : 1;
                                //'1' - RX FIFO full 
                                //'0' - Available locations in RX FIFO 
    
        uint8_t             : 2; // Only '00' allowed
        
        
        uint8_t TxFifoEmpty : 1; 
                                //'1': TX FIFO empty 
                                //'0': Data in TX FIFO
        
        uint8_t TxFifoFull  : 1; 
                                //'1' - TX FIFO full; 
                                //'0' - Available locations in TX FIFO

        uint8_t TxReuse     : 1;
                                //The packet is repeatedly retransmitted as long as CE is high. 
                                //TX_REUSE is set by the SPI command REUSE_TX_PL, and is reset
                                //by the SPI command W_TX_PAYLOAD or FLUSH TX 

        uint8_t             : 1; // Only '0' allowed
        
    };
};


union RFM73_EnableDynamicPayloadReg_t
{
    uint8_t Value;
    struct 
    {
        uint8_t EnableDynamicPayloadPipe0 : 1;
        uint8_t EnableDynamicPayloadPipe1 : 1;
        uint8_t EnableDynamicPayloadPipe2 : 1;
        uint8_t EnableDynamicPayloadPipe3 : 1;
        uint8_t EnableDynamicPayloadPipe4 : 1;
        uint8_t EnableDynamicPayloadPipe5 : 1;
        uint8_t                           : 2;
    };
};

union RFM73_FeatureReg_t
{
    uint8_t Value;

    struct 
    {
        uint8_t EnableNoAckTxPayload : 1; //Enables the W_TX_PAYLOAD_NOACK command
        uint8_t EnableAckPayload     : 1; //Enables Payload with ACK
        uint8_t EnableDynPayloadLen  : 1; //Enables Dynamic Payload Length
        uint8_t                      : 5; //Only "00000" allowed
    };
};
// -----------------------------------------------------------------------------



//---------------------------- BANK1 REGISTERS ---------------------------------

union RFM73_RxSensitivityTxPowerReg_t
{
    uint8_t Value;

    struct 
    {
        uint32_t               : 19;
        uint32_t TxPower       : 1; //RF output power in TX mode
                                   //'0' - Low power(-30dB down)
                                   //'1' - Normal power
 
        uint32_t RxSensitivity : 1; //Sensitivity in RX mode 
                                   //'0' - Normal mode
                                   //'1' - High sensitivity mode(different CD detection values)
        uint32_t               : 11;
    };
};

union RFM73_RssiThRssiEnableReg_t
{
    uint32_t Value;

    struct 
    {
        uint32_t                       : 17;
        uint32_t RssiMeasurementEnable : 1; //RSSI measurement
                                           //'0' - Enable
                                           //'1' - Disable                                   
        uint32_t                       : 7;
        uint32_t RssiThreshold         : 4; //RSSI Threshold for CD detect
                                           //1Mbps/250Kbps:-91dBm
                                           //2Mbps:-84dBm
        uint32_t                       : 3;
    };
};


union RFM73_RegisterBankReg_t
{
    uint8_t Value;

    struct 
    {
        uint32_t                      : 6;
        uint8_t RssiMeasurementEnable : 1; //RSSI measurement
                                           //'0' - Register bank 0
                                           //'1' - Register bank 1                               
        uint32_t                      : 25;
    };
};
// -----------------------------------------------------------------------------

#ifndef RFM73_POWER_ON_RESET_TIME_US
  #define RFM73_POWER_ON_RESET_TIME_US                10300
#endif

#ifndef RFM73_START_UP_FROM_POWER_DOWN_MODE_TIME_US
  #define RFM73_START_UP_FROM_POWER_DOWN_MODE_TIME_US 1500
#endif

class RFM73_t
{
public:

    void Init( RFM73_ControlInterface_t* pControlInterface );  
  
    // ------------------ Utils ------------------------//
    void SelectRegisterBank( RFM73_RegBank_t regBank );
    void SetAddressWidth( RFM73_AddressWidth_t addressWidth );
    void EnableAckPayload( bool enable );
    void EnableNoAckTxPayload( bool enable );
    void EnableDynamicPayload( bool enable );
    void SetDatarate( RFM73_Datarate_t datarate );
    void SetRfChannel( uint8_t channel );
    void EnableLnaGain( bool enable );
    RFM73_AddressWidth_t GetAddressWidth();
    void SetCrcMode( RFM73_CrcMode_t crcMode );
    bool GetMaxNumberRetransmitionIrqFlag();
    bool GetTxDataSentIrqFlag();
    bool GetRxDataReadyIrqFlag();
    void SetPowerMode( RFM73_PowerMode_t pwrMode );
    void SetOutputPower( RFM73_OutputPower_t power );
    bool IsCarrierDetect();
    bool IsRxFifoEmpty();
    bool IsRxFifoFull();
    bool IsTxFifoEmpty();
    bool IsTxFifoFull();
    void SetRxPayloadWidth( RFM73_Pipe_t pipe, uint8_t ploadWidth );
    RFM73_Pipe_t GetRxDataSource();
    void SeIrqMode( RFM73_IrqSource_t intSource, bool irqState );
    void SetPllMode( bool isPllLock );
    RFM73_RxFifoStatus_t GetRxFifoStatus();
    RFM73_TxFifoStatus_t GetTxFifoStatus();
    void EnableContiniousWave( bool enable ); // Enable continuous carrier transmission
    uint8_t GetRxPayloadWidth( RFM73_Pipe_t pipe );
    void SetAutoRetransmission( uint8_t retrCnt, uint16_t delay );
    uint8_t GetAutoRetransmittedCounter();
    uint8_t GetLostPacketCounter();
    void ClearIrqFlag( RFM73_IrqSource_t intSource );
    bool GetReuseTxStatus();
    void SetPrimaryTxMode();
    void SetPrimaryRxMode();
    void OpenPipe( RFM73_Pipe_t pipe,  const uint8_t *addr, bool autoAckEnable, bool dynPayloadEnable );
    void ClosePipe( RFM73_Pipe_t pipe );
    void SetRxPipeAddress( RFM73_Pipe_t pipe, const uint8_t *addr );
    void GetRxPipeAddress( RFM73_Pipe_t pipe, uint8_t *addr );
    void SetTxAddress( const uint8_t *addr );
    uint32_t GetChipId();
    void Activate( uint8_t value );
    void WriteTxPayload     ( const uint8_t* pPayload, uint8_t len );
    void WriteTxPayloadNoAck( const uint8_t* pPayload, uint8_t len );
    bool IsFeaturesActivate();
    void FeaturesActivate();
    void FeaturesDeActivate();
    void EnterToPowerDownMode();
    void EnterToStandByModeI();

public:    
   // --------------- RFM73 Commands ----------------- //   
    RFM73_StatusReg_t Nop();
    uint8_t  ReadReg(  RFM73_RegisterBank0_t reg );            
    uint32_t ReadReg( RFM73_RegisterBank1_t reg );         
    void     ReadReg(  RFM73_RegisterBank0_t reg, uint8_t* pData, uint8_t len );         
        
    RFM73_StatusReg_t  WriteReg(  RFM73_RegisterBank0_t reg, uint8_t value );
    void     WriteReg( RFM73_RegisterBank1_t reg, uint32_t value ); //!!!
    void     WriteReg(  RFM73_RegisterBank0_t reg, const uint8_t* pData, uint8_t len );         
        
    void     FlushRxFifo();
    void     FlushTxFifo();
    void     ReuseLastTxPayload();
    uint8_t  ReadRxPayload      ( uint8_t* pPayload, uint8_t len );
    void     WriteAckPayload    ( uint8_t pipe, const uint8_t* pPayload, uint8_t len );
    uint8_t  GetRxPayloadWidth();  
    // ------------------------------------------------ //

public:
    // ---------- Connection MCU <-> RFM73 ------------ //  

    void ChipEnableClr();
    void ChipEnableSet();  
    void DelayUsec( uint32_t msec ); 
    
private:   
    void ConnectionInit(); 
  
    void SpiChipSelectClr();
    void SpiChipSelectSet();
  
    uint8_t SpiRead();
    uint8_t SpiWrite( uint8_t byte );
    
    void WriteData( uint8_t* pData, uint8_t len );
    void ReadData ( uint8_t* pData, uint8_t len );
    // ------------------------------------------------ //
    
private:
    void EnabplePipeDynamicPayload( RFM73_Pipe_t pipe );
    void DisablePipeDynamicPayload( RFM73_Pipe_t pipe );

private:
    RFM73_ControlInterface_t* m_pControlInterface;
// --------------- RFM73 Commands ----------------- // 
    RFM73_RegBank_t GetActiveRegBank();
    void InitBank1();     
// ------------------------------------------------ //
};

#endif